#include "gravpch.h"
#include "fiber.h"

// Launch a fiber. Calls the fiber's callback function
static void launchFiber(GRAVEngine::jobs::fiber* fiber)
{
	// Get the function's callback function
	auto callback = fiber->getCallback();
	GRAV_ASSERT(callback);

	// Call the fiber's function
	callback(fiber);
}

GRAVEngine::jobs::fiber::fiber() : m_FunctionCallback(nullptr), m_CallingFiber(nullptr)
{
#ifdef _WIN32
	// Create the fiber with the launching fiber function
	m_FiberHandle = (fiberHandle) CreateFiber(0, (LPFIBER_START_ROUTINE) launchFiber, this);
	m_IsThreadFiber = false;
#endif
}

//GRAVEngine::jobs::fiber::fiber(fiberHandle fiber) : m_FiberHandle(fiber)
//{
//}

GRAVEngine::jobs::fiber::~fiber()
{
#ifdef _WIN32
	// Delete the system fiber
	if (m_FiberHandle && m_IsThreadFiber == false)
		DeleteFiber(m_FiberHandle);
#endif
}

void GRAVEngine::jobs::fiber::initializeFromCurrentThread()
{
#ifdef _WIN32
	// Delete the current system fiber if there is one
	if (m_FiberHandle && m_IsThreadFiber == false)
		DeleteFiber(m_FiberHandle);

	// Convert this thread to a fiber
	m_FiberHandle = ConvertThreadToFiber(nullptr);
	m_IsThreadFiber = true;
#endif
}

void GRAVEngine::jobs::fiber::switchTo(fiber* fiber)
{
	GRAV_ASSERT(fiber)
	GRAV_ASSERT(fiber->m_FiberHandle);

	// Switch to a fiber and pass it the given data
	fiber->m_CallingFiber = this;

#ifdef _WIN32
	SwitchToFiber(fiber->m_FiberHandle);
#endif
}

void GRAVEngine::jobs::fiber::switchToCallingFiber()
{
	GRAV_ASSERT(m_CallingFiber);
	GRAV_ASSERT(m_CallingFiber->m_FiberHandle);

	// Switch to the calling fiber
#ifdef _WIN32
	SwitchToFiber(m_CallingFiber->m_FiberHandle);
#endif
}

void GRAVEngine::jobs::fiber::setCallback(fiberFunction callback)
{
	m_FunctionCallback = callback;
}