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

// TODO: Implement job manager and fiber based job system
// TODO: Implement I/O threads (Socks, file I/O, system call). Slide 19 on Parallelizing the Naughty Dog Engine Using Fibers

namespace GRAVEngine
{
	namespace jobs
	{
		struct waitingFiberSlot
		{
			fiberIndex m_FiberIndex = UINT16_MAX;
			counterTarget m_TargetValue = 0;
		};


		class jobManager
		{
			friend counter;
			using mainMethodFunction = void(*)(jobManager*);

		public:
			jobManager();													// Constructor
			jobManager(const jobManager&) = delete;							// Cannot copy the job manager
			jobManager(jobManager&& other) = delete;						// Move constructor

			jobManager& operator= (const jobManager&) = delete;				// Cannot copy the job manager
			jobManager& operator= (jobManager&& other) noexcept = delete;	// Move set operator
			~jobManager();													// Deconstructor

			// Start up the job manager
			void startUp(jobManagerOptions& options/*, mainMethodFunction mainMethod*/);
			// Shut down the job manager
			void shutDown();


			// Kick jobs
			void kickJob(const declaration& declaration);
			void kickJobs(const declaration* declarations, size_t count);
			void kickJobAndWait(const declaration& declaration);
			void kickJobsAndWait(const declaration* declarations, size_t count);

			// Wait for a job to terminate
			void waitForCounter(counter* counter, counterTarget target, bool blocking = true);


			counter* allocCounter();
			void freeCounter(counter* counter);

		public:
			inline const uint8 getNumThreads() const{ return m_ThreadCount; }
			inline const uint16 getNumFibers() const{ return m_FiberCount; }
			inline bool isValid() const { return m_IsValid.load(std::memory_order_acquire); }

			gravThread* getThread(uint8 index);

			// Spawn a worker thread
			void spawnThread(uint8 index);

			// Statically get the job instance
			inline static jobManager* getInstance()
			{
				return s_Instance;
			}
		private:
			// Fiber Waiting list

			// Add a fiber to the waiting list
			void addWaitingFiber(fiberIndex index, counterTarget target);
			// Check if there are any fibers that need to be removed from the waiting list and be resumed
			void checkWaitingFibers(counterTarget target);
			//void waitForCounterProxy(waitForCounterProxyArgs args);

			// Thread
			threadIndex getCurrentThreadIndex() const;
			gravThread* getCurrentThread() const;
			tls* getCurrentTLS() const;

			// Fiber
			fiberIndex findFreeFiber();
			void cleanPreviousFiber(tls* tls = nullptr);


			// Get a jobQueue by priority. Defaults to low priority queue
			jobQueue& getQueue(jobPriority priority);
			// Get the next job and switch to a fiber
			bool getNextJob(declaration& declaration, tls* tls);

			// Callback Function
			//static void fiberCallbackMain(fiber* fiber);
			static void fiberCallback(fiber* fiber);
			static void threadCallback(gravThread* gravThread);
		private:
			// Fiber Waiting List
			uint16 m_MaxWaitingFibersCount;
			std::atomic_bool* m_FreeWaitingFibers;
			waitingFiberSlot* m_WaitingFibers;
			GRAVEngine::Locks::spinLock m_WaitingFiberLock;
			std::vector<fiberIndex> m_ReadyFibers;


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
			jobQueue* m_CriticalPriorityQueue;
			jobQueue* m_HighPriorityQueue;
			jobQueue* m_NormalPriorityQueue;
			jobQueue* m_LowPriorityQueue;

			// Static Instance
			static jobManager* s_Instance;
			std::atomic_bool m_IsValid = false;
		};


		//// Example basic job system
		//GRAVEngine::Locks::spinLock jobLock;
		//void* jobWorkerThread(void*)
		//{
		//
		//	// Keep on running jobs forever...
		//	while (true)
		//	{
		//		declaration declarationCopy;
		//
		//		// Wait for a job to become available
		//		jobLock.acquire();
		//		
		//		// Copy the job declaration locally and release ou rmutex lock
		//		declarationCopy = getNextJobFromQueue();
		//
		//		// Release the lock
		//		jobLock.release();
		//
		//		// Run the job
		//		declarationCopy.m_EntryPoint(declarationCopy.m_param);
		//
		//		// Job Done.
		//	}
		//}
	}
}