#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		class GRAVAPI graphicsContext
		{
		public:
			// Startup a context
			virtual void startup() = 0;
			// Swap the current buffers
			virtual void swapBuffers() = 0;
		};
	}
}