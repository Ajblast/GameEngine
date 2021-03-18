#include "gravpch.h"
#include "rendererAPI.h"

#include "platform/opengl/openglRendererAPI.h"

GRAVEngine::Rendering::rendererAPI::API GRAVEngine::Rendering::rendererAPI::s_API = GRAVEngine::Rendering::rendererAPI::API::OpenGL;

GRAVEngine::Rendering::rendererAPI* GRAVEngine::Rendering::rendererAPI::create()
{
	switch (s_API)
	{
	case Rendering::rendererAPI::API::NONE:    GRAV_ASSERT(false); return nullptr; // RendererAPI::None is currently not supported!
	case Rendering::rendererAPI::API::OpenGL:  return new openglRendererAPI();
	default: GRAV_ASSERT(false); return nullptr;
	}
}
