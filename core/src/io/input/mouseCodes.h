#pragma once

#include "common.h"
#include <array>

namespace GRAVEngine
{
	namespace Mouse
	{
		typedef uint16 mouseCode;

		// WARNING! Any changes to this enum requires a change in the coresponding  iterable mousecode array

		// Mouse Codes
		enum : mouseCode
		{
			// From glfw3.h
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7,

			ButtonLast = Button7,
			ButtonLeft = Button0,
			ButtonRight = Button1,
			ButtonMiddle = Button2
		};

		// Iterable Mouse Codes
		const std::array<mouseCode, 8> g_MouseCodes = {
			Mouse::Button0,
			Mouse::Button1,
			Mouse::Button2,
			Mouse::Button3,
			Mouse::Button4,
			Mouse::Button5,
			Mouse::Button6,
			Mouse::Button7
		};
	}
}