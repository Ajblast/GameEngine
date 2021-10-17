#include "gravpch.h"
#include "time.h"

std::atomic<float> GRAVEngine::Time::time::s_DeltaTime = 0;
std::atomic<float> GRAVEngine::Time::time::s_OldTime = 0;