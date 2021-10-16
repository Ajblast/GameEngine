#pragma once

#include "common.h"
#include "algorithmType.h"
#include "networkSettings.h"
#include <string>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			// Settings for when creating a trainer
			struct GRAVAPI trainerSettings
			{
			public:
				trainerSettings() {}
				trainerSettings(const std::string& programName, algorithmType algorithm, networkSettings settings, size_t trajectoryHorizon, const std::string& statsFolder, size_t summaryFrequency) :
					m_ProgramName(programName), m_Algorithm(algorithm), m_NetworkSettings(settings), m_TrajectoryHorizon(trajectoryHorizon), m_StatsFolder(statsFolder), m_SummaryFrequency(summaryFrequency)
				{}
				trainerSettings(const trainerSettings& other) : 
					m_ProgramName(other.m_ProgramName), m_Algorithm(other.m_Algorithm), m_NetworkSettings(other.m_NetworkSettings), m_TrajectoryHorizon(other.m_TrajectoryHorizon), m_StatsFolder(other.m_StatsFolder), m_SummaryFrequency(other.m_SummaryFrequency)
				{}
				trainerSettings(trainerSettings&& other) noexcept :
					m_ProgramName(std::move(other.m_ProgramName)), m_Algorithm(other.m_Algorithm), m_NetworkSettings(std::move(other.m_NetworkSettings)), m_TrajectoryHorizon(other.m_TrajectoryHorizon), m_StatsFolder(std::move(other.m_StatsFolder)), m_SummaryFrequency(other.m_SummaryFrequency)
				{}
				trainerSettings& operator=(const trainerSettings& other)
				{
					if (this != &other)
					{
						m_ProgramName = other.m_ProgramName;
						m_Algorithm = other.m_Algorithm;
						m_NetworkSettings = other.m_NetworkSettings;
						m_TrajectoryHorizon = other.m_TrajectoryHorizon;
						m_StatsFolder = other.m_StatsFolder;
						m_SummaryFrequency = other.m_SummaryFrequency;
					}
					return *this;
				}
				trainerSettings& operator=(trainerSettings&& other) noexcept
				{
					if (this != &other)
					{
						m_ProgramName = std::move(other.m_ProgramName);
						m_Algorithm = other.m_Algorithm;
						m_NetworkSettings = std::move(other.m_NetworkSettings);
						m_TrajectoryHorizon = other.m_TrajectoryHorizon;
						m_StatsFolder = std::move(other.m_StatsFolder);
						m_SummaryFrequency = other.m_SummaryFrequency;
					}
					return *this;
				}
			public:
				std::string m_ProgramName = "Behavior Name";	// Name of the program to train
				std::string m_StatsFolder;						// Folder where to write statistics
				algorithmType m_Algorithm = algorithmType::PPO;	// Type of algorithm to use
				networkSettings m_NetworkSettings;				// Settings for network creation
				size_t m_TrajectoryHorizon = 64;				// The max length of a trajectory
				size_t m_SummaryFrequency = 10000;				// How often does a trainer need to save stats and model
			};
		}
	}
}