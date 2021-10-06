#pragma once

#include "common.h"
#include "observationWriter.h"
#include "ai/tensors/tensor.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Sensors
		{
			// Writes to a tensor
			class GRAVAPI tensorWriter : public observationWriter
			{
			public:
				tensorWriter(Tensors::tensor tensor);

			private:
				Tensors::tensor m_Tensor;

				// Inherited via observationWriter
				virtual void write(float* data, size_t count) override;
			};
		}
	}
}