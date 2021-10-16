#pragma once

#include "common.h"
#include <unordered_map>
#include <vector>
#include <string>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			namespace Stats
			{
				class GRAVAPI stats
				{
				public:
					stats();

					// Create a stat to put in the map
					void createStat(const std::string& key);

					// Add a statistic
					void addStat(const std::string& key, float value);

					// Write the statistics out
					void write(const std::string& filePath);
				private:
					std::map<std::string, std::vector<float>> m_Stats;	// Stats
				};
			}
		}
	}
}