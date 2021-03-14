#pragma once

#include "common.h"
#include "rendering/buffers/indexBuffer.h"
#include "rendering/buffers/vertexBuffer.h"
#include "rendering/vertexArray.h"
#include "rendering/shaders/shader.h"
#include "rendering/textures/texture2D.h"
#include <glm/vec4.hpp>

namespace GRAVEngine
{
	namespace Rendering
	{
		// Generic interface for a rendering API
		class rendererAPI
		{
		public:
			enum class API
			{
				NONE = 0, OpenGL = 1
			};

		public:
			// Abstract API deconstructor
			virtual ~rendererAPI() = default;

			// Startup the API
			virtual void startup() = 0;
			// Set the API's viewport
			virtual void setViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;

			// Set the API's clear color
			virtual void setClearColor(const glm::vec4& color) = 0;
			// Clear the viewport
			virtual void clear() = 0;

			// Draw a vertex array by indices
			virtual void drawIndexed(vertexArray*& vertexArray, uint32 indexCount = 0) = 0;


#pragma region FactoryMethods
			virtual indexBuffer* createIndexBuffer(uint32* indices, uint32 count) = 0;
			virtual vertexBuffer* createVertexBuffer(uint32 size) = 0;
			virtual vertexBuffer* createVertexBuffer(float* vertices, uint32 size) = 0;
			virtual vertexArray* createVertexArray() = 0;

			// Create a shader from a file
			virtual shader* createShader(const std::string& filePath) = 0;
			// Create a shader with a vertex and fragment string
			virtual shader* createShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) = 0;

			virtual texture2D* createTexture(uint32 width, uint32 height) = 0;
			virtual texture2D* createTexture(const std::string& path) = 0;
#pragma endregion


			// Get the current API
			static API getAPI() { return s_API; }
			// Create an API. TODO: Remove this method somehow
			static rendererAPI* create();

			inline static rendererAPI* getInstance() { return s_Instance; }
		protected:
			static rendererAPI* s_Instance;
		private:
			static API s_API;
		};
	}
}