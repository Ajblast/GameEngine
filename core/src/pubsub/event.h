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
				GRAV_PROFILE_FUNCTION();

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
				GRAV_PROFILE_FUNCTION();

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
				GRAV_PROFILE_FUNCTION();

				Locks::scopedLock<decltype(m_VectorLock)> lock(m_VectorLock);

				size_t eventCount = m_Events.size();

				// Don't do anything if there are no events
				if (eventCount == 0)
					return;

				// Create an array for the events and a place for the counter
				scope<Jobs::declaration[]> jobs = createScope<Jobs::declaration[]>(eventCount);
				ref<Jobs::counter> counter;

				// Create a job per event
				for (size_t i = 0; i < eventCount; i++)
					jobs[i] = Jobs::declaration([&](uintptr_t) {m_Events[i].m_CallbackFunction(args...); }, 0, Jobs::jobPriority::NORMAL);

				// Kick the jobs and wait for them
				GRAV_KICK_JOBS(jobs.get(), eventCount, &counter);
				GRAV_WAIT_COUNTER(counter, 0);
			}

			// Reset the event
			void clear()
			{
				GRAV_PROFILE_FUNCTION();

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