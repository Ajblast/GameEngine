#include "gravpch.h"
#include "file.h"
#include "io.h"

GRAVEngine::IO::file::file() : m_FileMode(fileMode::NONE), m_FlushAfterWrite(false)
{}
GRAVEngine::IO::file::file(const std::string& filePath, GRAVEngine::IO::fileMode fileMode, bool flushAfterWrite)
{
	// Open the file
	open(filePath, fileMode, flushAfterWrite);
}
GRAVEngine::IO::file::file(file&& other) noexcept : m_FilePath(std::move(other.m_FilePath)), m_FileMode(other.m_FileMode), m_FlushAfterWrite(other.m_FlushAfterWrite), m_Stream(std::move(other.m_Stream))
{
	other.m_FileMode		= fileMode::NONE;
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
		m_Stream			= std::move(other.m_Stream);
		m_FileMode			= other.m_FileMode;
		m_FlushAfterWrite	= other.m_FlushAfterWrite;
	}
	
	return *this;

}

GRAVEngine::IO::file::~file()
{
	// Close the file handle
	close();
}

bool GRAVEngine::IO::file::tryOpen(const std::string& filePath, IO::fileMode fileMode, bool flushAfterWrite)
{
	try
	{
		open(filePath, fileMode, flushAfterWrite);
		return true;
	}
	catch (std::exception)
	{
		return false;
	}

	// Unknown exception type
	return false;
}
void GRAVEngine::IO::file::open(const std::string& filePath, IO::fileMode fileMode, bool flushAfterWrite)
{
	// Close the current file if it is open
	if (isOpen())
		close();

	// Set file mode and file size
	m_FilePath = filePath;
	m_FileMode = fileMode;
	m_FlushAfterWrite = flushAfterWrite;

	// Check if the file directory exists, and create it if needed if it is a write operation
	if ((fileMode & fileMode::OUTPUT) == fileMode::OUTPUT && IO::exists(filePath) == false)
		IO::createDirectory(std::filesystem::path(filePath).remove_filename());

	// Opening the file
	m_Stream.open(filePath, (std::fstream::openmode) fileMode);

	// Check for an error
	errorHandle();

	// Check if opening the file failed
	if (failed())
		throw Exceptions::IO::ioException("Unable to open file.");

	// The file is now open here
}
void GRAVEngine::IO::file::reopen(IO::fileMode fileMode)
{
	GRAV_ASSERT_LOGLESS(isOpen());

	// Close the current file first
	close();

	// Reopen the file mode
	open(m_FilePath, fileMode, m_FlushAfterWrite);
}
bool GRAVEngine::IO::file::close()
{
	if (isOpen())
	{
		// Close the stream
		m_Stream.close();

		// Check for any errors
		errorHandle();

		// The file was not able to be closed.
		if (failed())
			return false;

		return true;
	}

	return false;
}

bool GRAVEngine::IO::file::seekWrite(long offset, seekOrigin origin)
{
	if (isOpen() == false)
		return false;

	// Seek the write position
	m_Stream.seekp(offset, (std::ios_base::seekdir) origin);

	// Check if there is an error with the stream
	errorHandle();

	// Return false if it could not seek to the position
	if (failed())
		return false;

	// Default return true
	return true;
}
bool GRAVEngine::IO::file::seekRead(long offset, seekOrigin origin)
{
	if (isOpen() == false)
		return false;

	// Seek the input position
	m_Stream.seekg(offset, (std::ios_base::seekdir)origin);

	// Check if there is an error with the stream
	errorHandle();

	// Return false if it could not seek to the position
	if (failed())
		return false;

	// Default return true
	return true;
}

bool GRAVEngine::IO::file::flush()
{
	if (isOpen() == false)
		return false;

	// flush the stream
	m_Stream.flush();

	// Check if an error occured
	errorHandle();

	// Check if the flush failed
	if (failed())
		return false;

	// Default return tue
	return true;
}

const size_t GRAVEngine::IO::file::offsetWrite()
{
	// The file must be open
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isOutput());

	size_t position = m_Stream.tellp();

	errorHandle();

	return position;
}
const size_t GRAVEngine::IO::file::offsetRead()
{
	// The file must be open
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	size_t position = m_Stream.tellg();

	errorHandle();

	return position;
}
const size_t GRAVEngine::IO::file::fileSize()
{
	// File must be open
	GRAV_ASSERT_LOGLESS(isOpen());
	// File must be open in read or output
	GRAV_ASSERT_LOGLESS(isInput() || isOutput());

	size_t fileSize;

	if (isInput())
	{
		// Cache the current offset
		size_t currentPosition = offsetRead();

		// Go the end of the file
		seekRead(0, seekOrigin::end);
		fileSize = offsetRead();

		// Return back to the cached position
		seekRead(currentPosition, seekOrigin::beg);
	}
	else
	{
		// Cache the current offset
		size_t currentPosition = offsetWrite();

		// Go the end of the file
		seekWrite(0, seekOrigin::end);
		fileSize = offsetWrite();

		// Return back to the cached position
		seekWrite(currentPosition, seekOrigin::beg);
	}


	return fileSize;
}

size_t GRAVEngine::IO::file::readCount()
{
	// The file must be open for input
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	// Get the number of characters read
	return m_Stream.gcount();
}
int GRAVEngine::IO::file::peek()
{
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	// Peek the character
	int character = m_Stream.peek();

	// Check for an error
	errorHandle();

	return character;
}
bool GRAVEngine::IO::file::read(char* buffer, size_t bufferSize)
{
	GRAV_ASSERT_LOGLESS(buffer != nullptr);
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	// Do nothing if nowhere to put data
	if (bufferSize == 0)
	{
		// Reset the readcount to 0
		peek();
		// This is not a failure
		return true;
	}

	// Read into the buffer bufferSize of characters
	m_Stream.read(buffer, bufferSize);

	// Check if an error occured
	errorHandle();

	if (failed())
		return false;

	return true;
}
int GRAVEngine::IO::file::readChar()
{
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	// Get a character
	int character = m_Stream.get();

	// Get error code from reading
	errorHandle();

	return character;
}
bool GRAVEngine::IO::file::readAll(scope<char[]>& buffer, size_t& bufferSize)
{
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	// Set the buffer size
	bufferSize = fileSize();

	// Allocate the new buffer
	try
	{
		// TODO: override new to use custom allocation for memory tracking
		buffer = createScope<char[]>(bufferSize);
	}
	catch (std::bad_alloc)
	{
		throw Exceptions::allocationException("GRAVEngine::IO::file::readAll bad allocation");
	}

	// Seek to the beginning of the file
	seekRead(0, seekOrigin::beg);

	// Read into the buffer
	return read(buffer.get(), bufferSize);
}


void GRAVEngine::IO::file::write(const char* buffer, size_t bufferSize)
{
	GRAV_ASSERT_LOGLESS(buffer != nullptr);
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isOutput());

	// Write to the stream
	m_Stream.write(buffer, bufferSize);

	// Check if an error occured
	errorHandle();

	// Flush if it should flush after every write
	if (m_FlushAfterWrite)
		flush();
}
void GRAVEngine::IO::file::writeChar(char character)
{
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isOutput());

	// Write to the stream
	m_Stream.put(character);

	// Check if an error occured
	errorHandle();

	// Flush if it should flush after every write
	if (m_FlushAfterWrite)
		flush();
}