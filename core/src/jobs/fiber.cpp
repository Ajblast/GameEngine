#include "gravpch.h"
#include "fiber.h"
#include "exceptions/concurrency/fiberCreationException.h"
#include "exceptions/concurrency/fiberToThreadException.h"
#include "exceptions/concurrency/fiberSwitchException.h"

GRAVEngine::Jobs::fiber::fiber() : m_FiberHandle(nullptr), m_Index(0),m_IsThreadFiber(false)
{
}
void GRAVEngine::Jobs::fiber::spawn(fiberCallback callback)
{
	#ifdef GRAV_PLATFORM_WINDOWS
	// Create the fiber with the launching fiber function
	m_FiberHandle = (fiberHandle)CreateFiber(0, (LPFIBER_START_ROUTINE)callback, this);
	m_IsThreadFiber = false;
	#endif

	if (m_FiberHandle == NULL)
		throw Exceptions::fiberCreationException("Unable to spawn fiber.");
}
GRAVEngine::Jobs::fiber::~fiber()
{
	#ifdef GRAV_PLATFORM_WINDOWS
	// Delete the system fiber
	if (m_FiberHandle && m_IsThreadFiber == false)
		DeleteFiber(m_FiberHandle);
	#endif
}

void GRAVEngine::Jobs::fiber::initializeFromCurrentThread()
{
	#ifdef GRAV_PLATFORM_WINDOWS
	// Delete the current system fiber if there is one
	if (m_FiberHandle && m_IsThreadFiber == false)
		DeleteFiber(m_FiberHandle);

	// Convert this thread to a fiber
	m_FiberHandle = ConvertThreadToFiber(nullptr);
	m_IsThreadFiber = true;
	#endif

	// Was a handle created
	if (m_FiberHandle == NULL)
		throw Exceptions::fiberCreationException("Unable to create fiber from current thread.");
}
void GRAVEngine::Jobs::fiber::convertToThread()
{
	bool result;
	#ifdef GRAV_PLATFORM_WINDOWS
	// Turn this fiber back into a thread. Used for the main thread
	result = ConvertFiberToThread();
	m_FiberHandle = NULL;
	m_IsThreadFiber = false;
	#endif

	// Was the fiber turned back into a thread
	if (result == false)
		throw Exceptions::fiberToThreadException("Unable to convert the current fiber into a thread.");
}

void GRAVEngine::Jobs::fiber::switchTo(fiber* fiber)
{
	GRAV_ASSERT(fiber)
	GRAV_ASSERT(fiber->m_FiberHandle);

	#ifdef GRAV_PLATFORM_WINDOWS
	try
	{
		SwitchToFiber(fiber->m_FiberHandle);
	}
	catch (...)
	{
		GRAV_LOG_LINE_CRITICAL("%s: Unable to switch to fiber!!!!", GRAV_CLEAN_FUNC_SIG);
	}
	#endif
}