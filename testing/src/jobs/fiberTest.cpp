#include "pch.h"
#include "fiberTest.h"
#include "jobs/fiber.h"
#include <iostream>

void GRAVCore::Jobs::fiber::canInitializeFromCurrentThread()
{
	GRAVEngine::Jobs::fiber sut;
	sut.initializeFromCurrentThread();

	bool validInitialization = sut.isValid();

	// Have to turn the tread back into
	sut.convertToThread();

	bool validConversion = sut.isValid() == false;

	Assert::IsTrue(validInitialization && validConversion);
}

void GRAVCore::Jobs::fiber::canBeCreated()
{
	GRAVEngine::Jobs::fiber sut;
	sut.spawn([](GRAVEngine::Jobs::fiber*) {});

	Assert::IsTrue(sut.isValid());
}

static GRAVEngine::Jobs::fiber* sutPTR = nullptr;
static bool callbackRan = false;
static void testCallback(GRAVEngine::Jobs::fiber* fiber)
{
	// The callback was ran.
	callbackRan = true;
	// Switch back to the fiber
	fiber->switchTo(sutPTR);
}

void GRAVCore::Jobs::fiber::canSwithToOtherFiber()
{
	GRAVEngine::Jobs::fiber sut;
	// Initialize the current fiber so switching can occur
	sut.initializeFromCurrentThread();
	sutPTR = &sut;

	callbackRan = false;

	GRAVEngine::Jobs::fiber sut2;
	sut2.spawn(testCallback);

	// Call the other sut
	sut.switchTo(&sut2);

	// Turn fiber back to thread for cleaning things up
	sut.convertToThread();

	// Both fibers must be valid and the callback needs to run
	Assert::IsTrue(callbackRan);
}