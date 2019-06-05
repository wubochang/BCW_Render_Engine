#include "TextRenderer.h"

TextRenderer g_textRenderer;

TextRenderer::TextRenderer()
{
}

TextRenderer::~TextRenderer()
{
}

bool TextRenderer::Initialize()
{
	bool res = true;
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		res = false;
	}

	FT_Face face;
	if (FT_New_Face(ft, "../Fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		res = false;
	}

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << c << std::endl;
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		m_characters[c] = character;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// init shader
	m_textShader = ShaderManager::getInstance().GetShader("TextShader");
	m_uiMesh = new UIMesh();
	m_uiMesh->Initialize();

	return res;
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	m_textShader->Use();
	m_textShader->SetVec3("textColor", color);
	m_textShader->SetVec2("screenSize", glm::vec2(SCR_WIDTH, SCR_HEIGHT));
	m_textShader->SetInt("textTexture", 0);

	m_uiMesh->SetBuffers();

	for (char c : text)
	{
		Character ch = m_characters[c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat x_center = xpos + w / 2.0f;
		GLfloat y_center = ypos + h / 2.0f;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		m_textShader->SetVec2("screenPos", glm::vec2(x_center, y_center));
		m_textShader->SetVec2("widthHeight", glm::vec2(w, h));
		m_textShader->SetFloat("zOrder", 0.0);

		x += (ch.Advance >> 6) * scale; 

		glDrawElements(GL_TRIANGLES, m_uiMesh->GetVertCnt(), GL_UNSIGNED_INT, 0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}
