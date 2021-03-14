#pragma once

// Utility items
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <thread>
#include <cstdarg>
#include <condition_variable>

// String and output
#include <iostream>
#include <string>
#include <sstream>

// Generic Data Structures
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>

// Include common items
#include "common.h"


// Check if this is a windows build
#ifdef GRAVCORE_PLATFORM_WINDOWS
	#define NOMINMAX
	#include <Windows.h>
#endif