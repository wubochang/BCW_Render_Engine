#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
{
}

Texture::Texture(const Texture &)
{
}

Texture::~Texture()
{
}

bool Texture::Initialize(const char* fileName)
{
	glGenTextures(1, &m_textureID);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(fileName, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		GLenum type = GL_UNSIGNED_BYTE;
		GLint internalFormat = GL_RGB8;

		if (format == GL_RED)
		{
			internalFormat = GL_R8;
			//type = GL_UNSIGNED_BYTE;
		}
		else if (format == GL_RGB)
		{
			internalFormat = GL_RGB8;
			//type = GL_FLOAT;
		}
		else if (format == GL_RGBA)
		{
			//int aaa = 1;
			//type = GL_FLOAT;
			internalFormat = GL_RGBA8;
		}

		GenerateTexture(format,  internalFormat, width, height, type, data);
	}
	else
	{
		std::cout << "Failed to load texture " << fileName << std::endl;
		return false;
	}
	stbi_image_free(data);
	return true;
}

bool Texture::Initialize(const GLenum &format, GLint internalFormat, int width, int height, GLenum type, unsigned char * data)
{
	glGenTextures(1, &m_textureID);
	GenerateTexture(format, internalFormat, width, height, type, data);
	return true;
}

void Texture::GenerateTexture(const GLenum & format, GLint internalFormat, int width, int height, GLenum type, unsigned char * data)
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	m_format = format;
	m_internalFormat = internalFormat;
	m_width = width;
	m_height = height;
	m_type = type;
}

void Texture::ResizeTexture(int newWidth, int newHeight)
{
	GenerateTexture(m_format, m_internalFormat, newWidth, newHeight, m_type, nullptr);
}

void Texture::SetBuffers(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot); 
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::SetParameteri(GLenum pname, GLint param)
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, pname, param);
}

void Texture::SetParameterfv(GLenum pname, const GLfloat * params)
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameterfv(GL_TEXTURE_2D, pname, params);
}
