#include "gravpch.h"
#include "jobManager.h"

#include "locks/scopedLock.h"

#include <sstream>

GRAV_JOB_ENTRY_POINT(GRAVEngine::Jobs::switchToReadyFiber)
{
	// Get job manager and the current thread local storage
	jobManager* instance = jobManager::getInstance();
	tls* tls = instance->getCurrentTLS();

	// Set the new thread local storage
	fiberBundle* bundle = (fiberBundle*)param;
	tls->m_PreviousFiberIndex = tls->m_CurrentFiberIndex;
	tls->m_CurrentFiberIndex = bundle->m_Index;
	tls->m_PreviousFiberDestination = fiberDestination::POOL;

	// Fiber detached callback
	if (instance->m_Callbacks.m_OnFiberDetached)
		instance->m_Callbacks.m_OnFiberDetached(tls->m_PreviousFiberIndex, false);

	// Switch to the now ready fiber
	instance->m_Fibers[tls->m_PreviousFiberIndex].switchTo(&instance->m_Fibers[tls->m_CurrentFiberIndex]);

	// Get the thread local storage again as it could have changed
	tls = instance->getCurrentTLS();

	// Fiber attached callback
	if (instance->m_Callbacks.m_OnFiberAttached)
		instance->m_Callbacks.m_OnFiberAttached(tls->m_CurrentFiberIndex);

	// Clean the previous fiber after execution
	instance->cleanPreviousFiber();
}


// Static initial definition of instance
GRAVEngine::Jobs::jobManager* GRAVEngine::Jobs::jobManager::s_Instance = nullptr;

GRAVEngine::Jobs::jobManager::jobManager()
{
	// Threads
	m_ThreadCount = 0;
	m_ThreadAffinity = 0;

	// Fiber count
	m_FiberCount = 0;
	
	// Create the threads
	m_Threads = nullptr;
	m_Fibers = nullptr;
	m_IdleFibers = nullptr;

	m_QueueBehavior = queueBehavior::SPIN;
}
GRAVEngine::Jobs::jobManager::~jobManager() {}

void GRAVEngine::Jobs::jobManager::startUp(jobManagerOptions& options)
{
	GRAV_PROFILE_FUNCTION();

	GRAV_ASSERT(s_Instance == nullptr);
	GRAV_ASSERT(options.m_NumFibers > 0);
	GRAV_ASSERT(options.m_NumThreads > 0);
	GRAV_ASSERT(options.m_NumFibers >= options.m_NumThreads);
	if (options.m_ThreadAffinity)
		GRAV_ASSERT(options.m_NumThreads <= std::thread::hardware_concurrency());
	GRAV_ASSERT(options.m_MaxWaitingFibersCount > 1);

	m_Callbacks = options.m_Callbacks;

	GRAV_LOG_LINE_INFO("%s: Begin Startup", GRAV_CLEAN_FUNC_SIG);

	s_Instance = this;

	// Threads
	m_ThreadCount			= options.m_NumThreads;
	m_ThreadAffinity		= options.m_ThreadAffinity;

	// Fiber count
	m_FiberCount			= options.m_NumFibers;

	// Job queues
	m_CriticalPriorityQueue	= jobQueue(options.m_CriticalPriorityQueueSize);
	m_HighPriorityQueue		= jobQueue(options.m_HighPriorityQueueSize);
	m_NormalPriorityQueue	= jobQueue(options.m_NormalPriorityQueueSize);
	m_LowPriorityQueue		= jobQueue(options.m_LowPriorityQueueSize);

	// Thread queue behavior
	m_QueueBehavior.store(options.m_QueueBehavior);

	// Create the threads
	GRAV_LOG_LINE_INFO("%s: Create Threads", GRAV_CLEAN_FUNC_SIG);
	m_Threads = createScope<gravThread[]>(m_ThreadCount);

	GRAV_LOG_LINE_INFO("%s: Create Fibers", GRAV_CLEAN_FUNC_SIG);
	m_Fibers = createScope<fiber[]>(m_FiberCount);
	m_ReadyFibers = createScope<fiberBundle[]>(m_FiberCount);
	m_IdleFibers = createScope<std::atomic_bool[]>(m_FiberCount);

	// Spawn the threads
	{
		GRAV_PROFILE_SCOPE("Initialize Threads");
		GRAV_LOG_LINE_INFO("%s: Initialize Threads", GRAV_CLEAN_FUNC_SIG);
		for (threadIndex i = 0; i < m_ThreadCount; i++)
		{
			// set thread index
			auto tls = m_Threads[i].getTLS();
			tls->m_ThreadIndex = i;

			if (i != GRAV_MAIN_THREAD_INDEX)
			{
				// Spawn a worker thread
				GRAV_PROFILE_SCOPE("Spawn Thread");

				GRAV_LOG_LINE_INFO("%s: Spawn Worker Thread %u", GRAV_CLEAN_FUNC_SIG, i);
				m_Threads[i].spawn(threadCallback);
			}
			else
			{
				GRAV_LOG_LINE_INFO("%s: Initialize main thread", GRAV_CLEAN_FUNC_SIG);

				// Initialize the main thread into a gravThread because std::thread sucks
				m_Threads[i].initializeFromCurrentThread();

				// Main thread on initialized callback
				if (m_Callbacks.m_OnThreadInitialized)
					m_Callbacks.m_OnThreadInitialized(0);

				// We do not initialize the main thread's TLS fiber because it isn't used.
				// It is impossible to sync it's fiber with any fiber's in the pool, so just use one of the fiber's in the pool instead
				//tls->m_Fiber.initializeFromCurrentThread();
				tls->m_CurrentFiberIndex = GRAV_MAIN_FIBER_INDEX;
			}

			// Set the thread's name
			std::wstring threadName = L"Thread " + std::to_wstring(i);
			m_Threads[i].setName(threadName);

			// Set the thread's affinity
			if (m_ThreadAffinity)
				m_Threads[i].setAffinity(i);
		}

		// Thread creation callback
		if (m_Callbacks.m_OnThreadsCreated)
			m_Callbacks.m_OnThreadsCreated(m_ThreadCount);
	}

	// Spawn the fiber pool
	{
		GRAV_PROFILE_SCOPE("Spawn Fibers");
		GRAV_LOG_LINE_INFO("%s: Spawn Fibers", GRAV_CLEAN_FUNC_SIG);
		for (fiberIndex i = 0; i < m_FiberCount; i++)
		{
			fiber* fiber = &m_Fibers[i];
			fiber->setIndex(i);

			if (i != GRAV_MAIN_FIBER_INDEX)
			{
				// Spawn a fiber for the fiber pool
				fiber->spawn(fiberCallback);
				m_IdleFibers[i].store(true, std::memory_order_release);
			}
			else
			{
				GRAV_LOG_LINE_INFO("%s: Convert main thread into fiber", GRAV_CLEAN_FUNC_SIG);

				// Initialize from the main thread into a fiber and bind it to its coresponding fiber in the pool
				m_Fibers[i].initializeFromCurrentThread();
				// The main fiber is not idle or free
				m_IdleFibers[i].store(false, std::memory_order_release);
			}
		}

		// Fiber creation callback
		if (m_Callbacks.m_OnFibersCreated)
			m_Callbacks.m_OnFibersCreated(m_FiberCount);
	}

	// Fiber attached callback for the main fiber because it wouldn't be called otherwise
	if (m_Callbacks.m_OnFiberAttached)
		m_Callbacks.m_OnFiberAttached(GRAV_MAIN_FIBER_INDEX);

	m_IsValid.store(true, std::memory_order_relaxed);
	// Job manager statup callback
	if (m_Callbacks.m_OnJobManagerStartup)
		m_Callbacks.m_OnJobManagerStartup();

	GRAV_LOG_LINE_INFO("%s: End Startup", GRAV_CLEAN_FUNC_SIG);
}
void GRAVEngine::Jobs::jobManager::shutDown()
{
	GRAV_PROFILE_FUNCTION();
	GRAV_ASSERT(s_Instance);

	// Create the fiber's quitting
	m_ThreadQuitFibers = createScope<fiber[]>(m_ThreadCount);
	for (size_t i = 0; i < m_ThreadCount; i++)
		m_ThreadQuitFibers[i].spawn(fiberQuitCallback);

	// The job manager is now shutting down is invalid
	m_IsValid.store(false, std::memory_order_release);
	m_ShuttingDown.store(true, std::memory_order_release);

	GRAV_LOG_LINE_INFO("%s: Begin Shutdown", GRAV_CLEAN_FUNC_SIG);

	// Wake up all of the sleeping threads
	if (m_QueueBehavior.load(std::memory_order_relaxed) == queueBehavior::SLEEP)
		m_ThreadCV.notify_all();

	// Fiber detached callback
	if (m_Callbacks.m_OnFiberDetached)
		m_Callbacks.m_OnFiberDetached(getCurrentTLS()->m_PreviousFiberIndex, false);

	// Jump to this thread's quit fiber. This is done just in case a different thread started the shutdown process.
	GRAV_LOG_LINE_INFO("%s: Jump to thread quit fiber to isolate the starting main thread", GRAV_CLEAN_FUNC_SIG);
	threadIndex currentThread = getCurrentThreadIndex();
	tls* tls = getCurrentTLS();
	fiber* threadQuitFiber = &m_ThreadQuitFibers[currentThread];
	m_Fibers[tls->m_CurrentFiberIndex].switchTo(threadQuitFiber);


	// This should be the main fiber again, so we can now wait for the other thread's to die.

	// Wait for all of the threads to complete now that the job manager is invalid
	GRAV_LOG_LINE_INFO("%s: Wait for worker threads to join", GRAV_CLEAN_FUNC_SIG);
	for (size_t i = 0; i < m_ThreadCount; i++)
		if (i != GRAV_MAIN_THREAD_INDEX)
			m_Threads[i].join();


	// Convert the main thread fiber back into a thread
	GRAV_LOG_LINE_INFO("%s: Convert the main fiber back into a thread", GRAV_CLEAN_FUNC_SIG);
	m_Threads[GRAV_MAIN_THREAD_INDEX].getTLS()->m_Fiber.convertToThread();

	s_Instance = nullptr;
	if (m_Callbacks.m_OnJobManagerShutdown)
		m_Callbacks.m_OnJobManagerShutdown();

	GRAV_LOG_LINE_INFO("%s: End Shutdown", GRAV_CLEAN_FUNC_SIG);
}


void GRAVEngine::Jobs::jobManager::kickJob(const declaration& declaration)
{
	// Kick a job but don't associate it with a counter
	kickJob(declaration, nullptr);
}
void GRAVEngine::Jobs::jobManager::kickJob(const declaration& declaration, ref<counter>* count)
{
	GRAV_PROFILE_FUNCTION();

	// Don't kick a job if the entry point is null
	if (declaration.m_EntryPoint == false)
		return;

	GRAV_LOG_LINE_INFO("%s: Kick Job | EntryPoint %ul | Priority %s", GRAV_CLEAN_FUNC_SIG, declaration.m_EntryPoint, toString(declaration.m_Priority));

	// Check if a memory address to store a potential counter exists
	if (count && *count)
		(*count)->increment();					// The memory address exists and the counter exists, so just increment the counter
	else if (count)
		(*count) = createRef<counter>(this, 1);	// The memory address exists, but a counter doesn't, so make one

	// Get the job queue
	jobQueue& queue = getQueue(declaration.m_Priority);

	if (queue.enqueue({ declaration, count }) == false)
		throw GRAVEngine::Exceptions::jobQueueFullException("Job queue full");

	// Wake up one of the sleeping threads to do this job
	if (m_QueueBehavior.load(std::memory_order_relaxed) == queueBehavior::SLEEP)
		m_ThreadCV.notify_one();
}
void GRAVEngine::Jobs::jobManager::kickJobs(const declaration* declarations, size_t count)
{
	GRAV_PROFILE_FUNCTION();

	// Kick an array of jobs not associated with a counter
	for (size_t i = 0; i < count; i++)
		kickJob(declarations[i]);
}
void GRAVEngine::Jobs::jobManager::kickJobs(const declaration* declarations, size_t count, ref<counter>* counter)
{
	GRAV_PROFILE_FUNCTION();

	// Kick an array of jobs associated with a counter
	for (size_t i = 0; i < count; i++)
		kickJob(declarations[i], counter);
}

void GRAVEngine::Jobs::jobManager::waitForCounter(ref<counter>& counter, counterTarget target)
{
	GRAV_PROFILE_FUNCTION();

	// Check for premature finishing
	if (counter->getValue() == target)
	{
		// Make sure that the counter is not being actively used
		while (counter->m_DestructionLock.load() > 0) {}
		return;
	}

	// Get the thread local storage
	tls* tls = getCurrentTLS();
	fiberIndex currentFiberIndex = tls->m_CurrentFiberIndex;

	// Get the current fiber bundle from the ready fibers
	fiberBundle* currentBundle = &m_ReadyFibers[currentFiberIndex];	// Get current fiber bundle
	currentBundle->m_Index = currentFiberIndex;						// It's index is the current index
	currentBundle->m_FiberIsSwitched.store(false);				// The bundle is not currently stored

	// Add the fiber to the waiting list, and check if it finished while doing so
	bool alreadyDone = counter->addWaitingFiber(currentBundle, target);
	// Return if it finished already
	if (alreadyDone)
		return;

	// While waiting, run other jobs
	fiberIndex freeFiberIndex = findFreeFiber();					// Find a free fiber

	// Update the thread local storage
	tls->m_PreviousFiberIndex = currentFiberIndex;					// Set previous index
	tls->m_PreviousFiberDestination = fiberDestination::WAITNIG;	// Set previous dest to waiting. The one passed in in now waiting
	tls->m_PreviousFiberStored = &currentBundle->m_FiberIsSwitched;	// Set previous stored flag as the pointer to the current bundle's flag
	tls->m_CurrentFiberIndex = freeFiberIndex;						// Find a free fiber

	// Fiber detached callback
	if (m_Callbacks.m_OnFiberDetached)
		m_Callbacks.m_OnFiberDetached(tls->m_PreviousFiberIndex, true);

	// Switch to the new fiber from the current fiber
	m_Fibers[currentFiberIndex].switchTo(&m_Fibers[freeFiberIndex]);

	// Get the thread local storage again as it could have changed
	tls = getCurrentTLS();

	// Fiber attached callback
	if (m_Callbacks.m_OnFiberAttached)
		m_Callbacks.m_OnFiberAttached(tls->m_CurrentFiberIndex);

	// The fiber was switched back to here
	// Clean up the previous fiber information
	cleanPreviousFiber();
}


GRAVEngine::Jobs::threadID GRAVEngine::Jobs::jobManager::getCurrentThreadID() const
{
	gravThread* thread = getCurrentThread();
	return (thread == nullptr) ? GRAV_MAX_THREAD_ID : thread->getID();
}
GRAVEngine::Jobs::threadIndex GRAVEngine::Jobs::jobManager::getCurrentThreadIndex() const
{
	if (isValid())
	{
		gravThread* thread = getCurrentThread();
		for (size_t i = 0; i < m_ThreadCount; i++)
			if (&m_Threads[i] == thread)
				return i;
	}
	return GRAV_MAX_THREAD_INDEX;
}
std::wstring GRAVEngine::Jobs::jobManager::getCurrentThreadName() const
{
	gravThread* thread = getCurrentThread();
	return (thread == nullptr) ? L"" : thread->getName();
}

void GRAVEngine::Jobs::jobManager::addReadyFiber(fiberBundle* bundle)
{
	GRAV_PROFILE_FUNCTION();

	// Push a job to the critical queue that starts this now ready fiber
	declaration d;
	d.m_EntryPoint = switchToReadyFiber;
	d.m_Arg = (uintptr_t) bundle;
	d.m_Priority = jobPriority::CRITICAL;

	GRAV_LOG_LINE_INFO("%s: Add the now ready fiber to job queue.", GRAV_CLEAN_FUNC_SIG);
	kickJob(d);
}

GRAVEngine::Jobs::gravThread* GRAVEngine::Jobs::jobManager::getThread(threadIndex index) const
{
	GRAV_ASSERT(isValid());
	GRAV_ASSERT(index < m_ThreadCount);
	return &m_Threads[index];
}
GRAVEngine::Jobs::gravThread* GRAVEngine::Jobs::jobManager::getCurrentThread() const
{
	if (isValid())
	{
		#ifdef GRAV_PLATFORM_WINDOWS
		threadID id = GetCurrentThreadId();
		for (threadIndex i = 0; i < m_ThreadCount; i++)
			if (m_Threads[i].getID() == id)
				return &m_Threads[i];
		#endif
	}
	return nullptr;
}
GRAVEngine::Jobs::tls* GRAVEngine::Jobs::jobManager::getCurrentTLS() const
{
	gravThread* thread = getCurrentThread();
	return (thread == nullptr) ? nullptr : thread->getTLS();
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
void GRAVEngine::Jobs::jobManager::cleanPreviousFiber()
{
	GRAV_PROFILE_FUNCTION();

	tls* tls = getCurrentTLS();

	switch (tls->m_PreviousFiberDestination)
	{
	case fiberDestination::NONE:
		return;
	case fiberDestination::POOL:
		// The previous fiber was being placed into the pool, so state that the fiber is now idle and free
		m_IdleFibers[tls->m_PreviousFiberIndex].store(true, std::memory_order_release);
		tls->m_PreviousFiberDestination = fiberDestination::NONE;
		tls->m_PreviousFiberIndex = GRAV_MAX_FIBER_INDEX;
		break;
	case fiberDestination::WAITNIG:
		// The previous fiber was being placed into a waiting list, so it was stored
		tls->m_PreviousFiberStored->store(true, std::memory_order_release);
		tls->m_PreviousFiberDestination = fiberDestination::NONE;
		tls->m_PreviousFiberIndex = GRAV_MAX_FIBER_INDEX;
		break;
	default:
		break;

	}
}


GRAVEngine::Jobs::jobQueue& GRAVEngine::Jobs::jobManager::getQueue(jobPriority priority)
{
	switch (priority)
	{
	case GRAVEngine::Jobs::jobPriority::LOW:
		return m_LowPriorityQueue;
		break;
	case GRAVEngine::Jobs::jobPriority::NORMAL:
		return m_NormalPriorityQueue;
		break;
	case GRAVEngine::Jobs::jobPriority::HIGH:
		return m_HighPriorityQueue;
		break;
	case GRAVEngine::Jobs::jobPriority::CRITICAL:
		return m_CriticalPriorityQueue;
		break;
	default:
		return m_LowPriorityQueue;
		break;
	}
}
bool GRAVEngine::Jobs::jobManager::getNextJob(job& job)
{
	// Always run critical jobs first
	if (m_CriticalPriorityQueue.dequeue(job))
		return true;

	// TODO: Potentially look into the waiting fiber list or keep the dummy task

	if (m_HighPriorityQueue.dequeue(job))
		return true;
	if (m_NormalPriorityQueue.dequeue(job))
		return true;
	if (m_LowPriorityQueue.dequeue(job))
		return true;

	return false;
}

void GRAVEngine::Jobs::jobManager::fiberCallback(fiber* currentFiber)
{
	jobManager* instance = jobManager::getInstance();

	fiberIndex index = currentFiber->getIndex();
	GRAV_LOG_LINE_INFO("%s: Start Fiber: %i", GRAV_CLEAN_FUNC_SIG, index);
	
	// Fiber attached callback
	if (instance->m_Callbacks.m_OnFiberAttached)
		instance->m_Callbacks.m_OnFiberAttached(index);

	// Fiber initialized callback
	if (instance->m_Callbacks.m_OnFiberInitialized)
		instance->m_Callbacks.m_OnFiberInitialized(index);

	// This fiber is starting. Clean the previous fiber of its resources
	instance->cleanPreviousFiber();

	job job;
	while (instance->isValid())
	{	
		// Get the next job
		if (instance->getNextJob(job))
		{
			// Job Before callback
			if (instance->m_Callbacks.m_OnJobBefore)
				instance->m_Callbacks.m_OnJobBefore(job.m_Declaration);

			// Run the job
			job.m_Declaration.m_EntryPoint(job.m_Declaration.m_Arg);

			// Decrement the counter associated with this job if it exists
			if (job.m_Counter && *job.m_Counter)
				(*job.m_Counter)->decrement();

			// Job After callback
			if (instance->m_Callbacks.m_OnJobAfter)
				instance->m_Callbacks.m_OnJobAfter(job.m_Declaration);
		}
		else
		{
			// No job was found
			switch (instance->m_QueueBehavior)
			{
			case queueBehavior::YIELD:
				std::this_thread::yield();
				break;
			case queueBehavior::SLEEP:
			{
				// Lock the sleeping lock
				std::unique_lock<decltype(instance->m_SleepingThreadLock)> lock(instance->m_SleepingThreadLock);
				instance->m_ThreadCV.wait(lock);
				break;
			}
			case queueBehavior::SPIN:
				GRAVEngine::Jobs::gravThread::sleepFor(1);
				break;
			default:
				break;
			}
		}
	}

	// Fiber initialized callback
	if (instance->m_Callbacks.m_OnFiberEnds)
		instance->m_Callbacks.m_OnFiberEnds(index);

	GRAV_LOG_LINE_INFO("%s: End Fiber: %i", GRAV_CLEAN_FUNC_SIG, index);
	
	// Fiber detached callback
	if (instance->m_Callbacks.m_OnFiberDetached)
		instance->m_Callbacks.m_OnFiberDetached(instance->getCurrentTLS()->m_PreviousFiberIndex, false);

	// Switch back to the calling fiber because the thread's have to die using their personal fibers
	threadIndex currentThread = instance->getCurrentThreadIndex();
	fiber* threadQuitFiber = &instance->m_ThreadQuitFibers[currentThread];
	currentFiber->switchTo(threadQuitFiber);

	// Fiber's switch to their quit equivalent fibers and should never be ran again. As such, this area should never be ran.
	throw Exceptions::fiberCallbackException("Fiber code ran after switching to quit fiber. This code should never be reached!");
}
void GRAVEngine::Jobs::jobManager::fiberQuitCallback(fiber* fiber)
{
	jobManager* instance = jobManager::getInstance();

	// Wait for all of the threads to shut themselves down
	instance->m_ShutdownThreadCount.fetch_add(1, std::memory_order_seq_cst);
	while (instance->m_ShutdownThreadCount.load(std::memory_order_seq_cst) != instance->m_ThreadCount)
		gravThread::sleepFor(50);


	// Switch to the thread's personal fiber, or the first fiber in the array for the starting thread
	threadIndex currentThread = instance->getCurrentThreadIndex();
	if (currentThread == GRAV_MAIN_THREAD_INDEX)
		instance->m_ThreadQuitFibers[currentThread].switchTo(&instance->m_Fibers[GRAV_MAIN_FIBER_INDEX]);
	else
		instance->m_ThreadQuitFibers[currentThread].switchTo(&instance->m_Threads[currentThread].getTLS()->m_Fiber);


	// Fiber's switch to their quit equivalent fibers and should never be ran again. As such, this area should never be ran.
	throw Exceptions::fiberQuitCallbackException("Fiber Quit Callback code ran after switching to threads's personal fiber. This code should never be reached!");
}
void GRAVEngine::Jobs::jobManager::threadCallback(gravThread* gravThread)
{
	jobManager* instance = jobManager::getInstance();

	GRAV_LOG_LINE_INFO("%s: Initialize Thread: %u", GRAV_CLEAN_FUNC_SIG, gravThread->getID());

	// Wait until the job manager is valid
	while (instance->isValid() == false && instance->isShuttingDown() == false);
	{
		GRAV_LOG_LINE_INFO("%s: Thread: %u | Waiting for job manager to become valid.", GRAV_CLEAN_FUNC_SIG, gravThread->getID());
		GRAVEngine::Jobs::gravThread::sleepFor(1);
	}

	// Get the thread's thread local storage
	GRAVEngine::Jobs::tls* tls = gravThread->getTLS();

	// Thread initialized callback
	if (instance->m_Callbacks.m_OnThreadInitialized)
		instance->m_Callbacks.m_OnThreadInitialized(tls->m_ThreadIndex);


	fiberIndex freeFiberIndex = instance->findFreeFiber();


	// Setup the thread's personal fiber. It uses the max fiber index because these are personal fiber's that don't exist in the fiber pool.
	tls->m_Fiber.initializeFromCurrentThread();
	tls->m_CurrentFiberIndex = freeFiberIndex;

	// Switch to the found fiber
	GRAV_LOG_LINE_INFO("%s: Thread: %u | Start fiber", GRAV_CLEAN_FUNC_SIG, gravThread->getID());	
	tls->m_Fiber.switchTo(&instance->m_Fibers[freeFiberIndex]);
	GRAV_LOG_LINE_INFO("%s: Thread: %u | Thread Finished", GRAV_CLEAN_FUNC_SIG, gravThread->getID());

	// Fiber initialized callback
	if (instance->m_Callbacks.m_OnThreadEnds)
		instance->m_Callbacks.m_OnThreadEnds(tls->m_ThreadIndex);
}