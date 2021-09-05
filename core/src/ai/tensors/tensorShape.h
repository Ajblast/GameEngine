#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Tensors
		{
			// Shape of a tensor.
			// In BHWC romat
			// TODO: Change to BCHW format
			struct tensorShape
			{
			public:
				tensorShape(size_t n);
				tensorShape(size_t n, size_t c);
				tensorShape(size_t n, size_t w, size_t c);
				tensorShape(size_t n, size_t h, size_t w, size_t c);

				inline const size_t flatHeight() const { return m_Batch; }
				inline const size_t flatWidth() const { return m_Height * m_Width * m_Channels; }
				inline const size_t length() const { return flatHeight() * flatWidth(); }
				inline const size_t rank() const { return 4; }
				inline const size_t dimensions() const {
					return
						m_Batch > 1 ? 1 : 0 +
						m_Height > 1 ? 1 : 0 +
						m_Width > 1 ? 1 : 0 +
						m_Channels > 1 ? 1 : 0;
				}

				size_t operator[](size_t axis);

				// Get the index at the point
				size_t index(size_t n, size_t h, size_t w, size_t c);
				// Get the index at the point
				size_t index(size_t n, size_t c);

				// Flatten shape into N, C * H * W
				tensorShape flatten();

			public:
				size_t m_Batch;
				size_t m_Height;
				size_t m_Width;
				size_t m_Channels;
			};

			inline bool operator== (const tensorShape& lhs, const tensorShape& rhs)
			{
				return lhs.m_Batch == rhs.m_Batch && lhs.m_Channels == rhs.m_Channels && lhs.m_Height == rhs.m_Height && lhs.m_Width == rhs.m_Width;
			}
			inline bool operator!= (const tensorShape& lhs, const tensorShape& rhs)
			{
				return !(lhs == rhs);
			}
		}
	}
}