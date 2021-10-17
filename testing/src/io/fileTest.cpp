#include "pch.h"
#include "fileTest.h"
#include "io/files/file.h"

void GRAVCore::IO::file::canBeOpenedInINPUTMode()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestOpenFilePath, fileMode, false);
	Assert::IsTrue(sut.isOpen() && sut.fileMode() == fileMode);
}
void GRAVCore::IO::file::canBeOpenedInOUTPUTMode()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::OUTPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestOpenFilePath, fileMode, false);
	Assert::IsTrue(sut.isOpen() && sut.fileMode() == fileMode);
}
void GRAVCore::IO::file::canBeOpenedInENDMode()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT | GRAVEngine::IO::fileMode::END;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestOpenFilePath, fileMode, false);
	Assert::IsTrue(sut.isOpen() && sut.fileMode() == fileMode);
}
void GRAVCore::IO::file::canBeOpenedInAPPENDMode()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::APPEND;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestOpenFilePath, fileMode, false);
	Assert::IsTrue(sut.isOpen() && sut.fileMode() == fileMode);
}
void GRAVCore::IO::file::canBeOpenedInTRUNCATEMode()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::OUTPUT | GRAVEngine::IO::fileMode::TRUNCATE;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestOpenFilePath, fileMode, false);
	Assert::IsTrue(sut.isOpen() && sut.fileMode() == fileMode);
}
void GRAVCore::IO::file::canBeOpenedInBINARYMode()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT | GRAVEngine::IO::fileMode::BINARY;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestOpenFilePath, fileMode, false);
	Assert::IsTrue(sut.isOpen() && sut.fileMode() == fileMode);
}

void GRAVCore::IO::file::willHaveCorrectFilePath()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestOpenFilePath, fileMode, false);
	Assert::AreEqual(m_TestOpenFilePath, sut.filePath());
}
void GRAVCore::IO::file::willHaveCorrectFileExtension()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestOpenFilePath, fileMode, false);
	
	std::filesystem::path path = std::filesystem::path(m_TestOpenFilePath);
	std::filesystem::path extension = path.extension();
	std::string a = extension.string();

	std::string s = GRAVEngine::IO::fileExtension(std::filesystem::path(m_TestOpenFilePath));

	std::string t = GRAVEngine::IO::fileExtension(m_TestOpenFilePath);

	std::string fileExtension = ".txt";
	std::string foundExtension = sut.fileExtension();
	Assert::AreEqual(fileExtension, sut.fileExtension());
}

void GRAVCore::IO::file::canBeReopened()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestOpenFilePath, fileMode, false);

	// Reopen with the exact smae file mode
	sut.reopen(fileMode);
	Assert::IsTrue(sut.isOpen() && sut.fileMode() == fileMode);
}
void GRAVCore::IO::file::canBeClosed()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestOpenFilePath, fileMode, false);

	Assert::IsTrue(sut.close());
}
void GRAVCore::IO::file::canFlush()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::OUTPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestWriteFilePath, fileMode, true);

	Assert::IsTrue(sut.flush());
}

void GRAVCore::IO::file::canSeekWriteToBeginning()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT | GRAVEngine::IO::fileMode::OUTPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);

	Assert::IsTrue(sut.seekWrite(0, GRAVEngine::IO::seekOrigin::beg));
}
void GRAVCore::IO::file::canSeekWriteToCurent()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT | GRAVEngine::IO::fileMode::OUTPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);

	Assert::IsTrue(sut.seekWrite(0, GRAVEngine::IO::seekOrigin::cur));
}
void GRAVCore::IO::file::canSeekWriteToEnding()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT | GRAVEngine::IO::fileMode::OUTPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);

	Assert::IsTrue(sut.seekWrite(0, GRAVEngine::IO::seekOrigin::end));
}
void GRAVCore::IO::file::canSeekReadToBeginning()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);

	Assert::IsTrue(sut.seekRead(0, GRAVEngine::IO::seekOrigin::beg));
}
void GRAVCore::IO::file::canSeekReadToCurent()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);

	Assert::IsTrue(sut.seekRead(0, GRAVEngine::IO::seekOrigin::cur));
}
void GRAVCore::IO::file::canSeekReadToEnding()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);

	Assert::IsTrue(sut.seekRead(0, GRAVEngine::IO::seekOrigin::end));
}

void GRAVCore::IO::file::willHaveZeroWriteOffsetAtBeginning()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT | GRAVEngine::IO::fileMode::OUTPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);

	sut.seekWrite(0, GRAVEngine::IO::seekOrigin::beg);

	size_t offset = 0;
	Assert::AreEqual(offset, sut.offsetWrite());
}
void GRAVCore::IO::file::willHaveZeroReadOffsetAtBeginning()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);

	sut.seekRead(0, GRAVEngine::IO::seekOrigin::beg);

	size_t offset = 0;
	Assert::AreEqual(offset, sut.offsetRead());
}
void GRAVCore::IO::file::willHaveFileSizeWriteOffsetAtEnding()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT | GRAVEngine::IO::fileMode::OUTPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);

	sut.seekWrite(0, GRAVEngine::IO::seekOrigin::end);

	size_t filesize = m_TestReadFileContents.size();
	size_t offsetWrite = sut.offsetWrite();
	Assert::AreEqual(filesize, offsetWrite);
}
void GRAVCore::IO::file::willHaveFileSizeReadOffsetAtEnding()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);

	sut.seekRead(0, GRAVEngine::IO::seekOrigin::end);

	size_t filesize = m_TestReadFileContents.size();
	size_t offsetRead = sut.offsetRead();
	Assert::AreEqual(filesize, offsetRead);
}

void GRAVCore::IO::file::canReadSingleChar()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);

	int character = sut.readChar();
	Assert::AreEqual(m_TestReadFileContents[0], (char)character);
}
void GRAVCore::IO::file::canReadAll()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::INPUT;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestReadFilePath, fileMode, false);


	GRAVEngine::scope<char[]> buffer;
	size_t bufferSize;
	bool readResult = sut.readAll(buffer, bufferSize);

	size_t originalSize = m_TestReadFileContents.size();
	bool bufferSizeCorret = m_TestReadFileContents.size() == bufferSize;
	bool correctRead = std::memcmp(buffer.get(), m_TestReadFileContents.c_str(), bufferSize) == 0;

	// The read must succeed, have the same buffer size, and contents be equal
	Assert::IsTrue(readResult && bufferSizeCorret && correctRead);
}

void GRAVCore::IO::file::canWriteChar()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::OUTPUT | GRAVEngine::IO::fileMode::TRUNCATE;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestWriteFilePath, fileMode, true);

	sut.writeChar(m_TestWriteFileContents[0]);

	// Reopen the file as input
	sut.reopen(GRAVEngine::IO::fileMode::INPUT);

	int character = sut.readChar();
	Assert::AreEqual(m_TestWriteFileContents[0], (char)character);
}
void GRAVCore::IO::file::canWriteAll()
{
	GRAVEngine::IO::fileMode fileMode = GRAVEngine::IO::fileMode::OUTPUT | GRAVEngine::IO::fileMode::TRUNCATE;
	GRAVEngine::IO::file sut = GRAVEngine::IO::file(m_TestWriteFilePath, fileMode, true);

	// Writing an actual c-string
	sut.write(m_TestWriteFileContents.c_str(), m_TestWriteFileContents.size() + 1);

	// Reopen the file as input
	sut.reopen(GRAVEngine::IO::fileMode::INPUT);

	GRAVEngine::scope<char[]> buffer;
	size_t bufferSize;
	bool readResult = sut.readAll(buffer, bufferSize);
	bool bufferSizeCorret = m_TestWriteFileContents.size() + 1 == bufferSize;
	bool correctRead = m_TestWriteFileContents.compare(buffer.get()) == 0;

	// The read must succeed, have the same buffer size, and contents be equal
	Assert::IsTrue(readResult && bufferSizeCorret && correctRead);
}