#include "gravpch.h"
#include "openglRendererAPI.h"

#include "buffers/openglIndexBuffer.h"
#include "buffers/openglVertexBuffer.h"
#include "openglShader.h"
#include "openglVertexArray.h"
#include "openglTexture2D.h"

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
	// Enable opengl message logging
#ifdef GRAV_DEBUG
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

void GRAVEngine::Rendering::openglRendererAPI::drawIndexed(vertexArray*& vertexArray, uint32 indexCount)
{
	// Get the index count. Either the number of provided indexes for the amount of indexes in the index buffer
	uint32 count = indexCount ? indexCount : vertexArray->getIndexBuffer()->getCount();

	// Draw the elements
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

	// Bind null texture
	glBindTexture(GL_TEXTURE_2D, 0);
}


#pragma region FactoryMethods
GRAVEngine::Rendering::indexBuffer* GRAVEngine::Rendering::openglRendererAPI::createIndexBuffer(uint32* indices, uint32 count)
{
	return new openglIndexBuffer(indices, count);
}
GRAVEngine::Rendering::vertexBuffer* GRAVEngine::Rendering::openglRendererAPI::createVertexBuffer(uint32 size)
{
	return new openglVertexBuffer(size);
}
GRAVEngine::Rendering::vertexBuffer* GRAVEngine::Rendering::openglRendererAPI::createVertexBuffer(float* vertices, uint32 size)
{
	return new openglVertexBuffer(vertices, size);
}
GRAVEngine::Rendering::vertexArray* GRAVEngine::Rendering::openglRendererAPI::createVertexArray()
{
	return new openglVertexArray();
}
GRAVEngine::Rendering::shader* GRAVEngine::Rendering::openglRendererAPI::createShader(const std::string& filePath)
{
	return new openglShader(filePath);
}
GRAVEngine::Rendering::shader* GRAVEngine::Rendering::openglRendererAPI::createShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
{
	return new openglShader(name, vertexSrc, fragmentSrc);
}
GRAVEngine::Rendering::texture2D* GRAVEngine::Rendering::openglRendererAPI::createTexture(uint32 width, uint32 height)
{
	return new openglTexture2D(width, height);
}
GRAVEngine::Rendering::texture2D* GRAVEngine::Rendering::openglRendererAPI::createTexture(const std::string& path)
{
	return new openglTexture2D(path);
}
#pragma endregion