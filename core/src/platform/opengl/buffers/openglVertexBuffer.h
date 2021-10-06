#pragma once

#include "rendering/buffers/vertexBuffer.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		class GRAVAPI openglVertexBuffer : public vertexBuffer
		{
		public:
			// Create a vertex buffer with dynamic data
			openglVertexBuffer(uint32 size);
			// Create a vertex buffer with static data
			openglVertexBuffer(float* vertices, uint32 size);

			virtual ~openglVertexBuffer();

			virtual void bind() const override;
			virtual void unbind() const override;

			virtual void setData(const void* data, uint32 size) override;

			virtual const bufferLayout& getLayout() const override { return m_Layout; }
			virtual void setLayout(const bufferLayout& layout) override { m_Layout = layout; }
		private:
			uint32 m_RendererID;
			bufferLayout m_Layout;
		};
	}
}