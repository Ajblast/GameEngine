#pragma once

#include "actionBuffer.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Actions
		{
			class IHeuristicProvider
			{
			public:
				IHeuristicProvider() {}
				virtual ~IHeuristicProvider() {}

				virtual void heuristic(actionBuffer& buffers) = 0;
			};
		}
	}
}