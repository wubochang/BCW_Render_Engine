
#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "Vertex.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <glm/gtx/quaternion.hpp>

class Transform
{
public:
	Transform();
	Transform(const Transform&);
	~Transform();

	glm::mat4x4 GetWorldMatrix() { return m_worldMatrix; }
	void SetWorldMatrix(glm::mat4x4 mat) { m_worldMatrix = mat; }

	glm::vec3 GetPosition();
	glm::vec3 GetRotationEuler();
	glm::quat GetRotationQuat();
	glm::vec3 GetScale();

	glm::vec3 GetForward();
	glm::vec3 GetUp();
	glm::vec3 GetRight();

	void SetFowardUp(glm::vec3 forward, glm::vec3 up);

	void SetPosition(glm::vec3);
	void SetRotationEuler(glm::vec3);
	void SetRotationQuat(glm::quat);
	void SetScale(glm::vec3);

private:

	glm::mat4x4 m_worldMatrix;

	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale;

	glm::vec3 m_skew;
	glm::vec4 m_perspective;

	void UpdateMatrix()
	{
		m_worldMatrix = glm::mat4x4(1.0f);
		m_worldMatrix = glm::translate(m_worldMatrix, m_position);
		glm::mat4x4 rotationMat = glm::toMat4(m_rotation);
		m_worldMatrix = m_worldMatrix * rotationMat;
		m_worldMatrix = glm::scale(m_worldMatrix, m_scale);
	}

	void UpdateScaleRotPos()
	{
		glm::decompose(m_worldMatrix, m_scale, m_rotation, m_scale, m_skew, m_perspective);
	}
};


static glm::quat toQuaternion(glm::vec3 euler)
{
	glm::quat q = glm::quat(euler);
	return q;
}

static glm::vec3 toEulerAngle (glm::quat quat)
{
	glm::vec3 eulerAngles;
	float x, y, z, w;

	x = quat.x;
	y = quat.y;
	z = quat.z;
	w = quat.w;

	glm::mat4x4 mat = glm::toMat4(quat);

#define CLAMP(x , min , max) ((x) > (max) ? (max) : ((x) < (min) ? (min) : x))

	float roll = atan2(2 * (w * z + x * y), 1 - 2 * (z * z + x * x));
	float pitch = asin(CLAMP(2 * (w * x - y * z), -1.0f, 1.0f));
	float yaw = atan2(2 * (w * y + z * x), 1 - 2 * (x * x + y * y));

	eulerAngles.x = pitch;
	eulerAngles.y = yaw;
	eulerAngles.z = roll;

	return eulerAngles;
}

#endif // !_TRANSFORM_H
