#pragma once

#include "common.h"
#include <atomic>

namespace GRAVEngine
{
	namespace AI
	{
		// Static class that holds the current episode id.
		class GRAVAPI episodeCounter
		{
		public:
			// Get the current episode ID. Increases the id by 1
			static const uint32 episodeId();
		private:
			episodeCounter();
		private:
			static std::atomic<uint32> s_Counter;	// Atomic counter for the episode id.
		};
	}
}