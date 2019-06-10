#include "UIObject.h"

UIObject::UIObject()
{
}

UIObject::~UIObject()
{
}

void UIObject::Initialize(Texture* tex)
{
	m_shader = ShaderManager::getInstance().GetShader("UIShader");
	Initialize(m_shader, tex);
}

void UIObject::Initialize(Shader * shader, Texture * tex)
{
	m_shader = shader;

	m_mesh = UIMesh::GetMesh();

	if (tex == nullptr)
	{
		m_texture = new Texture();
		m_texture->Initialize("uv_checker.png");
	}
	else
	{
		m_texture = tex;
	}

	widthHeight = glm::vec2(100, 100);
	screenPos = glm::vec2(widthHeight.x * 0.5f, widthHeight.y * 0.5f);
}

void UIObject::RenderUIObject(float z)
{
	m_shader->Use();
	m_shader->SetTexture("uiTex", m_texture, 4);
	m_shader->SetVec2("screenSize", glm::vec2(SCR_WIDTH, SCR_HEIGHT));
	m_shader->SetVec2("screenPos", screenPos);
	m_shader->SetVec2("widthHeight", widthHeight);
	m_shader->SetFloat("zOrder", z);
	m_mesh->SetBuffers();
	glDrawElements(GL_TRIANGLES, m_mesh->GetVertCnt(), GL_UNSIGNED_INT, 0);
}
