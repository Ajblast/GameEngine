#pragma once

#include "common.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Distributions
		{
			class distribution
			{
			public:
				distribution(c10::IntArrayRef batchShape) : m_BatchShape(batchShape.begin(), batchShape.end()) {}
				virtual ~distribution() = default;
			public:
				virtual torch::Tensor mean() = 0;
				virtual torch::Tensor variance() = 0;
				virtual torch::Tensor stddev() = 0;

				virtual torch::Tensor sample(c10::IntArrayRef shape) = 0;
				virtual torch::Tensor logProb(torch::Tensor value) = 0;
				//virtual torch::Tensor sdf(torch::Tensor value) = 0;
				virtual torch::Tensor entropy() = 0;

				// Get the extended shape from a sample. Returns an std::vector instead of an IntArrayRef because the ref requires the data to outlive the ref
				std::vector<int64_t> _extended_shape(c10::IntArrayRef sampleShape = c10::IntArrayRef())
				{
					std::vector<int64_t> retVal = std::vector<int64_t>(sampleShape.begin(), sampleShape.end());
					retVal.insert(retVal.end(), m_BatchShape.begin(), m_BatchShape.end());
					return retVal;
				}

				inline c10::IntArrayRef batchShape() { return c10::IntArrayRef(m_BatchShape); }
			private:
				std::vector<int64_t> m_BatchShape;
			};
		}
	}
}