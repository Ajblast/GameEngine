#pragma once

#include "common.h"
#include "event.h"
#include "keyCodes.h"

namespace GRAVEngine
{
	namespace Events
	{
		class keyEvent : public event
		{
		public:
			Keys::keyCode getKeyCode() const { return m_KeyCode; }

			EVENT_CLASS_CATEGORY(eventCategoryKeyboard, eventCategoryInput)
		protected:
			keyEvent(const Keys::keyCode keyCode) : m_KeyCode(keyCode) {}

			Keys::keyCode m_KeyCode;
		};

		class keyPressedEvent : public keyEvent
		{
		public:
			keyPressedEvent(const Keys::keyCode keycode, const uint16 repeatCount)
				: keyEvent(keycode), m_RepeatCount(repeatCount) {}

			uint16 GetRepeatCount() const { return m_RepeatCount; }

			//std::string toString() const override
			//{
			//	std::stringstream ss;
			//	ss << "keyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			//	return ss.str();
			//}
		public:
			EVENT_CLASS_TYPE(keyPressed)
		
		private:
			uint16 m_RepeatCount;
		};

		class keyReleasedEvent : public keyEvent
		{
		public:
			keyReleasedEvent(const Keys::keyCode keycode)
				: keyEvent(keycode) {}

			//std::string toString() const override
			//{
			//	std::stringstream ss;
			//	ss << "KeyReleasedEvent: " << m_KeyCode;
			//	return ss.str();
			//}

			EVENT_CLASS_TYPE(keyReleased)
		};

		class KeyTypedEvent : public keyEvent
		{
		public:
			KeyTypedEvent(const Keys::keyCode keycode)
				: keyEvent(keycode) {}

			//std::string toString() const override
			//{
			//	std::stringstream ss;
			//	ss << "KeyTypedEvent: " << m_KeyCode;
			//	return ss.str();
			//}

			EVENT_CLASS_TYPE(keyTyped)
		};
	}
}