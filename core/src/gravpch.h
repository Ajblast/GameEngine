#pragma once

// Utility items
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

// String and output
#include <iostream>
#include <string>
#include <sstream>

// Generic Data Structures
#include <vector>
#include <unordered_map>
#include <unordered_set>

// Include logging


// Check if this is a windows build
#ifdef GRAVCORE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif