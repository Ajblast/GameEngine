#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

#include "common.h"
#include "fileModes.h"
#include "seekOrigin.h"
#include "io.h"

#include "exceptions/exceptions.h"

namespace GRAVEngine
{
	namespace IO
	{
		class GRAVAPI file
		{
		public:
			file();
			file(const std::string& filePath, fileMode mode, bool flushAfterWrite);
			file(const file& other) = delete;				// Can't copy file streams
			file& operator= (const file& other) = delete;	// Can't copy file streams
		
			file(file&& other) noexcept;
			file& operator= (file&& other) noexcept;

			virtual ~file();

			#pragma region FileOpening
			// Try to open the file
			bool tryOpen(const std::string& filePath, fileMode mode, bool flushAfterWrite);
			// Open the file
			void open(const std::string& filePath, fileMode mode, bool flushAfterWrite);
			// Reopen the file with a new file mode
			void reopen(fileMode mode);
			// Close the file
			bool close();
			#pragma endregion

			#pragma region Seeking
			// Seek into the file the write position
			bool seekWrite(long offset, seekOrigin origin);
			// Seek into the file the read position
			bool seekRead(long offset, seekOrigin origin);
			#pragma endregion

			// Flush the write buffer
			bool flush();

			#pragma region Offsets
			// Get the write offset into the file
			const size_t offsetWrite();
			// Get the read offset into the file
			const size_t offsetRead();
			// Get the size of the file in bytes
			const size_t fileSize();
			#pragma endregion

			#pragma region Input
			// Amount of characters read from the last read operation
			size_t readCount();
			// Peek the next read character
			int peek();
			// Read bufferSize bytes into a buffer
			bool read(char* buffer, size_t bufferSize);
			// Read a character. Returns eof if it was the end of the file
			int readChar();
			// Read the entire file into a buffer. Memory allocated
			bool readAll(char*& buffer, size_t& bufferSize);
			#pragma endregion

			#pragma region Output
			// Write all of the bytes in the buffer
			void write(const char* buffer, size_t bufferSize);
			// Write a character (byte) to the file
			void writeChar(char character);
			#pragma endregion

			#pragma region Getters
			// Get the file mode
			inline const fileMode fileMode() const { return m_FileMode; }
			// Get the file path
			inline const std::string& filePath() { return m_FilePath; }
			// Get the file extension
			inline const std::string& fileExtension() { return IO::fileExtension(filePath()); }

			// Is the file open
			inline const bool isOpen() const { return m_Stream.is_open(); }
			// Is the file at the end of the file
			inline const bool endOfFile() const { return m_Stream.eof(); }
			// Did the last io operation fail
			inline const bool failed() const { return m_Stream.fail(); }
			#pragma endregion

			#pragma region FileModeTests
			// Is read enabled
			inline const bool isInput() { return (m_FileMode & fileMode::INPUT) == fileMode::INPUT; }
			// Is write enabled
			inline const bool isOutput() { return (m_FileMode & fileMode::OUTPUT) == fileMode::OUTPUT; }
			// Is the output position moved to the end
			inline const bool isEnd() { return (m_FileMode & fileMode::END) == fileMode::END; }
			// Is the file appending
			inline const bool isAppend() { return (m_FileMode & fileMode::APPEND) == fileMode::APPEND; }
			// Is the file truncating
			inline const bool isTruncate() { return (m_FileMode & fileMode::TRUNCATE) == fileMode::TRUNCATE; }
			// Is the file in binary
			inline const bool isBinary() { return (m_FileMode & fileMode::BINARY) == fileMode::BINARY; }
			#pragma endregion

		protected:
			inline void errorHandle()
			{
				// Check if the stream is bad for any reason
				if (m_Stream.bad())
					throw Exceptions::IO::ioException("File operation error");
			}
		protected:
			std::string m_FilePath;	// File path
			IO::fileMode m_FileMode;	// File mode
			bool m_FlushAfterWrite;	// Should the buffer be flushed after every write?

			std::fstream m_Stream;
		};
	}
}