#include "gravpch.h"

#include "io/input/input.h"
#include "io/input/inputManager.h"
#include "application.h"
#include <GLFW/glfw3.h>

namespace GRAVEngine
{
	namespace IO
	{
		bool Input::isKeyPressed(const Keys::keyCode key)
		{
			return inputManager::getInstance()->isKeyPressed(key);
		}

		bool Input::isMouseButtonPressed(Mouse::mouseCode button)
		{
			return inputManager::getInstance()->isMouseButtonPressed(button);
		}
		vec2 Input::getMousePosition()
		{
			return inputManager::getInstance()->getMousePosition();
		}
		float Input::getMouseX()
		{
			return inputManager::getInstance()->getMouseX();
		}
		float Input::getMouseY()
		{
			return inputManager::getInstance()->getMouseY();
		}
	}
}