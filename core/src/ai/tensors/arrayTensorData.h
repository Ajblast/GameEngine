#pragma once

#include "common.h"
#include "ITensorData.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Tensors
		{
			class GRAVAPI arrayTensorData : public ITensorData
			{
			public:
				arrayTensorData(size_t count);
				arrayTensorData(tensorShape shape);
				virtual ~arrayTensorData();

				// Reserve a certain amount of data in the internal storage
				virtual void reserve(size_t count) override;
				// Upload data to the internal storage. Ignores extra data
				virtual void upload(float* data, tensorShape shape) override;
				// Get the data from the internal storage. Ignores extra data
				virtual void download(float*& data, tensorShape shape) override;
				// Get the data capacity
				virtual const size_t capacity() const override;

			private:
				scope<float[]> m_Data;
				size_t m_Shape;
			};
		}
	}
}