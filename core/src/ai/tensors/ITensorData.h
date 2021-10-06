#pragma once

#include "common.h"
#include "tensorShape.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Tensors
		{
			// Interface on how to store tensor data
			class GRAVAPI ITensorData
			{
			public:
				ITensorData() {}
				virtual ~ITensorData() {}

			public:
				// Reserve a certain amount of data in the internal storage
				virtual void reserve(size_t count) = 0;
				// Upload data to the internal storage.
				virtual void upload(float* data, tensorShape shape) = 0;
				// Get the data from the internal storage. Could be async.
				virtual void download(float*& data, tensorShape shape) = 0;

				// Get the data capacity
				virtual const size_t capacity() const = 0;
			};
		}
	}
}