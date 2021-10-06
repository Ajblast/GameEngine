#pragma once

#include "common.h"
#include "observationParams.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Sensors
		{
			// Abstract writer for an observation
			class GRAVAPI observationWriter
			{
			public:
				observationWriter(observationParams params);
				virtual ~observationWriter() = default;

				virtual void write(float* data, size_t count) = 0;
			protected:
				observationParams m_Params;
			};
		}
	}
}