#pragma once
#include "common.h"

#include "jobManagerOptions.h"
#include "jobPriorities.h"
#include "declaration.h"
#include "counter.h"
#include "gravThread.h"
#include "fiber.h"
#include "tls.h"
#include "containers/threadSafe/queue.h"

#include "locks/spinLock.h"
#include "exceptions/exceptions.h"

// TODO: Implement I/O threads (Socks, file I/O, system call). Slide 19 on Parallelizing the Naughty Dog Engine Using Fibers
// TODO: Implement automatic allocation for counters

#define GRAV_MAIN_THREAD_INDEX 0
#define GRAV_MAIN_FIBER_INDEX 0

namespace GRAVEngine
{
	namespace Jobs
	{
		GRAV_JOB_ENTRY_POINT(switchToReadyFiber);

		struct job
		{
			declaration m_Declaration;
			ref<counter>* m_Counter;
		};

		typedef queue<job, Locks::spinLock> jobQueue;

		class GRAVAPI jobManager
		{
			// The job manager is a friend of the counter so that waiting fibers can be added easierly and without complicating public API
			friend counter;
			// Switch to ready fiber is a friend because it needs access to the fibers and the tls
			friend GRAV_JOB_ENTRY_POINT(switchToReadyFiber);

		public:
			jobManager();													// Constructor
			jobManager(const jobManager&) = delete;							// Cannot copy the job manager
			jobManager(jobManager&& other) = delete;						// Move constructor
			jobManager& operator= (const jobManager&) = delete;				// Cannot copy the job manager
			jobManager& operator= (jobManager&& other) noexcept = delete;	// Move set operator
			~jobManager();													// Deconstructor

			// Start up the job manager
			void startUp(jobManagerOptions& options);
			// Shut down the job manager
			void shutDown();

			// Kick a single job
			void kickJob(const declaration& declaration);
			// Kick a single job and return a counter
			void kickJob(const declaration& declaration, ref<counter>* counter);
			// Kick an array of jobs. Jobs can be created on the stack as long as the array is not freed before all the jobs are ran
			void kickJobs(const declaration* declarations, size_t count);
			// Kick an array of jobs. Jobs can be created on the stack as long as the array is not freed before all the jobs are ran
			void kickJobs(const declaration* declarations, size_t count, ref<counter>* counter);
			// Wait for the atomic counter to be the target value before allowing execution to continue
			void waitForCounter(ref<counter>& counter, counterTarget target);

		public:
			inline const uint8 getNumThreads() const{ return m_ThreadCount; }
			inline const uint16 getNumFibers() const{ return m_FiberCount; }
			inline const bool isValid() const { return m_IsValid.load(std::memory_order_acquire); }
			inline const bool isShuttingDown() const { return m_ShuttingDown.load(std::memory_order_acquire); }

			// Get the current thraed id
			threadID getCurrentThreadID() const;
			// Get the current thread index
			threadIndex getCurrentThreadIndex() const;
			// Get the current thread name
			std::wstring getCurrentThreadName() const;

			// Statically get the job instance
			inline static jobManager* getInstance() { return s_Instance; }
		private:
			void addReadyFiber(fiberBundle* bundle);
		private:

			// Get a thread based on its index
			gravThread* getThread(threadIndex index) const;
			// Get the current thread
			gravThread* getCurrentThread() const;
			// Get the current tls
			tls* getCurrentTLS() const;

			// Find a free fiber
			fiberIndex findFreeFiber();
			// Clean the previous fiber
			void cleanPreviousFiber();


			// Get a jobQueue by priority. Defaults to low priority queue
			jobQueue& getQueue(jobPriority priority);
			// Get the next job
			bool getNextJob(job& job);

			// Callback Function
			static void fiberCallback(fiber* fiber);
			static void fiberQuitCallback(fiber* fiber);
			static void threadCallback(gravThread* gravThread);
		private:
			// Array to hold the currently ready fibers
			scope<fiberBundle[]> m_ReadyFibers;
			GRAVEngine::Locks::spinLock m_ReadyFiberLock;

			// Threads
			uint8 m_ThreadCount;						// The number of threads
			scope<gravThread[]> m_Threads;				// The array of threads
			bool m_ThreadAffinity;						// Should threads be set with a thread affinity

			// Thead sleeping behavior
			std::mutex m_SleepingThreadLock;			// Lock used for when threads sleep instead of spin locking
			std::condition_variable m_ThreadCV;			// Condition variable to wake up worker threads when they sleep instead of spin locking
			std::atomic<queueBehavior> m_QueueBehavior;	// What is the thread sleeping behavior when there are no jobs

			// Fibers
			fiberIndex m_FiberCount;					// The number of fibers
			scope<fiber[]> m_Fibers;					// The array of fibers
			scope<std::atomic_bool[]> m_IdleFibers;		// Array holding a bool that determines if a fiber is available

			// Thread shutdown infomation
			scope<fiber[]> m_ThreadQuitFibers;			// The fibers that are explicitly used to make sure that threads switch back to their personal fibers
			std::atomic_size_t m_ShutdownThreadCount;	// The amount of threads that have shutdown and finished

			// Job queues
			jobQueue m_CriticalPriorityQueue;		// The critical priority job queue
			jobQueue m_HighPriorityQueue;			// The high priority job queue
			jobQueue m_NormalPriorityQueue;			// The normal priority job queue
			jobQueue m_LowPriorityQueue;			// The low priority job queue

			jobManagerCallbacks m_Callbacks;			// Callbacks for the job manager

			// Static Instance
			static jobManager* s_Instance;				// The static job instance
			std::atomic_bool m_IsValid = false;			// Is the job manager valid?
			std::atomic_bool m_ShuttingDown = false;	// Is the job manager valid?
		};
	}
}