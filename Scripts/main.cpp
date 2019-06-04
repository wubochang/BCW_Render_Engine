
#include "glad_include.h"

#include <GLFW/glfw3.h>

#include "GLWindow.h"

int main()
{
	GLWindow* window = new GLWindow();

	window->Initialize(SCR_WIDTH, SCR_HEIGHT);

	while (window->Run())
	{
	}

	window->Shutdown();

	return 0;
}