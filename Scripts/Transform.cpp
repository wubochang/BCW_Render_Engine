#include "Transform.h"

Transform::Transform()
{
	m_worldMatrix = glm::mat4x4(1.0f);

	m_position = glm::vec3(0, 0, 0);
	m_rotation = toQuaternion(glm::vec3(0, 0, 0));
	m_scale = glm::vec3(1, 1, 1);

	UpdateMatrix();
}

Transform::Transform(const Transform &)
{
}

Transform::~Transform()
{
}

glm::vec3 Transform::GetPosition()
{
	UpdateScaleRotPos();
	return m_position;
}

glm::vec3 Transform::GetRotationEuler()
{
	UpdateScaleRotPos();
	return toEulerAngle(m_rotation);
}

glm::quat Transform::GetRotationQuat()
{
	UpdateScaleRotPos();
	return m_rotation;
}

glm::vec3 Transform::GetScale()
{
	UpdateScaleRotPos();
	if (m_scale.x < 0) m_scale.x *= -1;
	if (m_scale.y < 0) m_scale.y *= -1;
	if (m_scale.z < 0) m_scale.z *= -1;
	return m_scale;
}

glm::vec3 Transform::GetForward()
{
	float scale = GetScale().z;
	glm::vec3 forward;
	forward.x = m_worldMatrix[2][0] / scale;
	forward.y = m_worldMatrix[2][1] / scale;
	forward.z = m_worldMatrix[2][2] / scale;
	if (glm::length(forward) <= 0.01f)
	{
		return glm::vec3(0, 0, 1);
	}
	return forward;
}

glm::vec3 Transform::GetUp()
{
	float scale = GetScale().y;
	glm::vec3 up;
	up.x = m_worldMatrix[1][0] / scale;
	up.y = m_worldMatrix[1][1] / scale;
	up.z = m_worldMatrix[1][2] / scale;
	if (glm::length(up) <= 0.01f)
	{
		return glm::vec3(0, 1, 0);
	}
	return up;
}

glm::vec3 Transform::GetRight()
{
	float scale = GetScale().x;
	glm::vec3 right;
	right.x = m_worldMatrix[0][0] / scale;
	right.y = m_worldMatrix[0][1] / scale;
	right.z = m_worldMatrix[0][2] / scale;
	if (glm::length(right) <= 0.01f)
	{
		return glm::vec3(1, 0, 0);
	}
	return right;
}

void Transform::SetFowardUp(glm::vec3 forward, glm::vec3 up)
{
	forward = glm::normalize(forward);
	glm::vec3 right = glm::cross(forward, up);
	right = glm::normalize(right);
	up = glm::cross(right, forward);

	glm::vec3 scale = GetScale();

	m_worldMatrix[0][0] = right.x * scale.x;
	m_worldMatrix[0][1] = right.y * scale.x;
	m_worldMatrix[0][2] = right.z * scale.x;

	m_worldMatrix[1][0] = up.x * scale.y;
	m_worldMatrix[1][1] = up.y * scale.y;
	m_worldMatrix[1][2] = up.z * scale.y;

	m_worldMatrix[2][0] = forward.x * scale.z;
	m_worldMatrix[2][1] = forward.y * scale.z;
	m_worldMatrix[2][2] = forward.z * scale.z;

	auto dir = GetForward();
	int aaa = 1;
}

void Transform::SetPosition(glm::vec3 pos)
{
	m_position = pos;
	UpdateMatrix();
}

void Transform::SetRotationEuler(glm::vec3 rotEuler)
{
	m_rotation = toQuaternion(rotEuler);
	UpdateMatrix();
}

void Transform::SetRotationQuat(glm::quat rot)
{
	m_rotation = rot;
	UpdateMatrix();
}

void Transform::SetScale(glm::vec3 scale)
{
	m_scale = scale;
	UpdateMatrix();
}
