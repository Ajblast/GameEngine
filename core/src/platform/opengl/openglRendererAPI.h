#pragma once

#include "common.h"

#include "rendering/renderer/rendererAPI.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		// Message callback for opengl when anything occurs
		void openglMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam);

		class openglRendererAPI : public rendererAPI
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
			virtual void drawIndexed(vertexArray*& vertexArray, uint32 indexCount = 0) override;

			// Create an index buffer
			virtual indexBuffer* createIndexBuffer(uint32* indices, uint32 count) override;
			// Create a vertex buffer
			virtual vertexBuffer* createVertexBuffer(uint32 size) override;
			// Create a vertex buffer
			virtual vertexBuffer* createVertexBuffer(float* vertices, uint32 size) override;
			// Create a vertex array
			virtual vertexArray* createVertexArray() override;

			// Create a shader
			virtual shader* createShader(const std::string& filePath) override;
			// Create a shader
			virtual shader* createShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) override;

			// Create a texture
			virtual texture2D* createTexture(uint32 width, uint32 height) override;
			// Create a texture
			virtual texture2D* createTexture(const std::string& path) override;
		};
	}
}