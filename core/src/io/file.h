#pragma once

#include <cstdio>
#include <string>
#include "common.h"
#include "fileModes.h"
#include "seekOrigin.h"

#include "exceptions/exceptions.h"

namespace GRAVEngine
{
	namespace IO
	{
		class file
		{
		public:
			file();
			file(const std::string& filePath, fileMode fileMode, bool flushAfterWrite);
			file(const file& other);
			file& operator= (const file& other);
		
			file(file&& other) noexcept;
			file& operator= (file&& other) noexcept;

			~file();

			// Get the file path
			inline const std::string& filePath()
			{
				return m_FilePath;
			}

			void open(const std::string& filePath, fileMode fileMode, bool flushAfterWrite);
			// Reopen the file with a new file mode
			void reopen(fileMode fileMode);
			// Close the file
			bool close();

			// Seek into the file
			bool seek(long offset, seekOrigin origin);
			// Flush the write buffer
			bool flush();

			// Get the offset into the file
			size_t offset();
			// Get the end of file flag. Returns 0 if the file is not at the end of the file
			int eof();
			// Is the file at the end of the file
			bool endOfFile();
			// Get the size of the file in bytes
			size_t fileSize();

			// Read bufferSize bytes into a buffer
			size_t read(void* buffer, size_t bufferSize);
			// Read a character. Returns eof if it was the end of the file
			int readChar();
			// Read the entire file into a buffer. Memory allocated
			void readAll(void*& buffer, size_t& bufferSize);

			// Write all of the bytes in the buffer
			size_t write(void* buffer, size_t bufferSize);
			// Write a character (byte) to the file
			void writeChar(char character);

			inline bool isOpen() { return m_FileHandle != nullptr; }
		protected:
			void errorHandle(int err);
			const char* fileModeToString(fileMode fileMode);
		protected:
			std::string m_FilePath;	// File path
			fileMode m_FileMode;	// File mode
			FILE* m_FileHandle;		// File handle when the file is open
			bool m_FlushAfterWrite;	// Should the buffer be flushed after every write?
		};
	}
}