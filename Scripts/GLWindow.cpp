#include "GLWindow.h"

GLWindow::GLWindow()
{
}

GLWindow::GLWindow(const GLWindow &)
{
}

GLWindow::~GLWindow()
{
}

bool GLWindow::Initialize(int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, "BCW's Render Engine", NULL, NULL);
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, GLWindow::framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, InputManager::mouse_callback);
	glfwSetScrollCallback(m_window, InputManager::scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to InitializeGLAD" << std::endl;
		return false;
	}

	InitManagers();

	return true;
}

void GLWindow::InitManagers()
{
	g_materialManager.Initialize();

	// object manager needs to be initialized AFTER material manager
	g_objectManager.Initialize();

	g_renderManager.Initialize();
	g_shaderManager.Initialize();
}

void GLWindow::Shutdown()
{
	glfwTerminate();
}

void GLWindow::RenderScene()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}


bool GLWindow::Run()
{
	static float oldTime = glfwGetTime();

	g_inputManager.processInput(m_window);

	double newTime = glfwGetTime();
	float deltaTime = newTime - oldTime;
	oldTime = newTime;
	g_objectManager.Update(deltaTime);
	g_renderManager.Render();

	RenderScene();

	return !glfwWindowShouldClose(m_window);
}

