#pragma once
#include <string.h>

#define GRAVEngine_JOB_PRIORITY_LOW			0
#define GRAVEngine_JOB_PRIORITY_NORMAL		1
#define GRAVEngine_JOB_PRIORITY_HIGH		2
#define GRAVEngine_JOB_PRIORITY_CRITICAL	3

#define GRAVEngine_JOB_PRIORITY_STRING_LOW		"LOW"
#define GRAVEngine_JOB_PRIORITY_STRING_NORMAL	"NORMAL"
#define GRAVEngine_JOB_PRIORITY_STRING_HIGH		"HIGH"
#define GRAVEngine_JOB_PRIORITY_STRING_CRITICAL	"CIRTICAL"


namespace GRAVEngine
{
	namespace Jobs
	{
		enum class jobPriority
		{
			LOW			= GRAVEngine_JOB_PRIORITY_LOW, 
			NORMAL		= GRAVEngine_JOB_PRIORITY_NORMAL,
			HIGH		= GRAVEngine_JOB_PRIORITY_HIGH, 
			CRITICAL	= GRAVEngine_JOB_PRIORITY_CRITICAL
		};

		static const char* jobPriorityStrings[]
		{
			GRAVEngine_JOB_PRIORITY_STRING_LOW,
			GRAVEngine_JOB_PRIORITY_STRING_NORMAL,
			GRAVEngine_JOB_PRIORITY_STRING_HIGH,
			GRAVEngine_JOB_PRIORITY_STRING_CRITICAL
		};

		GRAVAPI inline const char* toString(GRAVEngine::Jobs::jobPriority priority)
		{
			return jobPriorityStrings[static_cast<int>(priority)];
		}

		GRAVAPI inline GRAVEngine::Jobs::jobPriority jobPriorityToEnum(const char* name)
		{
			int level = 0;

			for (auto levelstr : jobPriorityStrings)
			{
				if (strcmp(levelstr, name))
					return static_cast<GRAVEngine::Jobs::jobPriority>(level);

				level++;
			}

			return GRAVEngine::Jobs::jobPriority::LOW;
		}

	}
}