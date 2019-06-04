#include "Camera.h"

Camera g_mainCamera;

Camera::Camera()
{
	m_rotating = false;
	m_moving = false;
}

Camera::Camera(const Camera &)
{
}

Camera::~Camera()
{
}

bool Camera::Initialize()
{
	m_pivot = glm::vec3(0, 0, 0);
	m_radius = 5;
	m_phi = 0;
	m_theta = glm::radians(30.0f);
	return true;
}

void Camera::Shutdown()
{
}

glm::mat4x4 Camera::GetViewMatrix()
{
	glm::vec3 pos = GetPosition();
	glm::vec3 forward = GetForward();
	glm::vec3 up = GetUp();
	glm::mat4x4 viewMatrix = glm::lookAt(pos, pos + forward, up);
	return viewMatrix;
}

glm::mat4x4 Camera::GetProjectionMatrix()
{
	glm::mat4x4 m_projectionMatrix = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	return m_projectionMatrix;
}

void Camera::ProcessMouseScroll(double yoffset)
{
	m_radius -= yoffset;

	if (m_radius < 1.0f) m_radius = 1.0f;
}

glm::vec3 Camera::GetPosition()
{
	glm::vec3 pos;
	pos.x = m_radius * sin(m_theta) * cos(m_phi);
	pos.y = m_radius * cos(m_theta);
	pos.z = m_radius * sin(m_theta) * sin(m_phi);

	pos += m_pivot;

	return pos;
}

void Camera::Update()
{
	ProcessRotateEvent();
	ProcessMoveEvent();
}

void Camera::ProcessMoveEvent()
{
	bool movingPressed = g_inputManager.GetKey(GLFW_KEY_LEFT_ALT);
	movingPressed &= g_inputManager.GetMouse(GLFW_MOUSE_BUTTON_MIDDLE);

	if (movingPressed)
	{
		if (!m_moving)
		{
			m_moving = true;
			m_mouseStartPos = g_inputManager.GetMousePosition();
			m_startPivot = m_pivot;
		}
	}
	else
	{
		m_moving = false;
	}

	if (m_moving)
	{
		glm::vec2 mousePos = g_inputManager.GetMousePosition();
		glm::vec2 diff = mousePos - m_mouseStartPos;

		m_pivot = m_startPivot - diff.x * 0.005f * GetRight() + diff.y * 0.005f * GetUp();
	}
}

void Camera::ProcessRotateEvent()
{
	// process input
	bool rotatingPressed = g_inputManager.GetKey(GLFW_KEY_LEFT_ALT);
	rotatingPressed &= g_inputManager.GetMouse(GLFW_MOUSE_BUTTON_LEFT);

	if (rotatingPressed)
	{
		if (!m_rotating)
		{
			m_mouseStartPos = g_inputManager.GetMousePosition();
			m_startPhi = m_phi;
			m_startTheta = m_theta;
			m_rotating = true;
		}
	}
	else
	{
		m_rotating = false;
	}

	// update position
	if (m_rotating)
	{
		glm::vec2 mousePos = g_inputManager.GetMousePosition();
		glm::vec2 diff = mousePos - m_mouseStartPos;

		m_phi = m_startPhi + glm::radians(diff.x);

		// inverse y
		m_theta = m_startTheta - glm::radians(diff.y);

#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)

		m_theta = min(glm::radians(176.0f), max(m_theta, glm::radians(2.0f)));
	}
}
