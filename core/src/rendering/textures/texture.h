#pragma once

#include "common.h"
#include <string>

namespace GRAVEngine
{
	namespace Rendering
	{
		class GRAVAPI texture
		{
		public:
			virtual ~texture() = default;

			virtual uint32 getWidth() const = 0;
			virtual uint32 getHeight() const = 0;
			virtual uint32 getRendererID() const = 0;

			virtual void setData(void* data, uint32 size) = 0;

			virtual void bind(uint32 slot = 0) const = 0;

			virtual bool operator==(const texture& other) const = 0;
		};

	}
}