#pragma once

#define ASSERTIONS_ENABLED

#ifdef ASSERTIONS_ENABLED

//#define debugBreak() asm {int 3}

// Define inline assembly code that causes a break into the debugger -- Changes depending on target CPU
#define debugBreak() __debugbreak()


// Check the expression and fail if it is false
#define ASSERT(expr) \
	if (expr) {} \
	else \
	{ \
			reportAssertionFailure(#expr, \
				__FILE__, __LINE__) ; \
			debugBreak(); \
	}

// Check if this a debug build
#ifdef GRAVCORE_DEBUG
#define SLOW_ASSERT(expr) \
	if (expr) {} \
	else \
	{ \
		reportAssertionFailure(#expr, \
			__FILE__, __LINE__); \
		debugBreak(); \
	}
#else
#define SLOW_ASSERT(expr)
#endif

#else

// Evaluate to nothing
#define ASSERT(expr)
#define SLOW_ASSERT(expr)

#endif
