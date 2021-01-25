#include "gravpch.h"
#include "jobManager.h"

// Static initial definition of instance
GRAVEngine::jobs::jobManager* GRAVEngine::jobs::jobManager::s_Instance = nullptr;

GRAVEngine::jobs::jobManager::jobManager()
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

	// Job queues
	m_CriticalPriorityQueue = nullptr;
	m_HighPriorityQueue = nullptr;
	m_NormalPriorityQueue = nullptr;
	m_LowPriorityQueue = nullptr;


	// Create the threads
	m_Threads = nullptr;
	m_Fibers = nullptr;
	m_IdleFibers = nullptr;

	m_IsValid.store(false, std::memory_order_release);

}

GRAVEngine::jobs::jobManager::~jobManager()
{
	delete[] m_WaitingFibers;
	delete[] m_FreeWaitingFibers;
	delete[] m_Threads;
	delete[] m_Fibers;
	delete[] m_IdleFibers;

	delete m_CriticalPriorityQueue;
	delete m_HighPriorityQueue;
	delete m_NormalPriorityQueue;
	delete m_LowPriorityQueue;
}

void GRAVEngine::jobs::jobManager::startUp(jobManagerOptions& options/*, mainMethodFunction mainMethod*/)
{
	GRAV_ASSERT(s_Instance == nullptr);
	GRAV_ASSERT(options.m_NumFibers > 0);
	GRAV_ASSERT(options.m_NumThreads > 0);
	GRAV_ASSERT(options.m_ThreadAffinity && options.m_NumThreads > std::thread::hardware_concurrency() + 1);
	GRAV_ASSERT(options.m_MaxWaitingFibersCount > 1);

	// Threads
	m_ThreadCount = options.m_NumThreads;
	m_ThreadAffinity = options.m_ThreadAffinity;
	m_AutoInitializeThreads = options.m_AutoInitializeThreads;

	// Fiber count
	m_FiberCount = options.m_NumFibers;

	// Waiting Fibers
	m_MaxWaitingFibersCount = options.m_MaxWaitingFibersCount;
	m_WaitingFibers = new waitingFiberSlot[m_MaxWaitingFibersCount];
	m_FreeWaitingFibers = new std::atomic_bool[m_MaxWaitingFibersCount];

	// Job queues
	m_CriticalPriorityQueue = new jobQueue(options.m_CriticalPriorityQueueSize);
	m_HighPriorityQueue		= new jobQueue(options.m_HighPriorityQueueSize);
	m_NormalPriorityQueue	= new jobQueue(options.m_NormalPriorityQueueSize);
	m_LowPriorityQueue		= new jobQueue(options.m_LowPriorityQueueSize);


	// Create the threads
	m_Threads = new gravThread[m_ThreadCount];

	// Load up the information about the main thread
	auto mainThread = &m_Threads[0];

	// Get the main thread's thread local storage and convert this thread into a fiber
	tls* mainThreadTLS = mainThread->getTLS();
	mainThreadTLS->m_Fiber.initializeFromCurrentThread();

	// ********** This thread is a fiber from this point forward

	// Set the affinity if this is the main method
	//if (mainMethod)
	//	if (m_ThreadAffinity)
	//		mainThread->setAffinity(1);

	// Create the fibers. Must be done after the main thread has been converted into a fiber
	m_Fibers = new fiber[m_FiberCount];
	m_IdleFibers = new std::atomic_bool[m_FiberCount];

	// Set the fiber callbacks
	for (size_t i = 0; i < m_FiberCount; i++)
	{
		m_Fibers[i].setCallback(fiberCallback);
		m_IdleFibers[i].store(true, std::memory_order_release);
	}

	// Spawn the threads
	for (threadIndex i = 0; i < m_ThreadCount; i++)
	{
		// set thread index
		auto tls = m_Threads[i].getTLS();
		tls->m_ThreadIndex = i;

		//if (i == 0)
		//	continue;

		tls->m_HasAffinity = m_ThreadAffinity;

		if (m_AutoInitializeThreads)
			spawnThread(i);
	}

	// The job manager is now valid
	m_IsValid.store(true, std::memory_order_release);

	//// Set the currently free fiber
	//mainThreadTLS->m_CurrentFiberIndex = findFreeFiber();
	//
	//// Calling the Main function
	//m_MainCallback = mainMethod;
	//if (m_MainCallback == nullptr && m_ShutdownAfterMain)
	//{
	//	// Null callback error
	//}
	//
	//// Setup main fiber
	//const auto mainFiber = &m_Fibers[mainThreadTLS->m_CurrentFiberIndex];
	//mainFiber->setCallback(fiberCallbackMain);
	//	
	//// Execute the job system
	//if (m_MainCallback)
	//	mainThreadTLS->m_Fiber.switchTo(mainFiber, this);
	//
	//
	//// END of the job system
	//if (m_MainCallback)
	//{
	//	// Wait for all threads to shut down
	//	for (size_t i = 0; i < m_ThreadCount; i++)
	//	{
	//		m_Threads[i].join();
	//	}
	//}
}
void GRAVEngine::jobs::jobManager::shutDown()
{
	GRAV_ASSERT(s_Instance);

	// TODO: Properly shutdown the job manager. Thread and fiber shut down and memory management

	// The job manager is now invalid
	m_IsValid.store(false, std::memory_order_release);

	// Wait for all of the threads to complete
	for (size_t i = 0; i < m_ThreadCount; i++)
	{
		m_Threads[i].join();
	}

	delete[] m_WaitingFibers;
	delete[] m_FreeWaitingFibers;
	delete[] m_Threads;
	delete[] m_Fibers;
	delete[] m_IdleFibers;

	delete m_CriticalPriorityQueue;
	delete m_HighPriorityQueue;
	delete m_NormalPriorityQueue;
	delete m_LowPriorityQueue;

	s_Instance = nullptr;
}

void GRAVEngine::jobs::jobManager::kickJob(const declaration& declaration)
{
	GRAV_ASSERT(declaration.m_EntryPoint);

	auto queue = getQueue(declaration.m_Priority);

	// Increment the counter
	if (declaration.m_Counter)
		declaration.m_Counter->increment();

	// Throw an exception if the job was able to be enqueued
	if (queue.enqueue(declaration) == false)
		throw GRAVEngine::Exceptions::jobQueueFullException("Job queue full");
}
void GRAVEngine::jobs::jobManager::kickJobs(const declaration* declarations, size_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		kickJob(declarations[i]);
	}
}
void GRAVEngine::jobs::jobManager::kickJobAndWait(const declaration& declaration)
{
	GRAV_ASSERT(declaration.m_Counter);

	kickJob(declaration);
	waitForCounter(declaration.m_Counter, 0);
}
void GRAVEngine::jobs::jobManager::kickJobsAndWait(const declaration* declarations, size_t count)
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
	GRAVEngine::jobs::counter* m_Counter;
	GRAVEngine::jobs::counterTarget m_Target;
	std::condition_variable* cv;
};

void waitForCounterProxy(waitForCounterProxyArgs args)
{
	GRAVEngine::jobs::jobManager::getInstance()->waitForCounter(args.m_Counter, args.m_Target, false);
	args.cv->notify_all();
}
void GRAVEngine::jobs::jobManager::waitForCounter(counter* counter, counterTarget target, bool blocking)
{
	GRAV_ASSERT(counter);
	
	// Do nothing if the target is already met
	if (counter->getValue() == target)
		return;
	
	// Get the thread local storage
	tls* tls = getCurrentTLS();

	// TODO: Determine if this block of code should be removed
	// Should this wait block execution
	if (blocking)
	{
		// Stop blocking if the target is met
		if (counter->getValue() == target)
			return;

		// Set up condition variable
		std::condition_variable cv;
		waitForCounterProxyArgs args = { counter, target, &cv };


		// Kick off proxy job to make this thread keep working while waiting on other fibers to complete
		declaration declaration((entryPoint) waitForCounterProxy, (uintptr_t)&args, jobPriority::CRITICAL, counter );
		//declaration declaration((entryPoint*) waitForCounterProxy, (uintptr_t)&args, jobPriority::CRITICAL, counter );
		kickJob(declaration);


		// Lock this thread
		std::mutex mutex;
		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock);

		return;
	}


	// Add the current fiber to the waiting fiber list
	addWaitingFiber(tls->m_CurrentFiberIndex, target);

	//// Is this fiber stored
	//auto fiberStored = new std::atomic_bool(false);
	//// Check if the counter target has been made
	//if (counter->addWaitingFiber(tls->m_CurrentFiberIndex, target, fiberStored))
	//{
	//	delete fiberStored;
	//	return;
	//}

	// While waiting, run other jobs

	// Update the thread local storage
	tls->m_PreviousFiberIndex = tls->m_CurrentFiberIndex;
	tls->m_PreviousFiberDestination = fiberDestination::WAITNIG;
	//tls->m_PreviousFiberInWaitingList = fiberStored;

	// Switch to an idle fiber while waiting
	tls->m_CurrentFiberIndex = findFreeFiber();
	tls->m_Fiber.switchTo(&m_Fibers[tls->m_CurrentFiberIndex]);

	// Clean up the previous fiber information
	cleanPreviousFiber();
}


GRAVEngine::jobs::gravThread* GRAVEngine::jobs::jobManager::getThread(uint8 index)
{
	GRAV_ASSERT(index < m_ThreadCount);
	return &m_Threads[index];
}
void GRAVEngine::jobs::jobManager::spawnThread(uint8 index)
{
	getThread(index)->spawn(threadCallback);
}


void GRAVEngine::jobs::jobManager::addWaitingFiber(fiberIndex index, counterTarget target)
{
	for (size_t i = 0; i < m_MaxWaitingFibersCount; i++)
	{
		// Determine if this indexed fiber is free, and obtain it if it is free
		bool expected = true;
		if (std::atomic_compare_exchange_strong_explicit(&m_FreeWaitingFibers[i], &expected, false, std::memory_order_seq_cst, std::memory_order_relaxed))
		{
			// This slot is now obtained
			Locks::scopedLock<decltype(m_WaitingFiberLock)> lock(m_WaitingFiberLock);

			// Get the slot and set the values
			auto slot = &m_WaitingFibers[i];
			slot->m_FiberIndex = index;
			slot->m_TargetValue = target;
		}
	}

	// There are no free slots
	throw GRAVEngine::Exceptions::counterWaitListFullException("Job Counter waiting slots full");
}
void GRAVEngine::jobs::jobManager::checkWaitingFibers(counterTarget target)
{
	for (size_t i = 0; i < m_MaxWaitingFibersCount; i++)
	{
		// Determine if this indexed fiber is not free
		bool expected = true;
		if (m_FreeWaitingFibers[i].load(std::memory_order_acquire) == false)
		{
			Locks::scopedLock<decltype(m_WaitingFiberLock)> lock(m_WaitingFiberLock);

			// Get the slot and set the values
			auto slot = &m_WaitingFibers[i];

			// Check to see if this waiting fiber has not meet the target
			if (slot->m_TargetValue != target)
				continue;

			// This waiting fiber has meet the target, so add it to the list of fibers ready to be resumed
			//getCurrentTLS()->m_ReadyFibers.emplace_back(slot->m_FiberIndex, slot->m_FiberStored);
			m_ReadyFibers.emplace_back(slot->m_FiberIndex);

			// Free this slot for use
			m_FreeWaitingFibers[i].store(true, std::memory_order_release);
		}

	}
}



GRAVEngine::jobs::threadIndex GRAVEngine::jobs::jobManager::getCurrentThreadIndex() const
{
#ifdef _WIN32
	threadHandle handle = GetCurrentThread();
	for (threadIndex i = 0; i < m_ThreadCount; i++)
		if (m_Threads[i].getNativeHandle() == handle)
			return i;
#endif

	return UINT8_MAX;
}
GRAVEngine::jobs::gravThread* GRAVEngine::jobs::jobManager::getCurrentThread() const
{
#ifdef _WIN32
	threadHandle handle = GetCurrentThread();
	for (threadIndex i = 0; i < m_ThreadCount; i++)
		if (m_Threads[i].getNativeHandle() == handle)
			return &m_Threads[i];
#endif

	return nullptr;
}
GRAVEngine::jobs::tls* GRAVEngine::jobs::jobManager::getCurrentTLS() const
{
#ifdef _WIN32
	threadHandle handle = GetCurrentThread();
	for (size_t i = 0; i < m_ThreadCount; i++)
		if (m_Threads[i].getNativeHandle() == handle)
			return m_Threads[i].getTLS();

#endif
	return nullptr;
}

GRAVEngine::jobs::fiberIndex GRAVEngine::jobs::jobManager::findFreeFiber()
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
			if (std::atomic_compare_exchange_weak_explicit(&m_IdleFibers[i], &expected, false, std::memory_order_release, std::memory_order_relaxed))
				return i;
		}
	}
}
void GRAVEngine::jobs::jobManager::cleanPreviousFiber(tls* tls)
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


GRAVEngine::jobs::jobQueue& GRAVEngine::jobs::jobManager::getQueue(jobPriority priority)
{
	switch (priority)
	{
	case GRAVEngine::jobs::jobPriority::LOW:
		return *m_LowPriorityQueue;
		break;
	case GRAVEngine::jobs::jobPriority::NORMAL:
		return *m_NormalPriorityQueue;
		break;
	case GRAVEngine::jobs::jobPriority::HIGH:
		return *m_HighPriorityQueue;
		break;
	case GRAVEngine::jobs::jobPriority::CRITICAL:
		return *m_CriticalPriorityQueue;
		break;
	default:
		return *m_LowPriorityQueue;
		break;
	}
}
bool GRAVEngine::jobs::jobManager::getNextJob(declaration& declaration, tls* tls)
{
	if (tls == nullptr)
		tls = getCurrentTLS();


	// Always run critical jobs first
	if (m_CriticalPriorityQueue->dequeue(declaration))
		return true;

	// Look at the fibers that were placed on the waiting list and are now ready to be ran again.
	// Ready fibers
	//for (auto it = tls->m_ReadyFibers.begin(); it != tls->m_ReadyFibers.end(); it++)
	for (auto it = m_ReadyFibers.begin(); it != m_ReadyFibers.end(); it++)
	{
		fiberIndex index = *it;

		// Make sure fiber is stored
		//if (it->second->load(std::memory_order_acquire) == false)
		//	continue;

		// Erase the job
		//delete it->second;
		//tls->m_ReadyFibers.erase(it);
		m_ReadyFibers.erase(it);

		// Update the thread local storage
		tls->m_PreviousFiberIndex = tls->m_CurrentFiberIndex;
		tls->m_PreviousFiberDestination = fiberDestination::POOL;
		tls->m_CurrentFiberIndex = index;

		// Switch to the fiber
		tls->m_Fiber.switchTo(&m_Fibers[index]);
		cleanPreviousFiber(tls);

		break;
	}

	return m_HighPriorityQueue->dequeue(declaration) || m_NormalPriorityQueue->dequeue(declaration) || m_LowPriorityQueue->dequeue(declaration);
}

//void GRAVEngine::jobs::jobManager::fiberCallbackMain(fiber* fiber)
//{
//	auto manager = reinterpret_cast<jobManager*> (fiber->getUserData());
//
//	if (manager->m_MainCallback)
//		manager->m_MainCallback(manager);
//
//	if (manager->m_ShutdownAfterMain && manager->m_MainCallback)
//	{
//		// Switch to an idle fiber
//		const auto tls = manager->getCurrentTLS();
//		tls->m_CurrentFiberIndex = manager->findFreeFiber();
//
//		const auto fiber = &manager->m_Fibers[tls->m_CurrentFiberIndex];
//		tls->m_Fiber.switchTo(fiber, manager);
//	}
//
//	// Switch back to the calling fiber
//	fiber->switchToCallingFiber();
//}
void GRAVEngine::jobs::jobManager::fiberCallback(fiber* fiber)
{
	jobManager* manager = jobManager::getInstance();

	// This fiber is starting. Clean the previous fiber of its resources
	manager->cleanPreviousFiber(manager->getCurrentTLS());

	declaration job;
	while (manager->isValid())
	{
		// Get the current thread local storage
		tls* tls = manager->getCurrentTLS();
		
		// Get the next job
		if (manager->getNextJob(job, tls))
		{
			// Run the job
			job.m_EntryPoint(job.m_Param);

			// Decrement the counter associated with this job if it exists
			if (job.m_Counter)
			{
				// Decrement the counter because this job has now finished
				counterTarget counter = job.m_Counter->decrement();

				// Check if there are any fibers that need to be woken up
				jobManager::getInstance()->checkWaitingFibers(counter - 1);

			}

			// Check for the next job
			continue;
		}

		// Make the thread sleep for 1 ms until there is another job
		GRAVEngine::jobs::gravThread::sleepFor(1);
	}

	// Switch back to the calling thread
	fiber->switchToCallingFiber();
}
void GRAVEngine::jobs::jobManager::threadCallback(gravThread* gravThread)
{
	jobManager* manager = jobManager::getInstance();

	// Get the thread's thread local storage
	GRAVEngine::jobs::tls* tls = gravThread->getTLS();

	// Set the thread's affinity
	gravThread->setAffinity(tls->m_ThreadIndex);

	// Setup the thread's fiber
	tls->m_Fiber.initializeFromCurrentThread();
	tls->m_CurrentFiberIndex = manager->findFreeFiber();

	// Switch
	const auto fiber = &manager->m_Fibers[tls->m_CurrentFiberIndex];
	tls->m_Fiber.switchTo(fiber);
}

