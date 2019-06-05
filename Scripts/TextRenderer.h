#ifndef _TEXT_RENDERER_H_
#define _TEXT_RENDERER_H_

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader/Shader.h"
#include "Shader/ShaderManager.h"
#include "UIMesh.h"
#include "GLWindow.h"

#include <iostream>
#include <unordered_map>

struct Character
{
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};


class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	bool Initialize();

	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:

	std::unordered_map<GLchar, Character> m_characters;

	Shader* m_textShader;
	UIMesh* m_uiMesh;
};

extern TextRenderer g_textRenderer;

#endif // !_TEXT_RENDERER_H_
