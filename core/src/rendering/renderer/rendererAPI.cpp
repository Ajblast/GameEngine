#include "gravpch.h"
#include "rendererAPI.h"

#include "platform/opengl/openglRendererAPI.h"
#include "platform/opengl/buffers/openglIndexBuffer.h"
#include "platform/opengl/buffers/openglVertexBuffer.h"
#include "platform/opengl/openglShader.h"
#include "platform/opengl/openglVertexArray.h"
#include "platform/opengl/openglTexture2D.h"
#include "platform/opengl/openglContext.h"

GRAVEngine::Rendering::rendererAPI::API GRAVEngine::Rendering::rendererAPI::s_API = GRAVEngine::Rendering::rendererAPI::API::OpenGL;

GRAVEngine::scope<GRAVEngine::Rendering::rendererAPI> GRAVEngine::Rendering::rendererAPI::create()
{
	switch (s_API)
	{
	case Rendering::rendererAPI::API::NONE:    GRAV_ASSERT(false); return nullptr; // RendererAPI::None is currently not supported!
	case Rendering::rendererAPI::API::OpenGL:  return createScope<openglRendererAPI>();
	default: GRAV_ASSERT(false); return nullptr;
	}
}

GRAVEngine::ref<GRAVEngine::Rendering::indexBuffer> GRAVEngine::Rendering::rendererAPI::createIndexBuffer(uint32* indices, uint32 count)
{
	switch (s_API)
	{
	case Rendering::rendererAPI::API::NONE:    GRAV_ASSERT(false); return nullptr; // RendererAPI::None is currently not supported!
	case Rendering::rendererAPI::API::OpenGL:  return createRef<openglIndexBuffer>(indices, count);
	default: GRAV_ASSERT(false); return nullptr;
	}	
}
GRAVEngine::ref<GRAVEngine::Rendering::vertexBuffer> GRAVEngine::Rendering::rendererAPI::createVertexBuffer(uint32 size)
{
	switch (s_API)
	{
	case Rendering::rendererAPI::API::NONE:    GRAV_ASSERT(false); return nullptr; // RendererAPI::None is currently not supported!
	case Rendering::rendererAPI::API::OpenGL:  return createRef<openglVertexBuffer>(size);
	default: GRAV_ASSERT(false); return nullptr;
	}
	
}
GRAVEngine::ref<GRAVEngine::Rendering::vertexBuffer> GRAVEngine::Rendering::rendererAPI::createVertexBuffer(float* vertices, uint32 size)
{
	switch (s_API)
	{
	case Rendering::rendererAPI::API::NONE:    GRAV_ASSERT(false); return nullptr; // RendererAPI::None is currently not supported!
	case Rendering::rendererAPI::API::OpenGL:  return createRef<openglVertexBuffer>(vertices, size);
	default: GRAV_ASSERT(false); return nullptr;
	}
	
}
GRAVEngine::ref<GRAVEngine::Rendering::vertexArray> GRAVEngine::Rendering::rendererAPI::createVertexArray()
{
	switch (s_API)
	{
	case Rendering::rendererAPI::API::NONE:    GRAV_ASSERT(false); return nullptr; // RendererAPI::None is currently not supported!
	case Rendering::rendererAPI::API::OpenGL:  return createRef<openglVertexArray>();
	default: GRAV_ASSERT(false); return nullptr;
	}	
}

GRAVEngine::ref<GRAVEngine::Rendering::shader> GRAVEngine::Rendering::rendererAPI::createShader(const std::string& filePath)
{
	switch (s_API)
	{
	case Rendering::rendererAPI::API::NONE:    GRAV_ASSERT(false); return nullptr; // RendererAPI::None is currently not supported!
	case Rendering::rendererAPI::API::OpenGL:  return createRef<openglShader>(filePath);
	default: GRAV_ASSERT(false); return nullptr;
	}	
}
GRAVEngine::ref<GRAVEngine::Rendering::shader> GRAVEngine::Rendering::rendererAPI::createShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
{
	switch (s_API)
	{
	case Rendering::rendererAPI::API::NONE:    GRAV_ASSERT(false); return nullptr; // RendererAPI::None is currently not supported!
	case Rendering::rendererAPI::API::OpenGL:  return createRef<openglShader>(name, vertexSrc, fragmentSrc);
	default: GRAV_ASSERT(false); return nullptr;
	}	
}

GRAVEngine::ref<GRAVEngine::Rendering::texture2D> GRAVEngine::Rendering::rendererAPI::createTexture(uint32 width, uint32 height)
{
	switch (s_API)
	{
	case Rendering::rendererAPI::API::NONE:    GRAV_ASSERT(false); return nullptr; // RendererAPI::None is currently not supported!
	case Rendering::rendererAPI::API::OpenGL:  return createRef<openglTexture2D>(width, height);
	default: GRAV_ASSERT(false); return nullptr;
	}	
}
GRAVEngine::ref<GRAVEngine::Rendering::texture2D> GRAVEngine::Rendering::rendererAPI::createTexture(const std::string& path)
{
	switch (s_API)
	{
	case Rendering::rendererAPI::API::NONE:    GRAV_ASSERT(false); return nullptr; // RendererAPI::None is currently not supported!
	case Rendering::rendererAPI::API::OpenGL:  return createRef<openglTexture2D>(path);
	default: GRAV_ASSERT(false); return nullptr;
	}	
}

GRAVEngine::scope<GRAVEngine::Rendering::graphicsContext> GRAVEngine::Rendering::rendererAPI::createContext(void* window)
{
	switch (s_API)
	{
	case Rendering::rendererAPI::API::NONE:    GRAV_ASSERT(false); return nullptr; // RendererAPI::None is currently not supported!
	case Rendering::rendererAPI::API::OpenGL:  return createScope<openglContext>(static_cast<GLFWwindow*>(window));
	default: GRAV_ASSERT(false); return nullptr;
	}	
}
