#pragma once
#include "common.h"

#include "jobManagerOptions.h"
#include "jobPriorities.h"
#include "declaration.h"
#include "counter.h"
#include "gravThread.h"
#include "fiber.h"
#include "tls.h"
#include "jobQueue.h"

#include "locks/spinLock.h"
#include "exceptions/exceptions.h"

// TODO: Implement I/O threads (Socks, file I/O, system call). Slide 19 on Parallelizing the Naughty Dog Engine Using Fibers
// TODO: Implement automatic allocation for counters

#define GRAV_MAIN_THREAD_INDEX 0

namespace GRAVEngine
{
	namespace Jobs
	{
		struct waitingFiberSlot
		{
			fiberIndex m_FiberIndex = UINT16_MAX;
			counterTarget m_TargetValue = 0;
			ref<counter> counter = nullptr;
		};


		class jobManager
		{
			friend counter;

		public:
			using mainMethodFunction = void(*)();

			jobManager();													// Constructor
			jobManager(const jobManager&) = delete;							// Cannot copy the job manager
			jobManager(jobManager&& other) = delete;						// Move constructor

			jobManager& operator= (const jobManager&) = delete;				// Cannot copy the job manager
			jobManager& operator= (jobManager&& other) noexcept = delete;	// Move set operator
			~jobManager();													// Deconstructor

			// Start up the job manager
			void startUp(jobManagerOptions& options);
			void runMain(mainMethodFunction mainMethod);
			void startShutdown();
			// Shut down the job manager
			void shutDown();

			// Kick jobs
			void kickJob(const declaration& declaration);
			void kickJobs(const declaration* declarations, size_t count);
			void kickJobAndWait(const declaration& declaration);
			void kickJobsAndWait(const declaration* declarations, size_t count);

			// Wait for a job to terminate
			void waitForCounter(ref<counter> counter, counterTarget target, bool blocking = true);


			counter* allocCounter();
			void freeCounter(counter* counter);

		public:
			inline const uint8 getNumThreads() const{ return m_ThreadCount; }
			inline const uint16 getNumFibers() const{ return m_FiberCount; }
			inline bool isValid() const { return m_IsValid.load(std::memory_order_acquire); }
			inline bool isShuttingDown() const { return m_IsShuttingDown.load(std::memory_order_acquire); }

			gravThread* getThread(uint8 index);
			threadID getCurrentThreadID() const;

			// Statically get the job instance
			inline static jobManager* getInstance() { return s_Instance; }
		private:
			// Add a fiber to the waiting list
			void addWaitingFiber(ref<counter> counter, fiberIndex index, counterTarget target);
			// Check if there are any fibers that need to be removed from the waiting list and be resumed
			void checkWaitingFibers();
			//void waitForCounterProxy(waitForCounterProxyArgs args);

			// Thread
			threadIndex getCurrentThreadIndex() const;
			gravThread* getCurrentThread() const;
			tls* getCurrentTLS() const;

			// Spawn a worker thread
			void spawnThread(uint8 index);

			// Fiber
			fiberIndex findFreeFiber();
			void cleanPreviousFiber(tls* tls = nullptr);


			// Get a jobQueue by priority. Defaults to low priority queue
			jobQueue& getQueue(jobPriority priority);
			// Get the next job and switch to a fiber
			bool getNextJob(declaration& declaration, tls* tls);

			// Callback Function
			static void fiberCallbackMain(fiber* fiber);
			static void fiberCallback(fiber* fiber);
			static void threadCallback(gravThread* gravThread);
		private:
			// Fiber Waiting List
			uint16 m_MaxWaitingFibersCount;
			std::atomic_bool* m_FreeWaitingFibers;
			waitingFiberSlot* m_WaitingFibers;
			GRAVEngine::Locks::spinLock m_WaitingFiberLock;
			std::vector<fiberIndex> m_ReadyFibers;
			GRAVEngine::Locks::spinLock m_ReadyFiberLock;

			// Threads
			uint8 m_ThreadCount;
			gravThread* m_Threads;

			bool m_ThreadAffinity;
			bool m_AutoInitializeThreads;

			// Fibers
			uint16 m_FiberCount;
			fiber* m_Fibers;
			std::atomic_bool* m_IdleFibers;

			// Job queues
			jobQueue m_CriticalPriorityQueue;
			jobQueue m_HighPriorityQueue;
			jobQueue m_NormalPriorityQueue;
			jobQueue m_LowPriorityQueue;

			// Static Instance
			mainMethodFunction m_MainMethod;
			bool m_ShutdownAfterMain;
			static jobManager* s_Instance;
			std::atomic_bool m_IsValid = false;
			std::atomic_bool m_IsShuttingDown = false;
		};
	}
}