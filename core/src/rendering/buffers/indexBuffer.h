#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		// Buffer to store indexes. Only 32-bit max number of 32-bit indexes are supported.
		// Tells which vertexes should be rendered in a vertex buffer
		class GRAVAPI indexBuffer
		{
		public:
			virtual ~indexBuffer() = default;

			// Bind this index buffer
			virtual void bind() const = 0;
			// Unbind this index buffer. Don't unbind before unbinding vertex array
			virtual void unbind() const = 0;

			// Get the number of indices
			virtual uint32 getCount() const = 0;
		};
	}
}