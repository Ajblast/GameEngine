#include "gravpch.h"
#include "rendererCommand.h"

// Create the static instance of the renderering API
GRAVEngine::Rendering::rendererAPI* GRAVEngine::Rendering::rendererCommand::s_RendererAPI = GRAVEngine::Rendering::rendererAPI::create();

void GRAVEngine::Rendering::rendererCommand::startup()
{
	s_RendererAPI->startup();
}

void GRAVEngine::Rendering::rendererCommand::setViewPort(uint32 x, uint32 y, uint32 width, uint32 height)
{
	s_RendererAPI->setViewport(x, y, width, height);
}

void GRAVEngine::Rendering::rendererCommand::setClearColor(const glm::vec4& color)
{
	s_RendererAPI->setClearColor(color);
}

void GRAVEngine::Rendering::rendererCommand::clear()
{
	s_RendererAPI->clear();
}

void GRAVEngine::Rendering::rendererCommand::drawIndexed(vertexArray*& vertexArray, uint32 count)
{
	s_RendererAPI->drawIndexed(vertexArray, count);
}
