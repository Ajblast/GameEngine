#pragma once

#include <thread>
#include "common.h"
#include "tls.h"

namespace GRAVEngine
{
	namespace jobs
	{
		typedef std::thread::id threadID;
		typedef void* threadHandle;

		class gravThread
		{
		public:
			using threadCallbackFunction = void(*)(GRAVEngine::jobs::gravThread*);

		public:
			GRAVEngine::jobs::gravThread();
			GRAVEngine::jobs::gravThread(const gravThread& other) = delete;
			virtual GRAVEngine::jobs::gravThread::~gravThread() = default;			// Deconstructing thread does not despawn it

			void spawn(threadCallbackFunction callback);
			void setAffinity(size_t affinity);

			void join();

			inline tls* getTLS() { return &m_ThreadLocalStorage; }
			inline std::thread::id getID() { return m_Thread.get_id(); }
			inline threadHandle getNativeHandle() { return m_Thread.native_handle(); }

			static void sleepFor(uint32 ms);
		private:
			std::thread m_Thread;
			tls m_ThreadLocalStorage;
		};
	}
}