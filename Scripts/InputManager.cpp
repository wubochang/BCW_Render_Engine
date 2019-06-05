#include "InputManager.h"

void InputManager::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	InputManager::getInstance().m_mousePosition.x = xpos;
	InputManager::getInstance().m_mousePosition.y = ypos;
}

void InputManager::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	Camera::GetMainCamera().ProcessMouseScroll(yoffset);
}
