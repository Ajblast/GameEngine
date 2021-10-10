#pragma once

#include "base.h"

namespace GRAVCore
{
	namespace IO
	{
		TEST_CLASS(textFile)
		{
		public:
			TEST_METHOD(canReadLine);
			TEST_METHOD(canReadLineWithDelimiter);

			TEST_METHOD(canWrite);
			TEST_METHOD(canWriteLine);

			TEST_METHOD(canSearchForToken);
		private:
			// The open test file needs to be seperate because truncate mode will delete the contents and we need contents for other tests
			std::string m_TestOpenFilePath = "A:\\Development\\source\\Ajblast\\GameEngine\\testing\\TestOpenFile.txt";
			std::string m_TestWriteFilePath = "A:\\Development\\source\\Ajblast\\GameEngine\\testing\\TestWriteFile.txt";
			std::string m_TestReadFilePath = "A:\\Development\\source\\Ajblast\\GameEngine\\testing\\TestReadFile.txt";
			std::string m_TestWriteFileContents = "Test Write File Text";
			std::string m_TestReadFileContents = "Test File Text";
		};
	}
}