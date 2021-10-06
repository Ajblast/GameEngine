#pragma once

#include "common.h"

#include "rendering/renderer/rendererAPI.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		// Message callback for opengl when anything occurs
		GRAVAPI void openglMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam);

		class GRAVAPI openglRendererAPI : public rendererAPI
		{
			// Startup the API
			virtual void startup() override;
			// Set the API's viewport
			virtual void setViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
			
			// Set the API's clear color
			virtual void setClearColor(const glm::vec4& color) override;
			// Clear the viewport
			virtual void clear() override;
			
			// Draw a vertex array by indices
			virtual void drawIndexed(const ref<vertexArray>& vertexArray, uint32 indexCount = 0) override;
		};
	}
}