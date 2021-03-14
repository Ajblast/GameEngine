#pragma once

#include "rendering/buffers/indexBuffer.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		class openglIndexBuffer : public indexBuffer
		{
		public:
			// Create an index buffer with that has the current indices
			openglIndexBuffer(uint32* indices, uint32 count);

			virtual ~openglIndexBuffer();

			virtual void bind() const override;
			virtual void unbind() const override;

			virtual uint32 getCount() const override { return m_Count; }
		private:
			uint32 m_RendererID;
			uint32 m_Count;

		};
	}
}