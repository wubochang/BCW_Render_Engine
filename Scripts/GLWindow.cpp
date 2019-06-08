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

	InitVSync();

	return true;
}

bool GLWindow::InitVSync()
{
	// glGetString not working for no reason
	//char* extensions = (char*)glGetString(GL_EXTENSIONS);
	//if (strstr(extensions, "WGL_EXT_swap_control")) {
	wglSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglGetSwapIntervalEXT = (PFNWGLEXTGETSWAPINTERVALPROC)wglGetProcAddress("wglGetSwapIntervalEXT");


	// enable vsync by default
	SetVSyncState(true);
	return true;
}

void GLWindow::InitManagers()
{
	MaterialManager::getInstance().Initialize();

	// object manager needs to be initialized AFTER material manager
	ObjectManager::getInstance().Initialize();

	RenderManager::getInstance().Initialize();
	ShaderManager::getInstance().Initialize();
	InputManager::getInstance().Initialize();

	RenderManager::getInstance().ResizeScreen(SCR_WIDTH, SCR_HEIGHT);
}

void GLWindow::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);

	RenderManager::getInstance().ResizeScreen(width, height);
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
	Timer::getInstance().Tick();

	InputManager::getInstance().processInput(m_window);

	ObjectManager::getInstance().Update();
	RenderManager::getInstance().Render();

	RenderScene();

	if (InputManager::getInstance().GetKeyDown(GLFW_KEY_V))
	{
		SetVSyncState(!IsVSyncEnabled());
	}

	return !glfwWindowShouldClose(m_window);
}

