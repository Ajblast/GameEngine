#pragma once

#include "keyCodes.h"
#include "mouseCodes.h"

namespace GRAVEngine
{
	namespace IO
	{
		class Input
		{
		public:
			static bool isKeyPressed(Keys::keyCode key);

			static bool isMouseButtonPressed(Mouse::mouseCode button);
			static std::pair<float, float> getMousePosition();
			static float getMouseX();
			static float getMouseY();
		};
	}
}