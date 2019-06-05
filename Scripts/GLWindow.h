
#ifndef _GL_WINDOW_H
#define _GL_WINDOW_H

#include "glad_include.h"
#include <GLFW/glfw3.h>

#include <iostream>

#include "ObjectManager.h"
#include "Render/RenderManager.h"
#include "Shader/ShaderManager.h"
#include "MaterialManager.h"
#include "InputManager.h"
#include "Timer.h"

#define SCR_WIDTH (800)
#define SCR_HEIGHT (600)

#define SHADOW_WIDTH (1024)
#define SHADOW_HEIGHT (1024)

class GLWindow
{
public:
	GLWindow();
	GLWindow(const GLWindow&);
	~GLWindow();

	bool Initialize(int width, int height);
	void InitManagers();
	void Shutdown();

	bool Run();
	void RenderScene();

private:

	GLFWwindow* m_window;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}
};

#endif // !_GL_WINDOW_H
