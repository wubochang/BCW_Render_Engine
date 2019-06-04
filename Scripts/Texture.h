
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "glad_include.h"
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include <iostream>

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(const char*);
	bool Initialize(const GLenum &format, GLint internalFormat, int width, int height, GLenum type, unsigned char * data);


	void SetBuffers(unsigned int slot);
	unsigned int GetTextureID() { return m_textureID; }

	unsigned int GetWidth() { return m_width; }
	unsigned int GetHeight() { return m_height; }

	void SetParameteri(GLenum pname, GLint param);
	void SetParameterfv(GLenum pname, const GLfloat* params);


protected:

	unsigned int m_textureID;

	unsigned int m_width;
	unsigned int m_height;

	void GenerateTexture(const GLenum & format, GLint internalFormat, int width, int height, GLenum type, unsigned char * data);

};

#endif // !_TEXTURE_H_
