#pragma once

#include "common.h"
#include "actionBufferSegment.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Actions
		{
			// Class that holds the continuous and discrete actions for an IActionReciever
			class actionBuffer
			{
			public:
				actionBuffer();
				// Create an action buffer with buffer segments. Must not be null
				actionBuffer(ref<actionBufferSegment<float>> continuousActions, ref<actionBufferSegment<int32>> discreteActions);
				actionBuffer(const actionBuffer& other);
				actionBuffer(const actionBuffer&& other) noexcept;

				actionBuffer& operator=(const actionBuffer& other);
				actionBuffer& operator=(const actionBuffer&& other) noexcept;

				void copy(actionBuffer buffer);

				const ref<actionBufferSegment<float>>& continuousActions() { return m_ContinuousActions; }
				const ref<actionBufferSegment<int32>>& discreteActions() { return m_DiscreteActions; }

				// Clear this action buffer
				inline void clear() { m_ContinuousActions->clear(); m_DiscreteActions->clear(); }
				// Is this action buffer empty?
				inline const bool isEmpty() const { return m_ContinuousActions->isEmpty() && m_DiscreteActions->isEmpty(); }
			private:
				ref<actionBufferSegment<float>> m_ContinuousActions;
				ref<actionBufferSegment<int32>> m_DiscreteActions;
			};
		}
	}
}