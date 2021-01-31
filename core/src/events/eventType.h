#pragma once

namespace GRAVEngine
{
	namespace Events
	{
		enum class eventType
		{
			none = 0,
			windowClose, windowResize, windowFocus, windowLostFocus, windowMoved,
			appTick, appUpdate, appRender,
			keyPressed, keyReleased, keyTyped,
			mouseButtonPressed, mouseButtonReleased, mouseMoved, mouseScrolled
		};
	}
}