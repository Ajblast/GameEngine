#include "pch.h"
#include "textFileTest.h"
#include "io/files/textFile.h"

void GRAVCore::IO::textFile::canReadLine()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::textFile sut = GRAVEngine::IO::textFile(m_TestReadFilePath, fileMode, true);

	std::string readText;
	bool readResult = sut.readLine(readText);

	// The read must succeed, have the same buffer size, and contents be equal
	Assert::IsTrue(readResult && m_TestReadFileContents.compare(readText) == 0);
}
void GRAVCore::IO::textFile::canReadLineWithDelimiter()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::textFile sut = GRAVEngine::IO::textFile(m_TestReadFilePath, fileMode, true);

	std::string readText;
	bool readResult = sut.readLine(readText, ' ');

	std::string testDelimString = "Test";

	// The read must succeed, have the same buffer size, and contents be equal
	Assert::IsTrue(readResult && testDelimString.compare(readText) == 0);
}

void GRAVCore::IO::textFile::canWrite()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::OUTPUT | GRAVEngine::IO::fileMode::TRUNCATE;
	GRAVEngine::IO::textFile sut = GRAVEngine::IO::textFile(m_TestWriteFilePath, fileMode, true);

	sut.write(m_TestWriteFileContents);

	// Reopen the file as input
	sut.reopen(GRAVEngine::IO::fileMode::INPUT);

	std::string readString;
	bool readResult = sut.readAll(readString);
	bool correctRead = m_TestWriteFileContents.compare(readString) == 0;

	// The read must succeed, have the same buffer size, and contents be equal
	Assert::IsTrue(readResult && correctRead);
}
void GRAVCore::IO::textFile::canWriteLine()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::OUTPUT | GRAVEngine::IO::fileMode::TRUNCATE;
	GRAVEngine::IO::textFile sut = GRAVEngine::IO::textFile(m_TestWriteFilePath, fileMode, true);

	sut.writeLine(m_TestWriteFileContents);

	// Reopen the file as input
	sut.reopen(GRAVEngine::IO::fileMode::INPUT);

	std::string readString;
	bool readResult = sut.readLine(readString);
	bool correctRead = m_TestWriteFileContents.compare(readString) == 0;

	// The read must succeed, have the same buffer size, and contents be equal
	Assert::IsTrue(readResult && correctRead);
}
void GRAVCore::IO::textFile::canSearchForToken()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::textFile sut = GRAVEngine::IO::textFile(m_TestReadFilePath, fileMode, true);

	std::string token = "Test";
	Assert::IsTrue(sut.searchForToken(&token, 1, 200));
}
