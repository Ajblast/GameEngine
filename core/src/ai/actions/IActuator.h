#pragma once

#include "common.h"
#include "IHeuristicProvider.h"
#include "IActionReciever.h"
#include "actionSpec.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Actions
		{
			class IActuator : public IHeuristicProvider, public IActionReciever
			{
			public:
				IActuator() {}
				virtual ~IActuator() {}

				virtual std::string getName() const = 0;
				virtual void reset() = 0;
				virtual actionSpec getActionSpec() = 0;

				inline bool operator= (const IActuator& other) const { return getName().compare(other.getName()) == 0; }
			};
		}
	}
}