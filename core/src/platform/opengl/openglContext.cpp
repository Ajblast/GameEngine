#include "gravpch.h"
#include "openglContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

GRAVEngine::Rendering::openglContext::openglContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
{
	GRAV_ASSERT(windowHandle);
	// Assure that the window is valid
}

void GRAVEngine::Rendering::openglContext::startup()
{
	// Make this context's window the current context
	glfwMakeContextCurrent(m_WindowHandle);

	// Load glad
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	GRAV_ASSERT(status);
}

void GRAVEngine::Rendering::openglContext::swapBuffers()
{
	// Swap the buffers
	glfwSwapBuffers(m_WindowHandle);
}
