#pragma once

#include "common.h"
#include "rendering/graphicsContext.h"

struct GLFWwindow;

namespace GRAVEngine
{
	namespace Rendering
	{
		class GRAVAPI openglContext : public graphicsContext
		{
		public:
			openglContext(GLFWwindow* windowHandle);

			virtual void startup() override;
			virtual void swapBuffers() override;

			virtual void bind() override;
			virtual void unbind() override;
		private:
			GLFWwindow* m_WindowHandle;

		};
	}
}