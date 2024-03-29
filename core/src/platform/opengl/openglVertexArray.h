#pragma once

#include "common.h"
#include "rendering/vertexArray.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		// OpenGL version of a vertex array
		class GRAVAPI openglVertexArray : public vertexArray
		{
		public:
			openglVertexArray();
			virtual ~openglVertexArray();

			// Bind this vertex array
			virtual void bind() const override;
			// Unbind this vertex array
			virtual void unbind() const override;

			// Add a vertex buffer
			virtual void addVertexBuffer(const ref<vertexBuffer>& vertexBuffer) override;
			// Set the index buffer
			virtual void setIndexBuffer(const ref<indexBuffer>& indexBuffer) override;

			// Get an array of the vertex buffers
			virtual const std::vector<ref<vertexBuffer>>& getVertexBuffers() const override { return m_VertexBuffers; }
			// Get the index buffer
			virtual const ref<indexBuffer>& getIndexBuffer() const override { return m_IndexBuffer; }
		private:
			uint32 m_RendererID;
			uint32 m_VertexBufferIndex = 0;
			std::vector<ref<vertexBuffer>> m_VertexBuffers;	// The vertex buffers
			ref<indexBuffer> m_IndexBuffer;					// The index buffer of how vertices should be read

		};
	}
}