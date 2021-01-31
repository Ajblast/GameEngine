#pragma once

#include "common.h"
#include "eventType.h"
#include "eventCategory.h"

namespace GRAVEngine
{
	namespace Events
	{
#define EVENT_CLASS_TYPE(type) static eventType getStaticType() { return eventType::type; }\
								virtual eventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override {return category;}

		class event
		{
		public:
			virtual ~event() = default;

			bool m_Handled = false;

			virtual eventType getEventType() const = 0;
			virtual const char* getName() const = 0;
			virtual int32 getCategoryFlags() const = 0;
			virtual std::string toSTring() const { return getName(); }

			bool isInCategory(eventCategory category) { return getCategoryFlags() & category; }
		};
	}
}