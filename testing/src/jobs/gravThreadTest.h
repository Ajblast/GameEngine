#pragma once

#include "base.h"

namespace GRAVCore
{
	namespace Jobs
	{
		TEST_CLASS(gravThread)
		{
		public:
			TEST_METHOD(canInitializeFromCurrentThread);
			TEST_METHOD(canBeCreated);
			TEST_METHOD(canSetAffinity);
			TEST_METHOD(canSetName);
		};
	}
}