#pragma once

#include "platform.h"

#define MAX_EXCEPTION_STRING_LENGTH 8192
#define MAX_LOG_BUFFER_SIZE 8192
#define MAX_PATH_LENGTH 260

#ifndef DISABLE_ASSERTIONS
#define ASSERTIONS_ENABLED
#endif

#define BIT(x) (1 << x)

#define GRAV_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


#ifdef GRAV_PLATFORM_WINDOWS
#ifdef GRAV_DLL
#define GRAVAPI __declspec(dllexport)
#else
#define GRAVAPI __declspec(dllimport)
#endif
#endif


#include "memory/pointers/pointers.h"
#include "types/dataPrimitives.h"
#include "assertion.h"
#include "errorCodes.h"
#include "logging/logging.h"
