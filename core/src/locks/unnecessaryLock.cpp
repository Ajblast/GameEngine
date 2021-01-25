#include "gravpch.h"
#include "unnecessaryLock.h"

void GRAVEngine::Locks::unnecessaryLock::acquire()
{
	// Assert no one already has the lock
	GRAV_ASSERT(!m_locked);

	// Now lock So that overlapping critical operations are detected
	m_locked = true;
}

void GRAVEngine::Locks::unnecessaryLock::release()
{
	// Assert correct usage, that release is only called after acquire
	GRAV_ASSERT(m_locked);

	// Unlock
	m_locked = false;
}
