#pragma once
#include "common.h"
#include "rendererAPI.h"

#include <glm/vec4.hpp>

namespace GRAVEngine
{
	namespace Rendering
	{
		// Abstract Renderer Command
		class rendererCommand
		{
		public:
			// Startup the rendering command system. Creates the renderer API
			static void startup();
			// Set the renderer's API
			static void setViewPort(uint32 x, uint32 y, uint32 width, uint32 height);
			// Set the renderer's clear color
			static void setClearColor(const glm::vec4& color);
			// Clear the rendering API
			static void clear();

			// Draw an indexed vertex array
			static void drawIndexed(vertexArray*& vertexArray, uint32 count = 0);
		private:
			static rendererAPI* s_RendererAPI;
		};
	}
}