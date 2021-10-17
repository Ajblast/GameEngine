#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		class GRAVAPI graphicsContext
		{
		public:
			// Startup a context.
			virtual void startup() = 0;
			// Swap the current buffers
			virtual void swapBuffers() = 0;

			// Bind this graphical context
			virtual void bind() = 0;
			// Unbind this graphical context
			virtual void unbind() = 0;
		};
	}
}