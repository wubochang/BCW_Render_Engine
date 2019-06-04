
#ifndef _UI_OBJECT_H_
#define _UI_OBJECT_H_

#include "Shader/Shader.h"
#include "UIMesh.h"
#include "GLWindow.h"

class UIObject
{
public:
	UIObject();
	~UIObject();

	void Initialize(Texture* tex = nullptr);
	void Initialize(Shader* shader, Texture* tex = nullptr);

	void SetTexture(Texture* tex)
	{
		m_texture = tex;
	}

	void RenderUIObject(float order);

	glm::vec2 screenPos;
	glm::vec2 widthHeight;

private:

	Shader* m_shader;
	UIMesh* m_mesh;
	Texture* m_texture;
};

#endif // !_UI_OBJECT_H_
