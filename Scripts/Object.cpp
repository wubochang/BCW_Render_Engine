#include "Object.h"

Object::Object()
{
}

Object::Object(const Object &)
{
}

Object::~Object()
{
}

void Object::Initialize(Mesh* pMesh)
{
	m_mesh = pMesh;

	m_transform = new Transform();
	m_transform->SetPosition(glm::vec3(0, 0, 0));
}

void Object::Shutdown()
{
}
