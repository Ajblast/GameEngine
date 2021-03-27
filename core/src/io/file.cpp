#include "gravpch.h"
#include "file.h"

GRAVEngine::IO::file::file() : m_FileMode(fileMode::read), m_FileHandle(nullptr), m_FlushAfterWrite(false)
{}
GRAVEngine::IO::file::file(const std::string& filePath, GRAVEngine::IO::fileMode fileMode, bool flushAfterWrite) : m_FileHandle(nullptr)
{
	// Open the file
	open(filePath, fileMode, flushAfterWrite);
}
GRAVEngine::IO::file::file(const file& other)
{
	// Delete current resources
	close();

	// Copy resources
	m_FilePath = other.m_FilePath;
	m_FileHandle = other.m_FileHandle;
	m_FileMode = other.m_FileMode;
	m_FlushAfterWrite = other.m_FlushAfterWrite;
}
GRAVEngine::IO::file& GRAVEngine::IO::file::operator=(const file& other)
{
	if (this != &other)
	{
		// Close the current file
		close();

		// Copy the file handle
		m_FilePath			= other.m_FilePath;
		m_FileHandle		= other.m_FileHandle;
		m_FileMode			= other.m_FileMode;
		m_FlushAfterWrite	= other.m_FlushAfterWrite;
	}

	return *this;
}
GRAVEngine::IO::file::file(file&& other) noexcept : m_FilePath(std::move(other.m_FilePath)), m_FileMode(other.m_FileMode), m_FileHandle(other.m_FileHandle), m_FlushAfterWrite(other.m_FlushAfterWrite)
{
	other.m_FileMode		= fileMode::read;
	other.m_FileHandle		= nullptr;
	other.m_FlushAfterWrite = false;
}
GRAVEngine::IO::file& GRAVEngine::IO::file::operator=(file&& other) noexcept
{
	if (this != &other)
	{
		// Close the current file
		close();

		// Steal resources
		m_FilePath			= std::move(other.m_FilePath);
		m_FileHandle		= other.m_FileHandle;
		m_FileMode			= other.m_FileMode;
		m_FlushAfterWrite	= other.m_FlushAfterWrite;

		// Release pointers to resources
		other.m_FileHandle	= nullptr;
	}
	
	return *this;

}

GRAVEngine::IO::file::~file()
{
	// Close the file handle
	close();
}

bool GRAVEngine::IO::file::tryOpen(const std::string& filePath, fileMode fileMode, bool flushAfterWrite)
{
	try
	{
		open(filePath, fileMode, flushAfterWrite);
		return true;
	}
	catch (Exceptions::IO::ioException)
	{
		return false;
	}
	catch (Exceptions::unknownErrorException)
	{
		return false;
	}

	// Unknown exception type
	return false;
}

void GRAVEngine::IO::file::open(const std::string& filePath, fileMode fileMode, bool flushAfterWrite)
{
	// Close the current file if it is open
	if (isOpen())
		close();

	// Set file mode and file size
	m_FilePath = filePath;
	m_FileMode = fileMode;
	m_FlushAfterWrite = flushAfterWrite;

	// Check if the file directory exists, and create it if needed if it is not a read operation
	if (fileMode != fileMode::read && fileMode != fileMode::readUpdate)
	{
		std::filesystem::path path(filePath);
		if (std::filesystem::exists(path.remove_filename()) == false)
		{
			std::filesystem::create_directories(path);
		}
	}

	// Opening the file
	m_FileHandle = fopen(m_FilePath.c_str(), fileModeToString(m_FileMode));

	// Error Handling
	if (!m_FileHandle)
	{
		// Get the error code and test it
		int err = errno;
		errorHandle(err);
	}

	// The file is now open here
}

void GRAVEngine::IO::file::reopen(fileMode fileMode)
{
	GRAV_ASSERT(isOpen());

	// Reopen the file mode
	m_FileHandle = freopen(m_FilePath.c_str(), fileModeToString(fileMode), m_FileHandle);

	// Error Handling
	if (!m_FileHandle)
	{
		// Get the error code and test it
		int err = errno;
		errorHandle(err);
	}
}

bool GRAVEngine::IO::file::close()
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

bool GRAVEngine::IO::file::seek(long offset, seekOrigin origin)
{
	if (isOpen() == false)
		return false;

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

bool GRAVEngine::IO::file::flush()
{
	if (isOpen() == false)
		return false;

	size_t eof = fflush(m_FileHandle);

	// Get error code from reading
	int err = ferror(m_FileHandle); // Get an error if one occurred
	errorHandle(err);

	// Was the flush successful
	if (eof == 0)
		return true;
	return false;
}

size_t GRAVEngine::IO::file::offset()
{
	GRAV_ASSERT(isOpen());

	// Get the size
	size_t curOffset = ftell(m_FileHandle);

	// Check if there was an error
	int err = ferror(m_FileHandle);
	errorHandle(err);

	return curOffset;
}

int GRAVEngine::IO::file::eof()
{
	GRAV_ASSERT(isOpen());

	return feof(m_FileHandle);
}

bool GRAVEngine::IO::file::endOfFile()
{
	GRAV_ASSERT(isOpen());

	if(eof())
			return true;

	return false;
}

size_t GRAVEngine::IO::file::fileSize()
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

size_t GRAVEngine::IO::file::read(void* buffer, size_t bufferSize)
{
	GRAV_ASSERT(buffer != nullptr);
	GRAV_ASSERT(isOpen());

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

int GRAVEngine::IO::file::readChar()
{
	GRAV_ASSERT(isOpen());

	// Get a character
	int character = fgetc(m_FileHandle);

	// Get error code from reading
	int err = ferror(m_FileHandle); // Get an error if one occurred
	errorHandle(err);

	return character;
}

void GRAVEngine::IO::file::readAll(void*& buffer, size_t& bufferSize)
{
	GRAV_ASSERT(isOpen());

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
		throw Exceptions::allocationException("GRAVEngine::IO::file::readAll bad allocation");
	}

	// Seek to the beginning of the file
	seek(0, seekOrigin::set);

	// Read into the buffer
	read(buffer, bufferSize);
}

size_t GRAVEngine::IO::file::write(void* buffer, size_t bufferSize)
{
	GRAV_ASSERT(buffer != nullptr);
	GRAV_ASSERT(isOpen());

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

void GRAVEngine::IO::file::writeChar(char character)
{
	GRAV_ASSERT(isOpen());

	// Block here until all data has been written
	size_t bytesWritten = fputc(character, m_FileHandle);

	// Get error code from reading
	int err = ferror(m_FileHandle); // Get an error if one occurred
	errorHandle(err);

	if (m_FlushAfterWrite)
		flush();
}

void GRAVEngine::IO::file::errorHandle(int err)
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

const char* GRAVEngine::IO::file::fileModeToString(fileMode fileMode)
{
	switch (fileMode)
	{
	case GRAVEngine::IO::fileMode::read:
		return "rb";
		break;
	case GRAVEngine::IO::fileMode::readUpdate:
		return "rb+";
		break;
	case GRAVEngine::IO::fileMode::write:
		return "wb";
		break;
	case GRAVEngine::IO::fileMode::writeUpdate:
		return "wb+";
		break;
	case GRAVEngine::IO::fileMode::append:
		return "ab";
		break;
	case GRAVEngine::IO::fileMode::appendUpdate:
		return "ab+";
		break;
	default:
		return "rb";
		break;
	}

	return "rb";
}
