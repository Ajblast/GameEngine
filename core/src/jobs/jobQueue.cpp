#include "gravpch.h"
#include "jobQueue.h"

GRAVEngine::jobs::jobQueue::jobQueue(size_t maxSize) : m_MaxSize(maxSize) {}
GRAVEngine::jobs::jobQueue::jobQueue(const jobQueue& other) : m_MaxSize(other.m_MaxSize), m_Queue(other.m_Queue) {}
GRAVEngine::jobs::jobQueue::jobQueue(jobQueue&& other) noexcept : m_MaxSize(other.m_MaxSize), m_Queue(other.m_Queue) {}
GRAVEngine::jobs::jobQueue& GRAVEngine::jobs::jobQueue::operator=(const jobQueue& other)
{
    if (this != &other)
    {
        m_Queue = other.m_Queue;
        m_MaxSize = other.m_MaxSize;
    }

    return *this;
}
GRAVEngine::jobs::jobQueue& GRAVEngine::jobs::jobQueue::operator=(jobQueue&& other) noexcept
{
    if (this != &other)
    {
        m_Queue = other.m_Queue;
        m_MaxSize = other.m_MaxSize;
    }

    return *this;
}

bool GRAVEngine::jobs::jobQueue::enqueue(const declaration& data)
{
    GRAVEngine::Locks::scopedLock<decltype(m_Lock)> lock();

    if (m_Queue.size() >= m_MaxSize)
        return false;

    // Push the data
    m_Queue.push(data);
    return true;
}

bool GRAVEngine::jobs::jobQueue::dequeue(declaration& data)
{
    GRAVEngine::Locks::scopedLock<decltype(m_Lock)> lock();

    if (m_Queue.empty())
        return false;

    // Copy the front of the queue
    data = m_Queue.front();

    m_Queue.pop();
    return true;
}
