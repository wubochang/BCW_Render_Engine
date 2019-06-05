
#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include "GLWindow.h"
#include "Camera.h"

#include "Misc.h"

class InputManager
{
	MAKE_SINGLETON(InputManager)

public:
	~InputManager() {};

	void Initialize()
	{
		ZeroMemory(&m_keys, sizeof(m_keys));
		ZeroMemory(&m_keysPrev, sizeof(m_keysPrev));
		ZeroMemory(&m_mouseKeys, sizeof(m_mouseKeys));
	}


	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	bool GetKeyDown(int keyCode)
	{
		return m_keys[keyCode] == KeyState::DOWN && m_keysPrev[keyCode] == KeyState::UP;
	}
	bool GetKeyUp(int keyCode)
	{
		return m_keys[keyCode] == KeyState::UP && m_keysPrev[keyCode] == KeyState::DOWN;
	}

	bool GetKey(int keyCode)
	{
		return m_keys[keyCode] == KeyState::DOWN;
	}

	bool GetMouse(int keyCode)
	{
		return m_mouseKeys[keyCode] == KeyState::DOWN;
	}

	void processInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		for (int i = 0; i <= GLFW_KEY_LAST; i++)
		{
			m_keysPrev[i] = m_keys[i];
			m_keys[i] = (KeyState)glfwGetKey(window, i);
		}

		for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++)
		{
			m_mouseKeys[i] = (KeyState)glfwGetMouseButton(window, i);
		}
	}

	glm::vec2 GetMousePosition()
	{
		return m_mousePosition;
	}

private:

	enum KeyState
	{
		NOTPRESSED = 0,
		DOWN = GLFW_PRESS,
		UP = GLFW_RELEASE,
	};

	KeyState m_keys[512];
	KeyState m_keysPrev[512];
	KeyState m_mouseKeys[8];

	glm::vec2 m_mousePosition;
};

#endif // !_INPUT_MANAGER_H_
