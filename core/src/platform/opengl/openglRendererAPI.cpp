#include "gravpch.h"
#include "openglRendererAPI.h"

#include <glad/glad.h>

GRAVEngine::Rendering::rendererAPI* GRAVEngine::Rendering::rendererAPI::s_Instance = nullptr;

void GRAVEngine::Rendering::openglMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         GRAV_LOG_CRITICAL(message); return;
	case GL_DEBUG_SEVERITY_MEDIUM:       GRAV_LOG_ERROR(message); return;
	case GL_DEBUG_SEVERITY_LOW:          GRAV_LOG_WARN(message); return;
	case GL_DEBUG_SEVERITY_NOTIFICATION: GRAV_LOG_TRACE(message); return;
	}

	// Unknown severity level
	GRAV_ASSERT(false);
}

void GRAVEngine::Rendering::openglRendererAPI::startup()
{
	GRAV_PROFILE_FUNCTION();

	// Enable opengl message logging
#ifdef GRAVCORE_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GRAVEngine::Rendering::openglMessageCallback, nullptr);


	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

	// Enable vertex color blending
	glEnable(GL_BLEND);
	// Set the blending function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	s_Instance = this;
}

void GRAVEngine::Rendering::openglRendererAPI::setViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
	// Set the opengl viewport
	glViewport(x, y, width, height);
}

void GRAVEngine::Rendering::openglRendererAPI::setClearColor(const glm::vec4& color)
{
	// Set the clear color
	glClearColor(color.r, color.g, color.b, color.a);
}

void GRAVEngine::Rendering::openglRendererAPI::clear()
{
	// Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GRAVEngine::Rendering::openglRendererAPI::drawIndexed(const ref<vertexArray>& vertexArray, uint32 indexCount)
{
	GRAV_PROFILE_FUNCTION();

	// Get the index count. Either the number of provided indexes for the amount of indexes in the index buffer
	uint32 count = indexCount ? indexCount : vertexArray->getIndexBuffer()->getCount();

	// Draw the elements
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

	// Bind null texture
	glBindTexture(GL_TEXTURE_2D, 0);
}