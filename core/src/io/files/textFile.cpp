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

//bool GRAVEngine::IO::textFile::readLine(char* buffer, size_t num)
//{
//	GRAV_ASSERT_LOGLESS(isOpen());
//	GRAV_ASSERT_LOGLESS(isInput());
//
//	if (endOfFile())
//		return false;
//
//	// Read a line from the stream
//	m_Stream.getline((char*)buffer, num);
//
//	// Check if an error occured
//	errorHandle();
//
//	if (failed())
//		return false;
//
//	return true;
//}
//bool GRAVEngine::IO::textFile::readLine(char* buffer, size_t num, char delim)
//{
//	GRAV_ASSERT_LOGLESS(isOpen());
//	GRAV_ASSERT_LOGLESS(isInput());
//
//	if (endOfFile())
//		return false;
//
//	// Read a line from the stream
//	m_Stream.getline((char*)buffer, num, delim);
//
//	// Check if an error occured
//	errorHandle();
//
//	if (failed())
//		return false;
//
//	return true;
//}
bool GRAVEngine::IO::textFile::readLine(std::string& str)
{
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	if (endOfFile())
		return false;

	// Read a line
	std::getline(m_Stream, str);

	// Check if an error occured
	errorHandle();

	if (failed())
		return false;

	return true;
}
bool GRAVEngine::IO::textFile::readLine(std::string& str, char delim)
{
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	if (endOfFile())
		return false;

	// Read a line
	std::getline(m_Stream, str, delim);

	// Check if an error occured
	errorHandle();

	if (failed())
		return false;

	return true;
}

bool GRAVEngine::IO::textFile::readAll(std::string& str)
{
	GRAV_ASSERT_LOGLESS(isOpen());
	GRAV_ASSERT_LOGLESS(isInput());

	if (endOfFile())
		return false;

	scope<char[]> buffer;
	size_t bufferSize;

	bool readResult = file::readAll(buffer, bufferSize);
	if (readResult == false)
		return false;

	// Copy the generated buffer into another buffer +1 for /0
	scope<char[]> nBuffer = createScope<char[]>(bufferSize + 1);
	std::memcpy(nBuffer.get(), buffer.get(), bufferSize);
	nBuffer[bufferSize] = 0;

	// Create the string
	str = std::string(nBuffer.get());

	// Check if an error occured
	errorHandle();

	if (failed())
		return false;

	return true;
}

void GRAVEngine::IO::textFile::write(const std::string& string)
{
	// Write the string and the null character
	file::write(string.c_str(), string.size());
}
void GRAVEngine::IO::textFile::writeLine(const std::string& string)
{
	write(string + '\n');
}
//void GRAVEngine::IO::textFile::writeLine(const char* string, size_t size)
//{
//	// Blocking write the string without the null character
//	file::write(string, size - 1);
//
//	// Blocking write newline character
//	writeChar('\n');
//
//	// Write the null character
//	writeChar(0);
//}

bool GRAVEngine::IO::textFile::searchForToken(const std::string* tokens, size_t tokenCount, size_t byteSearchDepth)
{
	GRAV_ASSERT(isOpen());
	GRAV_ASSERT(isInput());

	// Move to the start of the file
	seekRead(0, seekOrigin::beg);

	// Create the buffer
	scope<char[]> buffer = createScope<char[]>(byteSearchDepth + 1);

	// Read from the file
	read(buffer.get(), byteSearchDepth);

	// Precheck to see if anything was actually read
	if (readCount() == 0)
		return false;

	// Lowercase the bytes
	for (size_t i = 0; i < readCount(); i++)
		buffer[i] = (char)::tolower(buffer[i]);

	// Make the file be a C-string
	buffer[readCount()] = 0;

	std::string bufferString(buffer.get());
	std::string token;
	for (size_t i = 0; i < tokenCount; i++)
	{
		token = tokens[i];
		// Make sure the token is lowercase
		for (auto it = token.begin(); it != token.end(); it++)
			(*it) = (char) tolower(*it);

		// Search the buffer string for the token
		size_t pos = bufferString.find(token);
		if (pos == std::string::npos)
			continue;
		else
			return true;

	}

	return false;
}
