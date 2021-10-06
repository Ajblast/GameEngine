#pragma once

#include "common.h"
#include "event.h"

namespace GRAVEngine
{
	namespace Events
	{
		class GRAVAPI windowResizeEvent : public event
		{
		public:
			windowResizeEvent(unsigned int width, unsigned int height)
				: m_Width(width), m_Height(height) {}

			unsigned int getWidth() const { return m_Width; }
			unsigned int getHeight() const { return m_Height; }

			//std::string ToString() const override
			//{
			//	std::stringstream ss;
			//	ss << "windowResizeEvent: " << m_Width << ", " << m_Height;
			//	return ss.str();
			//}

			EVENT_CLASS_TYPE(windowResize)
			EVENT_CLASS_CATEGORY(eventCategoryApplication)
		private:
			unsigned int m_Width, m_Height;
		};

		class GRAVAPI windowCloseEvent : public event
		{
		public:
			windowCloseEvent() = default;

			EVENT_CLASS_TYPE(windowClose)
			EVENT_CLASS_CATEGORY(eventCategoryApplication)
		};

		class GRAVAPI appTickEvent : public event
		{
		public:
			appTickEvent() = default;

			EVENT_CLASS_TYPE(appTick)
			EVENT_CLASS_CATEGORY(eventCategoryApplication)
		};

		class GRAVAPI appUpdateEvent : public event
		{
		public:
			appUpdateEvent() = default;

			EVENT_CLASS_TYPE(appUpdate)
			EVENT_CLASS_CATEGORY(eventCategoryApplication)
		};

		class GRAVAPI appRenderEvent : public event
		{
		public:
			appRenderEvent() = default;

			EVENT_CLASS_TYPE(appRender)
			EVENT_CLASS_CATEGORY(eventCategoryApplication)
		};
	}
}