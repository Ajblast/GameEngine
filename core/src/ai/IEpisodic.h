#pragma once

#include "common.h"
#include "doneReason.h"

namespace GRAVEngine
{
	namespace AI
	{
		class GRAVAPI IEpisodic
		{
		public:
			IEpisodic() = default;
			virtual ~IEpisodic() = default;

			virtual void onEpisodeBegin() = 0;
			virtual void onEpisodeEnd() = 0;
			virtual uint32 completedEpisdes() = 0;
			virtual void endEpisode() = 0;
			// Same as endEpisode, but not the agent's fault. Affects Training
			virtual void interuptEpisode() = 0;
			virtual void endEpisodeAndReset(doneReason reason) = 0;
		};
	}
}