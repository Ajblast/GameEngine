#pragma once

#include <thread>
#include "common.h"
#include "tls.h"

#define GRAV_MAX_THREAD_ID UINT32_MAX

namespace GRAVEngine
{
	namespace jobs
	{
		typedef uint32 threadID;
		typedef void* threadHandle;

		class gravThread
		{
		public:
			using threadCallbackFunction = void(*)(GRAVEngine::jobs::gravThread*);

		public:
			gravThread();
			gravThread(const gravThread& other) = delete;
			virtual GRAVEngine::jobs::gravThread::~gravThread() = default;			// Deconstructing thread does not despawn it

			void spawn(threadCallbackFunction callback);
			void setAffinity(size_t affinity);

			void join();
			// Get the handle and id from the currently running thread
			void initializeFromCurrentThread();

			inline tls* getTLS() { return &m_ThreadLocalStorage; }
			inline threadID getID() { return m_ThreadID; }
			inline bool isValid() { return getID() != GRAV_MAX_THREAD_ID; }
			inline threadHandle getHandle() { return m_ThreadHandle; }

			static void sleepFor(uint32 ms);
		private:
			//std::thread m_Thread;
			//std::thread::id m_ID;
			tls m_ThreadLocalStorage;

			threadID m_ThreadID			= GRAV_MAX_THREAD_ID;
			threadHandle m_ThreadHandle	= nullptr;
		};
	}
}