#pragma once

#include "common.h"
#include <random>
#include <vector>

namespace GRAVEngine
{
	// Get a sample of random integers. A single item vector is returned if no sampleSize is given
	GRAVAPI std::vector<int32> randomInt(int32 min, int32 max, size_t sampleSize = 1);
	// Get a sample of random integers. A single item vector is returned if no sampleSize is given
	GRAVAPI std::vector<size_t> randomInt(size_t min, size_t max, size_t sampleSize = 1);
}