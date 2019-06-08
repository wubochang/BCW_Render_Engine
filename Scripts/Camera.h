
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Transform.h"
#include "GLWindow.h"

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	static Camera& GetMainCamera()
	{
		static Camera instance;
		return instance;
	}

	bool Initialize();
	void Shutdown();

	glm::mat4x4 GetViewMatrix();
	glm::mat4x4 GetProjectionMatrix();

	void ProcessMouseScroll(double yoffset);

	glm::vec3 GetPosition();
	glm::vec3 GetPivot() { return m_pivot; }

	void Update();

private:

	glm::vec3 m_pivot;
	float m_theta;
	float m_phi;
	float m_radius;

	glm::vec2 m_mouseStartPos;

	float m_startTheta;
	float m_startPhi;
	bool m_rotating;

	glm::vec3 m_startPivot;
	bool m_moving;

	glm::vec3 GetForward()
	{
		return glm::normalize(m_pivot - GetPosition());
	}
	
	glm::vec3 GetUp()
	{
		return glm::cross(GetRight(), GetForward());
	}

	glm::vec3 GetRight()
	{
		return glm::cross(GetForward(), glm::vec3(0, 1, 0));
	}

	void ProcessRotateEvent();
	void ProcessMoveEvent();
};

#endif // !_CAMERA_H_
