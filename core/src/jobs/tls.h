#pragma once
#include <vector>

#include "common.h"
#include "fiber.h"

namespace GRAVEngine
{
	namespace jobs
	{
		typedef uint8 threadIndex;

		enum class fiberDestination : uint8
		{
			NONE, WAITNIG, POOL
		};

		struct tls
		{
			tls() : m_ThreadIndex(UINT8_MAX), m_HasAffinity(false), m_CurrentFiberIndex(UINT16_MAX) {}
			~tls() = default;

			threadIndex m_ThreadIndex;
			bool m_HasAffinity;

			// TODO: Make special threads for IO
			//bool m_IsIOThread = false;

			// Fiber of the thread
			fiber m_Fiber;

			// Current fiber
			fiberIndex m_CurrentFiberIndex;


			// Previous thread fiber
			uint16 m_PreviousFiberIndex = UINT16_MAX;
			//std::atomic_bool* m_PreviousFiberInWaitingList = nullptr;
			// Where was the previous fiber going to be placed
			fiberDestination m_PreviousFiberDestination = fiberDestination::NONE;

			// Fibers in the wait list that are now ready to be ran
			//std::vector<std::pair<fiberIndex, std::atomic_bool*>> m_ReadyFibers;

			// Clean this TLS. Remove previous fiber index and destination
			inline void clean()
			{
				m_PreviousFiberIndex = UINT16_MAX;
				m_PreviousFiberDestination = fiberDestination::NONE;
				//m_PreviousFiberInWaitingList = nullptr;
			}
		};
	}
}