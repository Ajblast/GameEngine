#include "gravpch.h"
#include "jobManager.h"

// Static initial definition of instance
GRAVEngine::Jobs::jobManager* GRAVEngine::Jobs::jobManager::s_Instance = nullptr;

GRAVEngine::Jobs::jobManager::jobManager()
{
	// Threads
	m_ThreadCount = 0;
	m_ThreadAffinity = 0;
	m_AutoInitializeThreads = false;

	// Fiber count
	m_FiberCount = 0;

	// Waiting Fibers
	m_MaxWaitingFibersCount = 0;
	m_WaitingFibers = nullptr;
	m_FreeWaitingFibers = nullptr;

	
	// Create the threads
	m_Threads = nullptr;
	m_Fibers = nullptr;
	m_IdleFibers = nullptr;
	m_MainMethod = nullptr;
	m_ShutdownAfterMain = true;
}
GRAVEngine::Jobs::jobManager::~jobManager() {}

void GRAVEngine::Jobs::jobManager::startUp(jobManagerOptions& options)
{
	GRAV_ASSERT(s_Instance == nullptr);
	GRAV_ASSERT(options.m_NumFibers > 0);
	GRAV_ASSERT(options.m_NumThreads > 0);
	if (options.m_ThreadAffinity)
		GRAV_ASSERT(options.m_NumThreads <= std::thread::hardware_concurrency());
	GRAV_ASSERT(options.m_MaxWaitingFibersCount > 1);

	GRAV_LOG_LINE_INFO("%s: Begin Startup", __FUNCTION__);

	s_Instance = this;

	m_ShutdownAfterMain = options.m_ShutDownAfterMain;

	// Threads
	m_ThreadCount = options.m_NumThreads;
	m_ThreadAffinity = options.m_ThreadAffinity;
	m_AutoInitializeThreads = options.m_AutoInitializeThreads;

	// Fiber count
	m_FiberCount = options.m_NumFibers;

	// Waiting Fibers
	GRAV_LOG_LINE_INFO("%s: Create waiting fiber list", __FUNCTION__);
	m_MaxWaitingFibersCount = options.m_MaxWaitingFibersCount;
	m_WaitingFibers			= new waitingFiberSlot[m_MaxWaitingFibersCount];
	m_FreeWaitingFibers		= new std::atomic_bool[m_MaxWaitingFibersCount];

	// All the slots start off free
	for (size_t i = 0; i < m_MaxWaitingFibersCount; i++)
		m_FreeWaitingFibers[i].store(true);

	// Job queues
	m_CriticalPriorityQueue	= jobQueue(options.m_CriticalPriorityQueueSize);
	m_HighPriorityQueue		= jobQueue(options.m_HighPriorityQueueSize);
	m_NormalPriorityQueue	= jobQueue(options.m_NormalPriorityQueueSize);
	m_LowPriorityQueue		= jobQueue(options.m_LowPriorityQueueSize);


	// Create the threads
	GRAV_LOG_LINE_INFO("%s: Create Threads", __FUNCTION__);
	m_Threads = new gravThread[m_ThreadCount];

	// Initialize the main thread
	// Load up the information about the main thread
	gravThread* mainThread = &m_Threads[GRAV_MAIN_THREAD_INDEX];
	mainThread->initializeFromCurrentThread();
	if (m_ThreadAffinity)
		mainThread->setAffinity(GRAV_MAIN_THREAD_INDEX);

	// Get the main thread's thread local storage and convert this thread into a fiber
	GRAV_LOG_LINE_INFO("%s: Convert the main thread into a fiber", __FUNCTION__);
	tls* mainThreadTLS = mainThread->getTLS();
	mainThreadTLS->m_Fiber.initializeFromCurrentThread();

	// ********** This thread is a fiber from this point forward


	// Create the fibers. Must be done after the main thread has been converted into a fiber
	GRAV_LOG_LINE_INFO("%s: Create Fibers", __FUNCTION__);
	m_Fibers = new fiber[m_FiberCount];
	m_IdleFibers = new std::atomic_bool[m_FiberCount];

	// Set the fiber callbacks
	GRAV_LOG_LINE_INFO("%s: Set fiber callbacks", __FUNCTION__);
	for (size_t i = 0; i < m_FiberCount; i++)
	{
		m_Fibers[i].setCallback(fiberCallback);
		m_IdleFibers[i].store(true, std::memory_order_release);
	}

	// Spawn the threads
	GRAV_LOG_LINE_INFO("%s: Spawn Threads", __FUNCTION__);
	for (threadIndex i = 0; i < m_ThreadCount; i++)
	{
		// set thread index
		auto tls = m_Threads[i].getTLS();
		tls->m_ThreadIndex = i;

		tls->m_HasAffinity = m_ThreadAffinity;

		// Only spawn threads for not the main thread
		if (i != GRAV_MAIN_THREAD_INDEX && m_AutoInitializeThreads)
			spawnThread(i);
	}

	m_IsValid.store(true, std::memory_order_relaxed);
	GRAV_LOG_LINE_INFO("%s: End Startup", __FUNCTION__);
}
void GRAVEngine::Jobs::jobManager::runMain(mainMethodFunction mainMethod)
{
	GRAV_ASSERT(mainMethod);

	GRAV_LOG_LINE_INFO("%s: Begin Main Method", __FUNCTION__);
	
	// Set the main method
	m_MainMethod = mainMethod;

	// Get the thread local storage
	tls* tls = getThread(GRAV_MAIN_THREAD_INDEX)->getTLS();

	// Find a free fiber to run the main method code
	tls->m_CurrentFiberIndex = findFreeFiber();
	const auto mainFiber = &m_Fibers[tls->m_CurrentFiberIndex];
	mainFiber->setCallback(fiberCallbackMain);

	// Execute the job system
	GRAV_LOG_LINE_INFO("%s: Execute Main Method", __FUNCTION__);
	tls->m_Fiber.switchTo(mainFiber);
	// This is the end of the job system after return to here from running main

	// Wait for all threads to shut down
	GRAV_LOG_LINE_INFO("%s: Wait for worker threads to join", __FUNCTION__);
	for (size_t i = 1; i < m_ThreadCount; i++)
		if(i != GRAV_MAIN_THREAD_INDEX)
			m_Threads[i].join();
	
	GRAV_LOG_LINE_INFO("%s: End Main Method", __FUNCTION__);
}
void GRAVEngine::Jobs::jobManager::startShutdown()
{
	GRAV_ASSERT(s_Instance);

	GRAV_LOG_LINE_INFO("%s: Flag Shutting Down", __FUNCTION__);
	
	// The job manager is now shutting down is invalid
	m_IsValid.store(false, std::memory_order_release);
	m_IsShuttingDown.store(true, std::memory_order_release);

}
void GRAVEngine::Jobs::jobManager::shutDown()
{
	GRAV_ASSERT(s_Instance);
	GRAV_ASSERT(isShuttingDown());

	// TODO: Properly shutdown the job manager. Thread and fiber shut down and memory management

	GRAV_LOG_LINE_INFO("%s: Begin Shutdown", __FUNCTION__);

	// Wait for all of the threads to complete now that the job manager is invalid
	GRAV_LOG_LINE_INFO("%s: Wait for worker threads to join", __FUNCTION__);
	for (size_t i = 0; i < m_ThreadCount; i++)
		if (i != GRAV_MAIN_THREAD_INDEX)
			m_Threads[i].join();

	// Convert the main thread fiber back into a thread
	GRAV_LOG_LINE_INFO("%s: Convert the main fiber back into a thread", __FUNCTION__);
	m_Threads[GRAV_MAIN_THREAD_INDEX].getTLS()->m_Fiber.convertToThread();

	GRAV_LOG_LINE_INFO("%s: Delete Waiting Fibers", __FUNCTION__);
	delete[] m_WaitingFibers;
	GRAV_LOG_LINE_INFO("%s: Delete Free Waiting Fibers", __FUNCTION__);
	delete[] m_FreeWaitingFibers;
	GRAV_LOG_LINE_INFO("%s: Delete Threads", __FUNCTION__);
	delete[] m_Threads;
	GRAV_LOG_LINE_INFO("%s: Delete Fibers", __FUNCTION__);
	delete[] m_Fibers;
	GRAV_LOG_LINE_INFO("%s: Delete Idle Fibers", __FUNCTION__);
	delete[] m_IdleFibers;

	s_Instance = nullptr;
	GRAV_LOG_LINE_INFO("%s: End Shutdown", __FUNCTION__);
}

void GRAVEngine::Jobs::jobManager::kickJob(const declaration& declaration)
{
	GRAV_ASSERT(declaration.m_EntryPoint);

	GRAV_LOG_LINE_INFO("%s: Kick Job | EntryPoint %ul | Priority %s", __FUNCTION__, declaration.m_EntryPoint, toString(declaration.m_Priority));

	jobQueue& queue = getQueue(declaration.m_Priority);

	// Increment the counter
	if (declaration.m_Counter)
		declaration.m_Counter->increment();

	// Throw an exception if the job was able to be enqueued
	if (queue.enqueue(declaration) == false)
		throw GRAVEngine::Exceptions::jobQueueFullException("Job queue full");
}
void GRAVEngine::Jobs::jobManager::kickJobs(const declaration* declarations, size_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		kickJob(declarations[i]);
	}
}
void GRAVEngine::Jobs::jobManager::kickJobAndWait(const declaration& declaration)
{
	GRAV_ASSERT(declaration.m_Counter);

	kickJob(declaration);
	waitForCounter(declaration.m_Counter, 0);
}
void GRAVEngine::Jobs::jobManager::kickJobsAndWait(const declaration* declarations, size_t count)
{
	// Kick off the jobs
	kickJobs(declarations, count);

	// Wait for every job's counter
	for (size_t i = 0; i < count; i++)
	{
		waitForCounter(declarations[i].m_Counter, 0);
	}
}

struct waitForCounterProxyArgs
{
	GRAVEngine::ref<GRAVEngine::Jobs::counter> m_Counter;
	GRAVEngine::Jobs::counterTarget m_Target;
	std::condition_variable* cv;
};

void waitForCounterProxy(waitForCounterProxyArgs args)
{
	GRAVEngine::Jobs::jobManager::getInstance()->waitForCounter(args.m_Counter, args.m_Target, false);
	args.cv->notify_all();
}
void GRAVEngine::Jobs::jobManager::waitForCounter(ref<counter> counter, counterTarget target, bool blocking)
{
	GRAV_ASSERT(counter);
	
	// Do nothing if the target is already met
	if (counter->getValue() == target)
		return;
	
	// Get the thread local storage
	tls* tls = getCurrentTLS();

	// Should this wait block execution
	if (blocking)
	{
		// Set up condition variable
		std::condition_variable cv;
		waitForCounterProxyArgs args = { counter, target, &cv };

		// Kick off proxy job to make this thread keep working while waiting on other fibers to complete
		declaration declaration((entryPoint) waitForCounterProxy, (uintptr_t)&args, jobPriority::CRITICAL, nullptr );
		kickJob(declaration);

		// Lock this thread
		std::mutex mutex;
		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock);

		return;
	}

	// Add the current fiber to the waiting fiber list
	addWaitingFiber(counter, tls->m_CurrentFiberIndex, target);

	// While waiting, run other jobs

	// Update the thread local storage
	tls->m_PreviousFiberIndex = tls->m_CurrentFiberIndex;
	tls->m_PreviousFiberDestination = fiberDestination::WAITNIG;

	// Switch to an idle fiber while waiting
	tls->m_CurrentFiberIndex = findFreeFiber();
	tls->m_Fiber.switchTo(&m_Fibers[tls->m_CurrentFiberIndex]);

	// Clean up the previous fiber information
	cleanPreviousFiber(tls);
}


GRAVEngine::Jobs::gravThread* GRAVEngine::Jobs::jobManager::getThread(uint8 index)
{
	GRAV_ASSERT(index < m_ThreadCount);
	return &m_Threads[index];
}
void GRAVEngine::Jobs::jobManager::spawnThread(uint8 index)
{
	GRAV_LOG_LINE_INFO("%s: Spawn Thread %u", __FUNCTION__, index);
	
	// Get the thread and spawn it. 
	getThread(index)->spawn(threadCallback);
}


void GRAVEngine::Jobs::jobManager::addWaitingFiber(ref<counter> counter, fiberIndex index, counterTarget target)
{
	GRAV_ASSERT(counter);

	// This slot is now obtained
	Locks::scopedLock<decltype(m_WaitingFiberLock)> lock(m_WaitingFiberLock);

	for (size_t i = 0; i < m_MaxWaitingFibersCount; i++)
	{
		// Determine if this indexed fiber is free, and obtain it if it is free
		bool expected = true;
		if (std::atomic_compare_exchange_strong_explicit(&m_FreeWaitingFibers[i], &expected, false, std::memory_order_seq_cst, std::memory_order_relaxed))
		{
			// Get the slot and set the values
			auto slot = &m_WaitingFibers[i];
			slot->m_FiberIndex = index;
			slot->m_TargetValue = target;
			slot->counter = counter;
			return;
		}
	}

	// There are no free slots
	throw GRAVEngine::Exceptions::counterWaitListFullException("Job Counter waiting slots full");
}
void GRAVEngine::Jobs::jobManager::checkWaitingFibers()
{
	Locks::scopedLock<decltype(m_WaitingFiberLock)> lock(m_WaitingFiberLock);

	for (size_t i = 0; i < m_MaxWaitingFibersCount; i++)
	{
		// Determine if this indexed fiber is not free
		if (m_FreeWaitingFibers[i].load(std::memory_order_acquire) == false)
		{
			// Get the slot and set the values
			auto slot = &m_WaitingFibers[i];

			// Check to see if this waiting fiber has not meet the target
			if (slot->m_TargetValue != slot->counter->getValue())
				continue;

			{
				Locks::scopedLock<decltype(m_ReadyFiberLock)> lock(m_ReadyFiberLock);
				// This waiting fiber has meet the target, so add it to the list of fibers ready to be resumed
				m_ReadyFibers.emplace_back(slot->m_FiberIndex);
			}

			// Free this slot for use
			m_FreeWaitingFibers[i].store(true, std::memory_order_release);
		}

	}
}



GRAVEngine::Jobs::threadIndex GRAVEngine::Jobs::jobManager::getCurrentThreadIndex() const
{
#ifdef GRAV_PLATFORM_WINDOWS
	threadID id = GetCurrentThreadId();
	for (threadIndex i = 0; i < m_ThreadCount; i++)
		if (m_Threads[i].getID() == id)
			return i;
#endif

	return UINT8_MAX;
}
GRAVEngine::Jobs::gravThread* GRAVEngine::Jobs::jobManager::getCurrentThread() const
{
#ifdef GRAV_PLATFORM_WINDOWS
	threadID id = GetCurrentThreadId();
	for (threadIndex i = 0; i < m_ThreadCount; i++)
		if (m_Threads[i].getID() == id)
			return &m_Threads[i];
#endif

	return nullptr;
}
GRAVEngine::Jobs::tls* GRAVEngine::Jobs::jobManager::getCurrentTLS() const
{
#ifdef GRAV_PLATFORM_WINDOWS
	threadID id = GetCurrentThreadId();
	for (threadIndex i = 0; i < m_ThreadCount; i++)
		if (m_Threads[i].getID() == id)
			return m_Threads[i].getTLS();
#endif

	return nullptr;
}

GRAVEngine::Jobs::fiberIndex GRAVEngine::Jobs::jobManager::findFreeFiber()
{
	while (true)
	{
		// Scan through all of the fibers
		for (fiberIndex i = 0; i < m_FiberCount; i++)
		{
			// Check if the fiber is free
			if (m_IdleFibers[i].load(std::memory_order_acquire) == false)
				continue;

			// See if the value is exchanged. If this fails, another thread beat this thread in exchanging, so don't double book the fiber
			bool expected = true;
			if (std::atomic_compare_exchange_strong_explicit(&m_IdleFibers[i], &expected, false, std::memory_order_release, std::memory_order_relaxed))
				return i;
		}
	}
}
void GRAVEngine::Jobs::jobManager::cleanPreviousFiber(tls* tls)
{
	GRAV_ASSERT(tls);

	switch (tls->m_PreviousFiberDestination)
	{
	case fiberDestination::NONE:
		return;
	case fiberDestination::POOL:
		// The previous fiber was being placed into the pool, so state that the fiber is now idle and free
		m_IdleFibers[tls->m_PreviousFiberIndex].store(true, std::memory_order_release);
		break;
	case fiberDestination::WAITNIG:
		// The previous fiber was being placed into a waiting list
		//tls->m_PreviousFiberInWaitingList->store(true, std::memory_order_relaxed);
		break;
	default:
		break;

	}
	
	// Clean the thread local storage
	tls->clean();
}


GRAVEngine::Jobs::jobQueue& GRAVEngine::Jobs::jobManager::getQueue(jobPriority priority)
{
	switch (priority)
	{
	case GRAVEngine::Jobs::jobPriority::LOW:
		return s_Instance->m_LowPriorityQueue;
		break;
	case GRAVEngine::Jobs::jobPriority::NORMAL:
		return s_Instance->m_NormalPriorityQueue;
		break;
	case GRAVEngine::Jobs::jobPriority::HIGH:
		return s_Instance->m_HighPriorityQueue;
		break;
	case GRAVEngine::Jobs::jobPriority::CRITICAL:
		return s_Instance->m_CriticalPriorityQueue;
		break;
	default:
		return s_Instance->m_LowPriorityQueue;
		break;
	}
}
bool GRAVEngine::Jobs::jobManager::getNextJob(declaration& declaration, tls* tls)
{
	if (tls == nullptr)
		tls = getCurrentTLS();

	// Always run critical jobs first
	if (m_CriticalPriorityQueue.dequeue(declaration))
		return true;

	// Look at the fibers that were placed on the waiting list and are now ready to be ran again.
	// Ready fibers
	for (auto it = m_ReadyFibers.begin(); it != m_ReadyFibers.end(); it++)
	{
		fiberIndex index = *it;

		// Erase the job
		{
			// TODO: Determine if the ready fiber lock needs to also enclose the loop
			// Need lock because m_ReadyFibers is modified
			Locks::scopedLock<decltype(m_ReadyFiberLock)> lock(m_ReadyFiberLock);

			m_ReadyFibers.erase(it);
		}

		// Update the thread local storage
		tls->m_PreviousFiberIndex = tls->m_CurrentFiberIndex;
		tls->m_PreviousFiberDestination = fiberDestination::POOL;
		tls->m_CurrentFiberIndex = index;

		// Switch to the fiber
		tls->m_Fiber.switchTo(&m_Fibers[index]);
		cleanPreviousFiber(tls);

		break;
	}

	if (m_HighPriorityQueue.dequeue(declaration))
		return true;
	if (m_NormalPriorityQueue.dequeue(declaration))
		return true;
	if (m_LowPriorityQueue.dequeue(declaration))
		return true;

	return false;
}

void GRAVEngine::Jobs::jobManager::fiberCallbackMain(fiber* fiber)
{
	GRAVEngine::Jobs::jobManager& manager = *GRAVEngine::Jobs::jobManager::getInstance();

	GRAV_LOG_LINE_INFO("%s: Begin Main Fiber Callback", __FUNCTION__);
	
	GRAV_LOG_LINE_INFO("%s: Before Main Execution", __FUNCTION__);
	// Run the main method
	manager.m_MainMethod();
	GRAV_LOG_LINE_INFO("%s: After Main Execution", __FUNCTION__);

	// Should the job system continue to run after the main method has been ran?
	if (manager.m_ShutdownAfterMain == false)
	{
		// Run the main thread as any other thread now.
		GRAV_LOG_LINE_INFO("%s: JobManager doesn't shutdown after main method. Running like normal fiber", __FUNCTION__);

		// Switch to an idle fiber to replace the main fiber
		const auto tls = manager.getCurrentTLS();
		tls->m_CurrentFiberIndex = manager.findFreeFiber();

		const auto fiber = &manager.m_Fibers[tls->m_CurrentFiberIndex];
		tls->m_Fiber.switchTo(fiber);
	}

	// Shut down the job system
	manager.startShutdown();

	GRAV_LOG_LINE_INFO("%s: End Main Fiber Callback", __FUNCTION__);
	// Switch back to the starting code than ran main
	fiber->switchToCallingFiber();
}
void GRAVEngine::Jobs::jobManager::fiberCallback(fiber* fiber)
{
	jobManager& manager = *jobManager::getInstance();

	GRAV_LOG_LINE_INFO("%s: Begin Fiber Callback", __FUNCTION__);
	
	// This fiber is starting. Clean the previous fiber of its resources
	manager.cleanPreviousFiber(manager.getCurrentTLS());

	declaration job;
	while (manager.isShuttingDown() == false)
	{
		// Get the current thread local storage
		tls* tls = manager.getCurrentTLS();
		
		// Get the next job
		if (manager.getNextJob(job, tls))
		{
			//GRAV_LOG_LINE_INFO("Thread %u has found a job to run.", tls->m_ThreadIndex);

			// Run the job
			job.m_EntryPoint(job.m_Param);

			// Decrement the counter associated with this job if it exists
			if (job.m_Counter)
				// Decrement the counter because this job has now finished
				counterTarget counter = job.m_Counter->decrement();

			//GRAV_LOG_LINE_INFO("Thread %u has finished its job.", tls->m_ThreadIndex);

			// Check if there are any fibers that need to be woken up
			jobManager::getInstance()->checkWaitingFibers();

			// Check for the next job
			continue;
		}

		// Make the thread sleep for 1 ms until there is another job
		GRAVEngine::Jobs::gravThread::sleepFor(1);
	}

	GRAV_LOG_LINE_INFO("%s: End Fiber Callback", __FUNCTION__);
	
	// Switch back to the calling thread
	fiber->switchToCallingFiber();
}
void GRAVEngine::Jobs::jobManager::threadCallback(gravThread* gravThread)
{
	jobManager& manager = *jobManager::getInstance();

	GRAV_LOG_LINE_INFO("%s: Initialize Thread: %u", __FUNCTION__, gravThread->getID());

	// Wait until the job manager is valid
	while (manager.isShuttingDown() || manager.isValid() == false)
	{
		if (manager.isShuttingDown())
		{
			GRAV_LOG_LINE_WARN("%s: Thread: %u | JobManager was shutting down while a thread was initialing. Exit initialization.", __FUNCTION__, gravThread->getID());

			return;
		}

		GRAV_LOG_LINE_INFO("%s: Thread: %u | Waiting for job manager to become valid.", __FUNCTION__, gravThread->getID());
		GRAVEngine::Jobs::gravThread::sleepFor(1);
	}

	// Get the thread's thread local storage
	GRAVEngine::Jobs::tls* tls = gravThread->getTLS();

	// Set the thread's affinity
	if (tls->m_HasAffinity)
	{
		GRAV_LOG_LINE_INFO("%s: Thread: %u | Set Thread Affinity", __FUNCTION__, gravThread->getID());
		gravThread->setAffinity(tls->m_ThreadIndex);
	}

	// Setup the thread's fiber
	tls->m_Fiber.initializeFromCurrentThread();
	tls->m_CurrentFiberIndex = manager.findFreeFiber();

	// Switch to the found fiber
	GRAV_LOG_LINE_INFO("%s: Thread: %u | Start fiber", __FUNCTION__, gravThread->getID());
	const auto fiber = &manager.m_Fibers[tls->m_CurrentFiberIndex];
	tls->m_Fiber.switchTo(fiber);
}

