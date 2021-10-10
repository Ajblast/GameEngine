#pragma once

#include "common.h"

#include "ai/agentException.h"

#include "concurrency/fiberCreationException.h"
#include "concurrency/fiberSwitchException.h"
#include "concurrency/fiberToThreadException.h"
#include "concurrency/fiberCallbackException.h"
#include "concurrency/fiberQuitCallbackException.h"
#include "concurrency/threadCreationException.h"
#include "concurrency/threadAffinityException.h"

#include "importing/importException.h"

#include "io/ioException.h"
#include "io/fileNotFoundException.h"
#include "io/unauthorizedAccessException.h"

#include "jobs/counterWaitListFullException.h"
#include "jobs/jobQueueFullException.h"

#include "standard/allocationException.h"
#include "standard/invalidArgumentException.h"
#include "standard/unkownErrorException.h"