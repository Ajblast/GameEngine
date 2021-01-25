#pragma once
#include <string.h>

// Open file for input operations. Throw exception if it doesn't exist
#define GRAVEngine_FILE_MODE_READ			0
// Open a file for update (RW). Throw exception if it doesn't exit
#define GRAVEngine_FILE_MODE_READ_UPDATE	1
// Create an empty file for output operations. Discard data if it already exists
#define GRAVEngine_FILE_MODE_WRITE			2
// Create and empty file and open it for update (RW). Discard data if it already exists
#define GRAVEngine_FILE_MODE_WRITE_UPDATE	3
// Open file for output at the end of a file. Create it if it doesn't exist. All writes appear at end of file. Seeking ignored
#define GRAVEngine_FILE_MODE_APPEND			4
// Open file for update (RW). Create it if it doesn't exist. All write appear at end of file.
// Seeking affects input position. Writing moves position back to end of file.
#define GRAVEngine_FILE_MODE_APPEND_UPDATE	5

#define GRAVEngine_FILE_MODE_STRING_READ			"READ"
#define GRAVEngine_FILE_MODE_STRING_READ_UPDATE		"READ_UPDATE"
#define GRAVEngine_FILE_MODE_STRING_WRITE			"WRITE"
#define GRAVEngine_FILE_MODE_STRING_WRITE_UPDATE	"WRITE_UPDATE"
#define GRAVEngine_FILE_MODE_STRING_APPEND			"APPEND"
#define GRAVEngine_FILE_MODE_STRING_APPEND_UPDATE	"APPEND_UPDATE"

namespace GRAVEngine
{
	namespace io
	{
		enum class fileMode
		{
			read			= GRAVEngine_FILE_MODE_READ,
			readUpdate		= GRAVEngine_FILE_MODE_READ_UPDATE,
			write			= GRAVEngine_FILE_MODE_WRITE,
			writeUpdate		= GRAVEngine_FILE_MODE_WRITE_UPDATE,
			append			= GRAVEngine_FILE_MODE_APPEND,
			appendUpdate	= GRAVEngine_FILE_MODE_APPEND_UPDATE
		};

		static const char* fileModeStrings[]
		{
			GRAVEngine_FILE_MODE_STRING_READ,
			GRAVEngine_FILE_MODE_STRING_READ_UPDATE,
			GRAVEngine_FILE_MODE_STRING_WRITE,
			GRAVEngine_FILE_MODE_STRING_WRITE_UPDATE,
			GRAVEngine_FILE_MODE_STRING_APPEND,
			GRAVEngine_FILE_MODE_STRING_APPEND_UPDATE			
		};

		inline const char* toString(GRAVEngine::io::fileMode fileMode)
		{
			return fileModeStrings[static_cast<int>(fileMode)];
		}

		inline GRAVEngine::io::fileMode fileModeToEnum(const char* name)
		{
			int level = 0;

			for (auto levelstr : fileModeStrings)
			{
				if (strcmp(levelstr, name))
					return static_cast<GRAVEngine::io::fileMode>(level);

				level++;
			}

			return GRAVEngine::io::fileMode::read;
		}
	}
}