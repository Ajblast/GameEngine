#pragma once
#include <string.h>

#define GRAVEngine_SEEK_ORIGIN_SET	0
#define GRAVEngine_SEEK_ORIGIN_CUR	1
#define GRAVEngine_SEEK_ORIGIN_END	2

#define GRAVEngine_SEEK_ORIGIN_STRING_SET	"SET"
#define GRAVEngine_SEEK_ORIGIN_STRING_CUR	"CUR"
#define GRAVEngine_SEEK_ORIGIN_STRING_END	"END"

namespace GRAVEngine
{
	namespace IO
	{
		enum class seekOrigin
		{
			set = GRAVEngine_SEEK_ORIGIN_SET,
			cur = GRAVEngine_SEEK_ORIGIN_CUR,
			end = GRAVEngine_SEEK_ORIGIN_END
		};

		static const char* seekOriginStrings[]
		{
			GRAVEngine_SEEK_ORIGIN_STRING_SET,
			GRAVEngine_SEEK_ORIGIN_STRING_CUR,
			GRAVEngine_SEEK_ORIGIN_STRING_END
		};

		inline const char* toString(GRAVEngine::IO::seekOrigin seekOrigin)
		{
			return seekOriginStrings[static_cast<int>(seekOrigin)];
		}

		inline GRAVEngine::IO::seekOrigin seekOriginToEnum(const char* name)
		{
			int level = 0;

			for (auto levelstr : seekOriginStrings)
			{
				if (strcmp(levelstr, name))
					return static_cast<GRAVEngine::IO::seekOrigin>(level);

				level++;
			}

			return GRAVEngine::IO::seekOrigin::set;
		}
	}
}