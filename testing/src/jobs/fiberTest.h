#pragma once

#include "base.h"

namespace GRAVCore
{
	namespace Jobs
	{
		TEST_CLASS(fiber)
		{
		public:
			TEST_METHOD(canInitializeFromCurrentThread);
			TEST_METHOD(canBeCreated);

			TEST_METHOD(canSwithToOtherFiber);
			//TEST_METHOD(canSetName);
		};
	}
}