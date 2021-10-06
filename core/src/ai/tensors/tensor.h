#pragma once

#include "common.h"
#include "ITensorData.h"
#include "tensorShape.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Tensors
		{
			class GRAVAPI tensor
			{
			public:
				tensor(tensorShape shape, std::string name = "");
				tensor(tensorShape shape, float* data, std::string name = "");
				tensor(tensorShape shape, scope<ITensorData>& data, std::string name= "");
				tensor(const tensor& other) = delete;
				~tensor();

				tensor(tensor&& other) noexcept;

				tensor flatten();
				tensor reshape(tensorShape shape);

				//float& operator[](size_t index);
				//const float operator[](size_t index) const;

				void set(float value, size_t b, size_t c);
				void set(float value, size_t b, size_t h, size_t w, size_t c);

				float at(size_t b, size_t c);
				float at(size_t b, size_t h, size_t w, size_t c);
				
				std::string name() { return m_Name; }
				tensorShape shape();
			private:
				std::string m_Name;
				tensorShape m_Shape;
				scope<ITensorData> m_Data;
			};
		}
	}
}