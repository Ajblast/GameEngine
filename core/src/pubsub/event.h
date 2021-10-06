#pragma once

#include "common.h"
#include "locks/scopedLock.h"
#include "locks/spinLock.h"
#include "jobs/jobs.h"
#include <functional>
#include <vector>

namespace GRAVEngine
{
	namespace PubSub
	{
		// NOTE: This is not thread safe at all.

		// An event's handle
		typedef size_t eventHandle;

		//// Storage used for an event
		template<typename...  T>
		struct eventStorage
		{
			std::function<void(T...)> m_CallbackFunction;
			eventHandle m_Handle;
		};


		// An event that can be used to store multiple callbacks to functions.
		template<typename... T>
		class GRAVAPI event
		{
			typedef std::function<void(T...)> callback;


		public:
			// Register a callback with this event. Gives back the handle for the event
			eventHandle registerCallback(const callback& function)
			{
				Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

				// Get the new handle
				eventHandle handle = m_CurrentHandleValue++;

				// Add the function to the list of events with the handle for it
				m_Events.push_back({ function, handle});

				return handle;
			}

			// Unregister a callback based on the handle
			void unregisterCallback(eventHandle handle)
			{
				Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

				// Find the event by the handle value
				auto it = std::find_if(m_Events.begin(), m_Events.end(), [handle](eventStorage<T...> val) { return val.m_Handle == handle; });

				if (it == m_Events.end())
					return;

				// Erase the callback
				m_Events.erase(it);
			}

			// Execute the events
			void execute(T... args)
			{
				Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

				ref<Jobs::counter> counter = createRef<Jobs::counter>();

				for (auto it = m_Events.begin(); it != m_Events.end(); it++)
				{
					Jobs::declaration decl = Jobs::declaration([it](uintptr_t) {it->m_CallbackFunction(args...); }, 0, Jobs::jobPriority::NORMAL, counter);

					GRAV_KICK_JOB(decl);

					//it->m_CallbackFunction(args...);
				}

				// Wait for all of the jobs
				GRAV_WAIT_COUNTER(counter, 0);
			}

			// Reset the event
			void clear()
			{
				Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

				m_Events.clear();
				m_CurrentHandleValue = 0;
			}
		private:
			std::vector<eventStorage<T...>> m_Events;	// Event storage
			eventHandle m_CurrentHandleValue = 0;		// The current handle value to be used. Not random and can run out. Unlikely but not impossible.

			Locks::spinLock m_VectorLock;
		};


	}
}