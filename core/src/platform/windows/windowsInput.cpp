#include "gravpch.h"

#include "io/input.h"
#include "application.h"
#include <GLFW/glfw3.h>

namespace GRAVEngine
{
	namespace IO
	{
		bool Input::isKeyPressed(const Keys::keyCode key)
		{
			auto* window = static_cast<GLFWwindow*>(application::getInstance().getWindow().getNativeWindow());
			auto state = glfwGetKey(window, static_cast<int32>(key));

			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}

		bool Input::isMouseButtonPressed(Mouse::mouseCode button)
		{
			auto* window = static_cast<GLFWwindow*>(application::getInstance().getWindow().getNativeWindow());
			auto state = glfwGetMouseButton(window, static_cast<int32>(button));

			return state == GLFW_PRESS;
		}
		std::pair<float, float> Input::getMousePosition()
		{
			auto* window = static_cast<GLFWwindow*>(application::getInstance().getWindow().getNativeWindow());
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			return { (float)xpos, (float)ypos };
		}
		float Input::getMouseX()
		{
			return getMousePosition().first;
		}
		float Input::getMouseY()
		{
			return getMousePosition().second;
		}
	}
}