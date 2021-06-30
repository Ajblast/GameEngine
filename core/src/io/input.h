#pragma once

#include "keyCodes.h"
#include "mouseCodes.h"
#include "types/vecs/vec2.h"

namespace GRAVEngine
{
	namespace IO
	{
		class Input
		{
		public:
			static bool isKeyPressed(Keys::keyCode key);

			static bool isMouseButtonPressed(Mouse::mouseCode button);
			static GRAVEngine::vec2 getMousePosition();
			static float getMouseX();
			static float getMouseY();
		};
	}
}