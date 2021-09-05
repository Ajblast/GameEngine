#include "gravpch.h"
#include "episodeCounter.h"

std::atomic<GRAVEngine::uint32> GRAVEngine::AI::episodeCounter::s_Counter = 0;

// Get the current episode ID. Increases the id by 1
const GRAVEngine::uint32 GRAVEngine::AI::episodeCounter::episodeId() { return s_Counter.fetch_add(1); }
