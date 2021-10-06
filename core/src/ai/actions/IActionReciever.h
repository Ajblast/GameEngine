#pragma once

#include "common.h"
#include "actionBuffer.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Actions
		{
			class GRAVAPI IActionReciever
			{
			public:
				IActionReciever() {}
				virtual ~IActionReciever() {}

				virtual void onActionRecieved(actionBuffer buffers) = 0;
			};
		}
	}
}