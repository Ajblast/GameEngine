#pragma once

#include "common.h"
#include "ai/sensors/observationParams.h"
#include "ai/actions/actionSpec.h"
#include <vector>
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			// Settings for how to construct a network
			struct networkSettings
			{
			public:
				networkSettings() {}
				networkSettings(int64 layerCount, int64 hiddenCount, std::vector<Sensors::observationParams> observationSpecs, AI::Actions::actionSpec actionSpec) :
					m_LayerCount(layerCount), m_HiddenCount(hiddenCount), m_ObservationSpecs(observationSpecs), m_ActionSpec(actionSpec)
				{}
				networkSettings(const networkSettings& other) :
					m_LayerCount(other.m_LayerCount), m_HiddenCount(other.m_HiddenCount), m_ObservationSpecs(other.m_ObservationSpecs), m_ActionSpec(other.m_ActionSpec)
				{}
				networkSettings(networkSettings&& other) noexcept :
					m_LayerCount(other.m_LayerCount), m_HiddenCount(other.m_HiddenCount), m_ObservationSpecs(std::move(other.m_ObservationSpecs)), m_ActionSpec(std::move(other.m_ActionSpec)) 
				{}
				networkSettings& operator=(const networkSettings& other) noexcept
				{
					if (this != &other)
					{
						m_LayerCount = other.m_LayerCount;
						m_HiddenCount = other.m_HiddenCount;
						m_ObservationSpecs = other.m_ObservationSpecs;
						m_ActionSpec = other.m_ActionSpec;
					}
					return *this;
				}
				networkSettings& operator=(networkSettings&& other) noexcept
				{
					if (this != &other)
					{
						m_LayerCount = other.m_LayerCount;
						m_HiddenCount = other.m_HiddenCount;
						m_ObservationSpecs = std::move(other.m_ObservationSpecs);
						m_ActionSpec = std::move(other.m_ActionSpec);
					}
					return *this;
				}
			public:
				int64 m_LayerCount = 2;										// Number of layers
				int64 m_HiddenCount = 128;									// Number of hidden nodes in a layer
				std::vector<Sensors::observationParams> m_ObservationSpecs;	// The input observation specs
				AI::Actions::actionSpec m_ActionSpec;						// The output action spec
			};
		}
	}
}
