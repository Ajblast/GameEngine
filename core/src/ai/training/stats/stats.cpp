#include "gravpch.h"
#include "stats.h"
#include "io/files/textFile.h"
#include <sstream>
#include "torch/torch.h"

GRAVEngine::AI::Training::Stats::stats::stats()
{
}

void GRAVEngine::AI::Training::Stats::stats::createStat(const std::string& key)
{
	m_Stats.emplace(key, std::vector<float>());
}

void GRAVEngine::AI::Training::Stats::stats::addStat(const std::string& key, float value)
{
	// Create the pair if needed
	if (m_Stats.find(key) == m_Stats.end())
		createStat(key);

	// Add the value
	m_Stats[key].push_back(value);
}

void GRAVEngine::AI::Training::Stats::stats::write(const std::string& filePath)
{
	std::stringstream ss;

	if (IO::exists(filePath) == false)
	{
		// Write column names
		for (auto it = m_Stats.begin(); it != m_Stats.end(); it++)
		{
			if (it != m_Stats.begin())
				ss << ",";

			// Write the name
			ss << it->first;
		}
		ss << "\n";
	}


	for (auto it = m_Stats.begin(); it != m_Stats.end(); it++)
	{
		// Write out he comma if not the last item
		if (it != m_Stats.begin())
			ss << ",";
		
		float average = it->second.size() == 0 ? 0 : torch::tensor(it->second).mean().item<float>();
		ss << average;
	}
	ss << "\n";

	// Remove all the current stats
	for (auto it = m_Stats.begin(); it != m_Stats.end(); it++)
	{
		it->second.clear();
	}

	// Open the file and write
	GRAVEngine::IO::textFile file = GRAVEngine::IO::textFile(filePath, GRAVEngine::IO::fileMode::INPUT | GRAVEngine::IO::fileMode::OUTPUT |  GRAVEngine::IO::fileMode::APPEND, true);
	file.write(ss.str());
}
