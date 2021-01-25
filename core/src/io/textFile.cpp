#include "gravpch.h"
#include "textFile.h"

//GRAVEngine::io::textFile::textFile() : file()
//{
//}

GRAVEngine::io::textFile::textFile(const char* filePath, fileMode fileMode, bool flushAfterWrite) : file(filePath, fileMode, flushAfterWrite)
{
}

GRAVEngine::io::textFile& GRAVEngine::io::textFile::operator=(const textFile& other)
{
	file::operator=(other);

	return *this;
}

GRAVEngine::io::textFile::textFile(textFile&& other) noexcept : file(other)
{
}

GRAVEngine::io::textFile& GRAVEngine::io::textFile::operator=(textFile&& other) noexcept
{
	file::operator=(other);

	return *this;
}

bool GRAVEngine::io::textFile::readString(char* buffer, int maxCharCount)
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

void GRAVEngine::io::textFile::writeString(const char* string)
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Blocking write the string
	fputs(string, m_FileHandle);

	// Get error code from reading
	int err = ferror(m_FileHandle); // Get an error if one occurred
	errorHandle(err);
}

void GRAVEngine::io::textFile::writeLine(const char* string)
{
	GRAV_ASSERT(m_FileHandle != nullptr);

	// Blocking write the string
	writeString(string);

	// Blocking write newline character
	writeChar('\n');
}



//void GRAVEngine::io::textFile::readLine(char*, size_t num)
//{
//}
