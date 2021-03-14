#pragma once

#include "common.h"
#include "rendering/vertexArray.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		// OpenGL version of a vertex array
		class openglVertexArray : public vertexArray
		{
		public:
			openglVertexArray();
			virtual ~openglVertexArray();

			// Bind this vertex array
			virtual void bind() const override;
			// Unbind this vertex array
			virtual void unbind() const override;

			// Add a vertex buffer
			virtual void addVertexBuffer(vertexBuffer* vertexBuffer) override;
			// Set the index buffer
			virtual void setIndexBuffer(indexBuffer* indexBuffer) override;

			// Get an array of the vertex buffers
			virtual const std::vector<vertexBuffer*>& getVertexBuffers() const override { return m_VertexBuffers; }
			// Get the index buffer
			virtual const indexBuffer* getIndexBuffer() const override { return m_IndexBuffer; }
		private:
			uint32 m_RendererID;
			uint32 m_VertexBufferIndex = 0;
			std::vector<vertexBuffer*> m_VertexBuffers;	// The vertex buffers
			indexBuffer* m_IndexBuffer;					// The index buffer of how vertices should be read

		};
	}
}