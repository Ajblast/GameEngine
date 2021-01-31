#pragma once

#define MAX_EXCEPTION_STRING_LENGTH 1024
#define MAX_LOG_BUFFER_SIZE 1024
#define MAX_PATH_LENGTH 260

#ifndef DISABLE_ASSERTIONS
#define ASSERTIONS_ENABLED
#endif

// TODO: Change this new to allow tracking of memory allocations

#define STRING_COPY(destination, source, maxSize) \
size_t stringLength = strlen(source) + 1; \
stringLength = std::min(stringLength, (size_t)maxSize); \
\
char* tmp = new char[stringLength]; \
strncpy(tmp, source, stringLength); \
tmp[stringLength - 1] = 0; \
destination = tmp

#include "platform.h"
#include "dataPrimitives.h"
#include "assertion.h"
#include "errorCodes.h"
#include "logging/logging.h"
#include "time/time.h"
