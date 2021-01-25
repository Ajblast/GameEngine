#include "gravpch.h"
#include "file.h"

//GRAVEngine::io::file::file() : m_FilePath(nullptr), m_FileMode(fileMode::read), m_FileHandle(nullptr), m_FlushAfterWrite(false)
//{}
GRAVEngine::io::file::file(const char* filePath, GRAVEngine::io::fileMode fileMode, bool flushAfterWrite)
{
	GRAV_ASSERT(filePath != nullptr);

	STRING_COPY(m_FilePath, filePath, MAX_PATH_LENGTH);

	// Set file mode and file size
	m_FileMode = fileMode;
	m_FlushAfterWrite = flushAfterWrite;

	// Opening the file
	m_FileHandle = fopen(m_FilePath, fileModeToString(m_FileMode));

	// Error Handling
	if (!m_FileHandle)
	{
		// Get the error code and test it
		int err = errno;
		errorHandle(err);
	}

	// The file is now open here

}
GRAVEngine::io::file::file(const file& other)
{
	// Delete current resources
	delete[] m_FilePath;
	close();

	// Copy resources
	STRING_COPY(m_FilePath, other.m_FilePath, MAX_PATH_LENGTH);
	m_FileHandle = other.m_FileHandle;
	m_FileMode = other.m_FileMode;
	m_FlushAfterWrite = other.m_FlushAfterWrite;
}
GRAVEngine::io::file& GRAVEngine::io::file::operator=(const file& other)
{
	if (this != &other)
	{
		// Delete existing resources
		delete[] m_FilePath;

		// Close the current file
		close();

		// Copy the string
		STRING_COPY(m_FilePath, other.m_FilePath, MAX_PATH_LENGTH);

		// Copy the file handle
		m_FileHandle		= other.m_FileHandle;
		m_FileMode			= other.m_FileMode;
		m_FlushAfterWrite	= other.m_FlushAfterWrite;
	}

	return *this;
}
GRAVEngine::io::file::file(file&& other) noexcept : m_FilePath(other.m_FilePath), m_FileMode(other.m_FileMode), m_FileHandle(other.m_FileHandle), m_FlushAfterWrite(other.m_FlushAfterWrite)
{
	other.m_FilePath		= nullptr;
	other.m_FileMode		= fileMode::read;
	other.m_FileHandle		= nullptr;
	other.m_FlushAfterWrite = false;
}
GRAVEngine::io::file& GRAVEngine::io::file::operator=(file&& other) noexcept
{
	if (this != &other)
	{
		// Delete the file path
		delete[] m_FilePath;

		// Close the current file
		close();

		// Steal resources
		m_FilePath			= other.m_FilePath;
		m_FileHandle		= other.m_FileHandle;
		m_FileMode			= other.m_FileMode;
		m_FlushAfterWrite	= other.m_FlushAfterWrite;

		// Release pointers to resources
		other.m_FilePath	= nullptr;
		other.m_FileHandle	= nullptr;
	}
	
	return *this;

}

GRAVEngine::io::file::~file()
{
	// Delete the file path text
	delete[] m_FilePath;

	// Close the file handle
	close();
}

void GRAVEngine::io::file::reopen(fileMode fileMode)
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Reopen the file mode
	m_FileHandle = freopen(m_FilePath, fileModeToString(fileMode), m_FileHandle);

	// Error Handling
	if (!m_FileHandle)
	{
		// Get the error code and test it
		int err = errno;
		errorHandle(err);
	}
}

bool GRAVEngine::io::file::close()
{
	if (m_FileHandle)
	{
		// Close the file if it is open
		int closeVal = fclose(m_FileHandle);
		m_FileHandle = nullptr;

		if (closeVal)
			return false;
		return true;
	}

	return false;
}

bool GRAVEngine::io::file::seek(long offset, seekOrigin origin)
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Seek into the file
	int seekVal = fseek(m_FileHandle, offset, static_cast<int>(origin));

	// Check if there was an error
	int err = ferror(m_FileHandle);
	errorHandle(err);

	// See if it was successful
	if (seekVal)
		return false;

	return true;
}

bool GRAVEngine::io::file::flush()
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	size_t eof = fflush(m_FileHandle);

	// Get error code from reading
	int err = ferror(m_FileHandle); // Get an error if one occurred
	errorHandle(err);

	// Was the flush successful
	if (eof == 0)
		return true;
	return false;
}

size_t GRAVEngine::io::file::offset()
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Get the size
	size_t curOffset = ftell(m_FileHandle);

	// Check if there was an error
	int err = ferror(m_FileHandle);
	errorHandle(err);

	return curOffset;
}

int GRAVEngine::io::file::eof()
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	return feof(m_FileHandle);
}

bool GRAVEngine::io::file::endOfFile()
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	if(eof())
			return true;

	return false;
}

size_t GRAVEngine::io::file::fileSize()
{
	// Cache the current offset
	size_t currentPosition = offset();

	// Go the end of the file
	seek(0, seekOrigin::end);
	size_t fileSize = offset();

	// Return back to the cached position
	seek(currentPosition, seekOrigin::set);

	return fileSize;
}

size_t GRAVEngine::io::file::read(void* buffer, size_t bufferSize)
{
	GRAV_ASSERT(buffer != nullptr);
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Do nothing if nowhere to put data
	if (bufferSize == 0)
		return 0;

	// Block here until all data has been read
	size_t bytesRead = fread(buffer, 1, bufferSize, m_FileHandle);

	// Get error code from reading
	int err = ferror(m_FileHandle); // Get an error if one occurred
	errorHandle(err);
								 
	// Return the amount of bytes read if successful
	return bytesRead;
}

int GRAVEngine::io::file::readChar()
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Get a character
	int character = fgetc(m_FileHandle);

	// Get error code from reading
	int err = ferror(m_FileHandle); // Get an error if one occurred
	errorHandle(err);

	return character;
}

void GRAVEngine::io::file::readAll(void*& buffer, size_t& bufferSize)
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Set the buffer size
	bufferSize = fileSize();

	// Allocate the new buffer
	try
	{
		// TODO: override new to use custom allocation for memory tracking
		buffer = new void* [bufferSize];
	}
	catch (std::bad_alloc)
	{
		throw Exceptions::allocationException("GRAVEngine::io::file::readAll bad allocation");
	}

	// Seek to the beginning of the file
	seek(0, seekOrigin::set);

	// Read into the buffer
	read(buffer, bufferSize);
}

size_t GRAVEngine::io::file::write(void* buffer, size_t bufferSize)
{
	GRAV_ASSERT(buffer != nullptr);
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Do nothing if there is no data to write
	if (bufferSize == 0)
		return 0;

	// Block here until all data has been written
	size_t bytesWritten = fwrite(buffer, 1, bufferSize, m_FileHandle);

	// Get error code from reading
	int err = ferror(m_FileHandle); // Get an error if one occurred
	errorHandle(err);

	if (m_FlushAfterWrite)
		flush();

	return bytesWritten;
}

void GRAVEngine::io::file::writeChar(char character)
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Block here until all data has been written
	size_t bytesWritten = fputc(character, m_FileHandle);

	// Get error code from reading
	int err = ferror(m_FileHandle); // Get an error if one occurred
	errorHandle(err);

	if (m_FlushAfterWrite)
		flush();
}

void GRAVEngine::io::file::errorHandle(int err)
{
	if (err == 0)
		return;

	if (err == GRAVEngine_ERROR_FILE_NOT_FOUND)
		throw Exceptions::IO::fileNotFoundException(m_FilePath, "Unable to open file.");
	if (err == GRAVEngine_ERROR_IO)
		throw Exceptions::IO::ioException("IO Exception");
	if (err == GRAVEngine_ERROR_PERMISSION_DENIED)
		throw Exceptions::IO::unauthorizedAccessException(m_FilePath, "Unable to access file.");
	
	throw Exceptions::unknownErrorException("Unknown file error");
}

const char* GRAVEngine::io::file::fileModeToString(fileMode fileMode)
{
	switch (fileMode)
	{
	case GRAVEngine::io::fileMode::read:
		return "rb";
		break;
	case GRAVEngine::io::fileMode::readUpdate:
		return "rb+";
		break;
	case GRAVEngine::io::fileMode::write:
		return "wb";
		break;
	case GRAVEngine::io::fileMode::writeUpdate:
		return "wb+";
		break;
	case GRAVEngine::io::fileMode::append:
		return "ab";
		break;
	case GRAVEngine::io::fileMode::appendUpdate:
		return "ab+";
		break;
	default:
		return "rb";
		break;
	}

	return "rb";
}
