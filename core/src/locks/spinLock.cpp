#include "gravpch.h"
#include "spinLock.h"

bool GRAVEngine::Locks::spinLock::try_lock()
{
    // Use an acquire fence to ensure all subsequent reads by this thread will be valid
    bool alreadyLocked = m_atomic.test_and_set(std::memory_order_acquire);

    return !alreadyLocked;
}

void GRAVEngine::Locks::spinLock::lock()
{
    // Spin until successful acquire
    while (!try_lock())
    {
        // Yield this thread
        std::this_thread::yield();
    }
}

void GRAVEngine::Locks::spinLock::unlock()
{
    // Use release semantics to ensure that all prior write have been fully committed before unlocking
    m_atomic.clear(std::memory_order_release);
}
