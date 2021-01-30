#pragma once

#include "common.h"
#include "declaration.h"
#include "locks/locks.h"
#include <queue>

namespace GRAVEngine
{
	namespace Jobs
	{
		class jobQueue
		{
		public:
			jobQueue();
			jobQueue(size_t maxSize);
			jobQueue(const jobQueue& other);
			jobQueue(jobQueue&& other) noexcept;
			jobQueue& operator=(const jobQueue& other);
			jobQueue& operator=(jobQueue&& other) noexcept;

			~jobQueue() = default;

			bool enqueue(const declaration& data);
			bool dequeue(declaration& data);
		private:
			GRAVEngine::Locks::spinLock m_Lock;
			std::queue<declaration> m_Queue;
			size_t m_MaxSize;
		};
	}
}