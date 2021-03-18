#pragma once

#include "platform.h"

#define MAX_EXCEPTION_STRING_LENGTH 1024
#define MAX_LOG_BUFFER_SIZE 1024
#define MAX_PATH_LENGTH 260

#ifndef DISABLE_ASSERTIONS
#define ASSERTIONS_ENABLED
#endif

#define BIT(x) (1 << x)

#define GRAV_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// TODO: Change this new to allow tracking of memory allocations
// TODO: Get rid of this function and the const char* and just use std::string. Why did I even use them in the first place??
#define STRING_COPY(destination, source, maxSize) \
size_t stringLength = strlen(source) + 1; \
stringLength = std::min(stringLength, (size_t)maxSize); \
\
char* tmp = new char[stringLength]; \
strncpy(tmp, source, stringLength); \
tmp[stringLength - 1] = 0; \
destination = tmp

#include "memory/pointers/pointers.h"
#include "dataPrimitives.h"
#include "assertion.h"
#include "errorCodes.h"
#include "logging/logging.h"
