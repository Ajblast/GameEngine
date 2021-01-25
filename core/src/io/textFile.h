#pragma once

#include "common.h"
#include "file.h"

namespace GRAVEngine
{
	namespace io
	{
		class textFile : public file
		{
			textFile() = delete;
			textFile(const char* filePath, fileMode fileMode, bool flushAfterWrite);
			textFile& operator= (const textFile& other);

			textFile(textFile&& other) noexcept;
			textFile& operator= (textFile&& other) noexcept;

			// Read in a string into buffer. Returns false if file is at eof. String may have changed if error occurs while reading
			bool readString(char* buffer, int num);
			//void readLine(char*, size_t num);
			void writeString(const char* string);
			// Write a string and append a newline character to the end of the string
			void writeLine(const char* string);
		};
	}
}