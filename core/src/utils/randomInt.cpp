#include "gravpch.h"
#include "randomInt.h"

// Get a sample of random integers. A single item vector is returned if no sampleSize is given
std::vector<GRAVEngine::int32> GRAVEngine::randomInt(int32 min, int32 max, size_t sampleSize)
{
	std::vector<int32> retVal;

	std::random_device rd;									// Hardware device for the number generator
	std::mt19937 gen(rd());									// Generator to generate the random values
	std::uniform_int_distribution<int32> dist(min, max);	// Distribution to use

															// Generate the sample size of values
	for (size_t i = 0; i < sampleSize; i++)
		retVal.push_back(dist(gen));

	return retVal;
}

// Get a sample of random integers. A single item vector is returned if no sampleSize is given
std::vector<size_t> GRAVEngine::randomInt(size_t min, size_t max, size_t sampleSize)
{
	std::vector<size_t> retVal;

	std::random_device rd;									// Hardware device for the number generator
	std::mt19937 gen(rd());									// Generator to generate the random values
	std::uniform_int_distribution<size_t> dist(min, max);	// Distribution to use

															// Generate the sample size of values
	for (size_t i = 0; i < sampleSize; i++)
		retVal.push_back(dist(gen));

	return retVal;
}
