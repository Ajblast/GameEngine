#pragma once

#include <string>
#include "common.h"
#include "file.h"

namespace GRAVEngine
{
	namespace IO
	{
		class GRAVAPI textFile : public file
		{
		public:
			textFile();
			textFile(const std::string& filePath, IO::fileMode fileMode, bool flushAfterWrite);
			textFile(const textFile& other) = delete;				// Delete copy constructor
			textFile& operator= (const textFile& other) = delete;	// Delete copy operator

			textFile(textFile&& other) noexcept;
			textFile& operator= (textFile&& other) noexcept;

			// Read a string into buffer. A null character is automatically appended.
			//bool readLine(char* buffer, size_t num);
			// Read a string into buffer spliting by delim. A null character is automatically appended.			
			//bool readLine(char* buffer, size_t num, char delim);
			
			// Read a string into buffer. A null character is automatically appended.
			bool readLine(std::string& str);
			// Read a string into buffer spliting by delim. A null character is automatically appended.
			bool readLine(std::string& str, char delim);
			// Read the entire file into the string
			bool readAll(std::string& str);

			// Write a string to the file
			void write(const std::string& string);
			// Write a string and append a newline character to the end of the string
			void writeLine(const std::string& string);
			// Write a string and append a newline character to the end of the string
			//void writeLine(const char* string, size_t size);

			// Search the header of the file for a specific token.
			bool searchForToken(const std::string* tokens, size_t tokencount, size_t byteSearchDepth = 200);
		};
	}
}