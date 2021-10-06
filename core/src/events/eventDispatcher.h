#pragma once

#include "common.h"
#include "event.h"
#include <functional>

namespace GRAVEngine
{
	namespace Events
	{
		// Events are currently blocking. Events are immediately dispatched and must be dealt with then and there.
		// For future, a better strategy migh be to buffer events in an event bus and process them during the "event" part of the update stage

		class GRAVAPI eventDispatcher
		{
		public:
			eventDispatcher(event& event) : m_Event(event) {}

			// F is deduced by the compiler
			template<typename T, typename F>
			bool dispatch(const F& function)
			{
				// Only dispatch the event if it matches the specified dispatch
				if (m_Event.getEventType() == T::getStaticType())
				{
					// Dispatch the event with the following function
					m_Event.m_Handled |= function(static_cast<T&>(m_Event));
					return true;
				}

				return false;
			}
		private:
			event& m_Event;
		};
	}
}