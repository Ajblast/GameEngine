#pragma once

#include "base.h"

namespace GRAVCore
{
	namespace Logging
	{
		namespace Sinks
		{

			TEST_CLASS(fileSink)
			{
			public:
				TEST_METHOD(canBeCreated);
				TEST_METHOD(hasProperFilePath);
				TEST_METHOD(canSetVerbosity);
			private:
				std::string m_FilePath = "A:\\Development\\source\\Ajblast\\GameEngine\\testing\\fileSink.log";
			};

			TEST_CLASS(ostreamSink)
			{
			public:
				TEST_METHOD(canSetVerbosity);
			};

			TEST_CLASS(msvcSink)
			{
			};
		}
	}
}
