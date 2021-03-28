#pragma once

#include <assert.h>

// TODO: Dynamic assertion enabling
#define ASSERTIONS_ENABLED

#ifdef ASSERTIONS_ENABLED

//#define debugBreak() asm {int 3}

// Define inline assembly code that causes a break into the debugger -- Changes depending on target CPU
#define debugBreak() __debugbreak()


// Check the expression and fail if it is false
#define GRAV_ASSERT(expr) \
	if (expr) {} \
	else \
	{ \
			GRAVEngine::Logging::reportAssertionFailure(#expr, \
				__FILE__, __LINE__) ; \
			debugBreak(); \
	}
#define GRAV_ASSERT_LOGLESS(expr) \
	if (expr) {} \
	else \
	{ \
			debugBreak(); \
	}
// Check if this a debug build
#ifdef GRAVCORE_DEBUG
#define GRAV_SLOW_ASSERT(expr) \
	if (expr) {} \
	else \
	{ \
		GRAVEngine::Logging::reportAssertionFailure(#expr, \
			__FILE__, __LINE__); \
		debugBreak(); \
	}
#define GRAV_SLOW_ASSERT_LOGLESS(expr) \
	if (expr) {} \
	else \
	{ \
		debugBreak(); \
	}
#else
#define GRAV_SLOW_ASSERT(expr)
#define GRAV_SLOW_ASSERT_LOGLESS(expr)
#endif

#define GRAV_STATIC_ASSERT(expr) static_assert(expr);

#else

// Evaluate to nothing
#define GRAV_ASSERT(expr)
#define GRAV_ASSERT_LOGLESS(expr)
#define GRAV_SLOW_ASSERT_LOGLESS(expr)

#endif

// TODO: Implement exceptions to code base.