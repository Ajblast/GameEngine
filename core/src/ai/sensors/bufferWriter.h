#pragma once

#include "common.h"
#include "observationWriter.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Sensors
		{
			// Writes to a buffer
			class GRAVAPI bufferWriter : public observationWriter
			{
			public:
				bufferWriter(float* data, size_t size);

			private:
				float* m_Data;
				size_t m_Size;

				// Inherited via observationWriter
				virtual void write(float* data, size_t count) override;
			};
		}
	}
}