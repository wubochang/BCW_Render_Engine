#include "InputManager.h"

InputManager g_inputManager;

InputManager::InputManager()
{
	ZeroMemory(&m_keys, sizeof(m_keys));
	ZeroMemory(&m_keysPrev, sizeof(m_keysPrev));
	ZeroMemory(&m_mouseKeys, sizeof(m_mouseKeys));
}

InputManager::InputManager(const InputManager &)
{
}

InputManager::~InputManager()
{
}

void InputManager::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	g_inputManager.m_mousePosition.x = xpos;
	g_inputManager.m_mousePosition.y = ypos;
}

void InputManager::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	g_mainCamera.ProcessMouseScroll(yoffset);
}
