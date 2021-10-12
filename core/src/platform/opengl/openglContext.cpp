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
	GRAV_PROFILE_FUNCTION();

	// Make this context's window the current context
	bind();

	// Load glad
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	GRAV_ASSERT(status);

	GRAV_LOG_INFO("OpenGL Info:");
	GRAV_LOG_INFO("  Vendor: %s", glGetString(GL_VENDOR));
	GRAV_LOG_INFO("  Renderer: %s", glGetString(GL_RENDERER));
	GRAV_LOG_INFO("  Version: %s\n", glGetString(GL_VERSION));

	// GRAVEngine requires at least OpenGL version 4.5!"
	GRAV_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5));

	//unbind();
}

void GRAVEngine::Rendering::openglContext::swapBuffers()
{
	GRAV_PROFILE_FUNCTION();

	// Swap the buffers
	glfwSwapBuffers(m_WindowHandle);
}

void GRAVEngine::Rendering::openglContext::bind()
{
	glfwMakeContextCurrent(m_WindowHandle);
}

void GRAVEngine::Rendering::openglContext::unbind()
{
	glfwMakeContextCurrent(0);
}
