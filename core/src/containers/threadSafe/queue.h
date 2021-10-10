#pragma once

#include "common.h"
#include "locks/scopedLock.h"
#include "locks/multiLock.h"
#include <queue>
#include <mutex>

namespace GRAVEngine
{
	namespace Jobs
	{
		template<typename T, typename L>
		class queue
		{
		public:
			queue();
			queue(size_t maxSize);
			queue(const queue& other);
			queue(queue&& other) noexcept;
			queue& operator=(const queue& other);
			queue& operator=(queue&& other) noexcept;

			~queue() = default;

			bool enqueue(const T& data);
			bool dequeue(T& data);
		private:
			L m_Lock;
			std::queue<T> m_Queue;
			size_t m_MaxSize;
		};

		template<typename T, typename L>
		GRAVEngine::Jobs::queue<T, L>::queue() : queue<T, L>(0)
		{
		}
		template<typename T, typename L>
		GRAVEngine::Jobs::queue<T, L>::queue(size_t maxSize) : m_MaxSize(maxSize)
		{
		}
		template<typename T, typename L>
		GRAVEngine::Jobs::queue<T, L>::queue(const queue& other) : m_MaxSize(other.m_MaxSize)
		{
			// Lock the other queue as we are accessing its data
			GRAVEngine::Locks::scopedLock<decltype(other.m_Lock)> lock(other.m_Lock);

			// Copy of the items in the other queue
			m_Queue = std::queue<T>(other.m_Queue);
		}
		template<typename T, typename L>
		queue<T, L>::queue(queue&& other) noexcept : m_MaxSize(other.m_MaxSize)
		{
			// Lock the other queue as we are accessing its data
			GRAVEngine::Locks::scopedLock<decltype(other.m_Lock)> lock(other.m_Lock);

			// Move all of the items in the other queue
			m_Queue = std::move(other.m_Queue);
		}
		template<typename T, typename L>
		queue<T, L>& queue<T, L>::operator=(const queue& other)
		{
			if (this != &other)
			{
				Locks::multiLock<decltype(m_Lock), decltype(other.m_Lock)> lock(m_Lock, other.m_Lock);
				lock.lock();

				m_Queue = other.m_Queue;
				m_MaxSize = other.m_MaxSize;
				lock.unlock();
			}

			return *this;
		}
		template<typename T, typename L>
		queue<T, L>& queue<T, L>::operator=(queue&& other) noexcept
		{
			if (this != &other)
			{
				Locks::multiLock<decltype(m_Lock), decltype(other.m_Lock)> lock(m_Lock, other.m_Lock);
				lock.lock();

				m_Queue = std::move(other.m_Queue);
				m_MaxSize = std::move(other.m_MaxSize);

				lock.unlock();
			}

			return *this;
		}
		template<typename T, typename L>
		bool queue<T, L>::enqueue(const T& data)
		{
			GRAVEngine::Locks::scopedLock<decltype(m_Lock)> lock(m_Lock);

			if (m_Queue.size() >= m_MaxSize)
				return false;

			// Push the data
			m_Queue.push(data);
			return true;
		}
		template<typename T, typename L>
		bool queue<T, L>::dequeue(T& data)
		{
			GRAVEngine::Locks::scopedLock<decltype(m_Lock)> lock(m_Lock);

			if (m_Queue.empty())
				return false;

			// Copy the front of the queue
			data = m_Queue.front();

			m_Queue.pop();
			return true;
		}
	}
}