#pragma once

#include "common.h"
#include "rendering/buffers/vertexBuffer.h"
#include "rendering/buffers/indexBuffer.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		// Stores vertex attribute configutations and which vertex buffers to use
		// Has a vertex buffer (VBO) and an index buffer (EBO)
		class vertexArray
		{
		public:
			virtual ~vertexArray() = default;

			// Bind this vertex array
			virtual void bind() const = 0;
			// Unbind this vertex array
			virtual void unbind() const = 0;

			// Add a vertex buffer
			virtual void addVertexBuffer(vertexBuffer* vertexBuffer) = 0;
			// Set the index buffer
			virtual void setIndexBuffer(indexBuffer* indexBuffer) = 0;

			// Get a list of the vertex buffers
			virtual const std::vector<vertexBuffer*>& getVertexBuffers() const = 0;
			// Get the index buffer
			virtual const indexBuffer* getIndexBuffer() const = 0;

			//static vertexArray* create();
		};
	}
}