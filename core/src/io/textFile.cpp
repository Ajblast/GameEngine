#include "gravpch.h"
#include "textFile.h"

GRAVEngine::IO::textFile::textFile() : file()
{
}
GRAVEngine::IO::textFile::textFile(const std::string& filePath, IO::fileMode fileMode, bool flushAfterWrite) : file(filePath, fileMode, flushAfterWrite)
{
}
GRAVEngine::IO::textFile::textFile(textFile&& other) noexcept : file(std::move(other))
{
}
GRAVEngine::IO::textFile& GRAVEngine::IO::textFile::operator=(textFile&& other) noexcept
{
	file::operator=(std::move(other));

	return *this;
}

void GRAVEngine::IO::textFile::readLine(char* buffer, size_t num)
{
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	// Read a line from the stream
	m_Stream.getline((char*)buffer, num);

	// Check if an error occured
	errorHandle();
}
void GRAVEngine::IO::textFile::readLine(char* buffer, size_t num, char delim)
{
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	// Read a line from the stream
	m_Stream.getline((char*)buffer, num, delim);

	// Check if an error occured
	errorHandle();
}
void GRAVEngine::IO::textFile::readLine(std::string& str)
{
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	// Read a line
	std::getline(m_Stream, str);
}
void GRAVEngine::IO::textFile::readLine(std::string& str, char delim)
{
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	// Read a line
	std::getline(m_Stream, str, delim);
}

void GRAVEngine::IO::textFile::write(const std::string& string)
{
	file::write(string.c_str(), string.size());
}
void GRAVEngine::IO::textFile::writeLine(const std::string& string)
{
	writeLine(string.c_str(), string.size());
}
void GRAVEngine::IO::textFile::writeLine(const char* string, size_t size)
{
	// Blocking write the string
	write(string);

	// Blocking write newline character
	writeChar('\n');
}