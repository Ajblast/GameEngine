#pragma once

#include "common.h"
#include "rendering/windows/windowProperties.h"
#include "rendering/windows/window.h"
#include "rendering/graphicsContext.h"

#include <GLFW/glfw3.h>
#define GRAV_GLFW_CONTEXT_VERSION_MAJOR 4
#define GRAV_GLFW_CONTEXT_VERSION_MINOR 6
#define GRAV_GLFW_OPENGL_PROFILE GLFW_OPENGL_CORE_PROFILE

namespace GRAVEngine
{
	namespace Rendering
	{
		class GRAVAPI windowsWindow : public window
		{
		public:
			windowsWindow(const windowProperties& properties);
			windowsWindow(scope<windowsWindow>& window, const windowProperties& properties);
			virtual ~windowsWindow();

			void onUpdate() override;

			// Window attributes
			inline uint32 getWidth() const override { return m_Data.m_Width; }
			inline uint32 getHeight() const override { return m_Data.m_Height; }

			void setEventCallback(const eventCallbackFunction& callback) override { m_Data.m_EventCallback = callback; }
			void setVSync(bool enabled) override;
			bool isVSync() const override;

			inline void* getNativeWindow() const { return m_Window; }
		private:
			virtual void startup(const windowProperties& properties);
			virtual void startup(scope<windowsWindow>& window, const windowProperties& properties);
			virtual void shutdown();
		private:
			struct windowData
			{
				std::string m_Title;
				uint32 m_Width, m_Height;
				bool m_VSync;

				eventCallbackFunction m_EventCallback;
			};

			GLFWwindow* m_Window;
			windowData m_Data;
		};
	}
}