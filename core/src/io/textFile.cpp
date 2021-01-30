#include "gravpch.h"
#include "textFile.h"

//GRAVEngine::IO::textFile::textFile() : file()
//{
//}

GRAVEngine::IO::textFile::textFile(const char* filePath, fileMode fileMode, bool flushAfterWrite) : file(filePath, fileMode, flushAfterWrite)
{
}

GRAVEngine::IO::textFile& GRAVEngine::IO::textFile::operator=(const textFile& other)
{
	file::operator=(other);

	return *this;
}

GRAVEngine::IO::textFile::textFile(textFile&& other) noexcept : file(other)
{
}

GRAVEngine::IO::textFile& GRAVEngine::IO::textFile::operator=(textFile&& other) noexcept
{
	file::operator=(other);

	return *this;
}

bool GRAVEngine::IO::textFile::readString(char* buffer, int maxCharCount)
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Blocking read the string
	void* string = fgets(buffer, maxCharCount, m_FileHandle);

	// Get error code from reading
	int err = ferror(m_FileHandle); // Get an error if one occurred
	errorHandle(err);

	// Check if a string was actually read in. (Not end of file)
	if (string == nullptr)
		return false;

	return true;
}

void GRAVEngine::IO::textFile::writeString(const char* string)
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Blocking write the string
	fputs(string, m_FileHandle);

	// Get error code from reading
	int err = ferror(m_FileHandle); // Get an error if one occurred
	errorHandle(err);
}

void GRAVEngine::IO::textFile::writeLine(const char* string)
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Blocking write the string
	writeString(string);

	// Blocking write newline character
	writeChar('\n');
}



//void GRAVEngine::IO::textFile::readLine(char*, size_t num)
//{
//}
