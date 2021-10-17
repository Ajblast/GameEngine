#pragma once

#include <thread>
#include "common.h"
#include "jobTypes.h"
#include "tls.h"

namespace GRAVEngine
{
	namespace Jobs
	{
		class GRAVAPI gravThread
		{
		public:
			//typedef std::function<void(gravThread*)> threadCallback;
			using threadCallback = void(*)(gravThread*);

		public:
			gravThread();
			gravThread(const gravThread& other) = delete;	// Copy constructor is not allowed as a single thread object can only exist per thread
			~gravThread();									// Deconstructing thread does not despawn it

			// Spawn a thread into this thread holder
			void spawn(threadCallback callback);
			// Join this thread with the current thread
			void join();
			// Get the handle and id from the currently running thread
			void initializeFromCurrentThread();

			// Set the thread's affinity
			void setAffinity(size_t affinity);
			// Get the thread's affinity
			size_t getAffinity();
			// Set the thread's name
			void setName(const std::wstring& name);
			std::wstring getName() const;

			inline tls* getTLS() { return &m_ThreadLocalStorage; }
			inline threadID getID() const { return m_ThreadID; }
			inline threadHandle getHandle() const { return m_ThreadHandle; }

			// Is the current thread valid
			inline bool isValid() const { return getID() != GRAV_MAX_THREAD_ID; }

			// Cause the thread to sleep for so many milliseconds
			static void sleepFor(uint32 ms);
		private:

		private:
			tls m_ThreadLocalStorage;								// The thread local storage of the thread

			threadID m_ThreadID			= GRAV_MAX_THREAD_ID;		// The thread's ID
			threadHandle m_ThreadHandle	= nullptr;					// The native handle for the thread
		};
	}
}