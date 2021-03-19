#include "gravpch.h"
#include "windowsWindow.h"

#include "events/applicationEvents.h"
#include "events/mouseEvents.h"
#include "events/keyEvents.h"

#include "rendering/renderer/rendererAPI.h"
#include "platform/opengl/openglContext.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		static uint8 s_GLFWWindowCount = 0;

		static void glfwErrorCallback(int error, const char* description)
		{
			GRAV_LOG_LINE_ERROR("GLFW Error (%d: %s)", error, description);
		}

		windowsWindow::windowsWindow(const windowProperties& properties)
		{
			GRAV_PROFILE_FUNCTION();

			startup(properties);
		}
		windowsWindow::~windowsWindow()
		{
			GRAV_PROFILE_FUNCTION();

			shutdown();
		}

		void windowsWindow::onUpdate()
		{
			GRAV_PROFILE_FUNCTION();

			// Poll for events and swap the buffers
			glfwPollEvents();
			m_Context->swapBuffers();
		}

		void windowsWindow::setVSync(bool enabled)
		{
			GRAV_PROFILE_FUNCTION();

			if (enabled)
				glfwSwapInterval(1);
			else
				glfwSwapInterval(0);

			m_Data.m_VSync = enabled;
		}
		bool windowsWindow::isVSync() const
		{
			return m_Data.m_VSync;
		}

		void windowsWindow::startup(const windowProperties& properties)
		{
			GRAV_PROFILE_FUNCTION();

			m_Data.m_Title = properties.m_Title;
			m_Data.m_Width = properties.m_Width;
			m_Data.m_Height = properties.m_Height;

			GRAV_LOG_LINE_INFO("Creating window \"%s\" (%d, %d)", properties.m_Title.c_str(), properties.m_Width, properties.m_Height);

			// Initialize glfw if needed
			if (s_GLFWWindowCount == 0)
			{
				// TODO: glfwTerminate on system shutdown
				int success = glfwInit();
				GRAV_ASSERT(success);

				// Set the callback function for when glfw has an error
				glfwSetErrorCallback(glfwErrorCallback);

				GRAV_LOG_LINE_INFO("%s: GLFW Version %s", __FUNCTION__, glfwGetVersionString());


				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GRAV_GLFW_CONTEXT_VERSION_MAJOR);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GRAV_GLFW_CONTEXT_VERSION_MINOR);
			}

#ifdef GRAV_DEBUG
			if (Rendering::rendererAPI::getAPI() == Rendering::rendererAPI::API::OpenGL)
			{
				glfwWindowHint(GLFW_OPENGL_PROFILE, GRAV_GLFW_OPENGL_PROFILE);
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			}
#endif

			{
				GRAV_PROFILE_SCOPE("glfwCreateWindow");

				// Create the window
				m_Window = glfwCreateWindow((int)properties.m_Width, (int)properties.m_Height, m_Data.m_Title.c_str(), nullptr, nullptr);
				if (m_Window == nullptr)
				{
					GRAV_LOG_LINE_ERROR("Error in creating window (%s)", m_Data.m_Title.c_str());
					return;
				}

				// Increment the window count
				++s_GLFWWindowCount;
			}

			// Create the context of the window
			m_Context = Rendering::rendererAPI::createContext(m_Window);
			m_Context->startup();


			// Set data associated with this window
			glfwSetWindowUserPointer(m_Window, &m_Data);
			setVSync(true);


			// TODO: Callbacks

			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int32 width, int32 height)
			{
					windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
					data.m_Width = width;
					data.m_Height = height;

					Events::windowResizeEvent event(width, height);
					data.m_EventCallback(event);
			});
			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
					windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

					Events::windowCloseEvent event;
					data.m_EventCallback(event);
			});
			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods)
			{
					windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
						case GLFW_PRESS:
						{
							Events::keyPressedEvent event(key, 0);
							data.m_EventCallback(event);
							break;
						}
						case GLFW_RELEASE:
						{
							Events::keyReleasedEvent event(key);
							data.m_EventCallback(event);
							break;
						}
						case GLFW_REPEAT:
						{
							Events::keyPressedEvent event(key, 1);
							data.m_EventCallback(event);
							break;
						}
					}
			});
			glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32 keycode)
			{
					windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

					Events::KeyTypedEvent event(keycode);
					data.m_EventCallback(event);
			});
			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int32 button, int32 action, int32 mods)
			{
					windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
						case GLFW_PRESS:
						{
							Events::mouseButtonPressedEvent event(button);
							data.m_EventCallback(event);
							break;
						}
						case GLFW_RELEASE:
						{
							Events::mouseButtonReleasedEvent event(button);
							data.m_EventCallback(event);
							break;
						}
					}
			});
			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
					windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

					Events::mouseScrolledEvent event((float) xOffset, (float) yOffset);
					data.m_EventCallback(event);
			});
			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
					windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

					Events::mouseMovedEvent event((float)xPos, (float)yPos);
					data.m_EventCallback(event);
			});

		}
		void windowsWindow::shutdown()
		{
			GRAV_PROFILE_FUNCTION();

			// Destroy the window decrement the count
			glfwDestroyWindow(m_Window);
			--s_GLFWWindowCount;

			// Terminate glfw if there are no more windows
			if (s_GLFWWindowCount == 0)
				glfwTerminate();
		}
	}
}