#pragma once

#include "jobPriorities.h"
#include "declaration.h"
#include "counter.h"
#include "jobManagerOptions.h"
#include "jobManager.h"

#define GRAV_KICK_JOB(declaration, counter)			GRAVEngine::Jobs::jobManager::getInstance()->kickJob(declaration, counter)
#define GRAV_KICK_JOBS(declarations, size, counter)	GRAVEngine::Jobs::jobManager::getInstance()->kickJobs(declarations, size, counter)
#define GRAV_WAIT_COUNTER(counter, target)			GRAVEngine::Jobs::jobManager::getInstance()->waitForCounter(counter, target)

// TODO: Special job mutex that is used for lock that are held longer
//	Puts the current job to sleep if needed instead of spin lock
//	Makes it that other jobs can be ran while waiting on resources

// TODO: Allow naming/renaming of fibers
//	Indicate current job

// TODO: Fiber callstacks
//	Viewable in the debugger
//	Can inspect fibers just like inspecting threads

// TODO: Fiber Crash handling
//	Dumping call stack into core dumps like threads

// TODO: Sized Fibers
//	Allocate fibers with different stack sizes
//	Naughty Dog: 128 x 64 KiB stack, 32 x 512 KiB stack

// TODO: Job profiling system
//	Allow visualizing of all past jobs and currently running jobs

// TODO: Special I/O Threads (Sockets, file I/O, system calls)
//	Implement by system threads
//	Implement like interrupt handlers (Read data, post new job)
//	Always waiting and nver do expensive processing of the data