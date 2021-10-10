#pragma once

#include "base.h"

namespace GRAVCore
{
	namespace IO
	{
		TEST_CLASS(file)
		{
		public:
			TEST_METHOD(canBeOpenedInINPUTMode);
			TEST_METHOD(canBeOpenedInOUTPUTMode);
			TEST_METHOD(canBeOpenedInENDMode);
			TEST_METHOD(canBeOpenedInAPPENDMode);
			TEST_METHOD(canBeOpenedInTRUNCATEMode);
			TEST_METHOD(canBeOpenedInBINARYMode);

			TEST_METHOD(willHaveCorrectFilePath);
			TEST_METHOD(willHaveCorrectFileExtension);

			TEST_METHOD(canBeReopened);
			TEST_METHOD(canBeClosed);

			TEST_METHOD(canSeekWriteToBeginning);
			TEST_METHOD(canSeekWriteToCurent);
			TEST_METHOD(canSeekWriteToEnding);
			TEST_METHOD(canSeekReadToBeginning);
			TEST_METHOD(canSeekReadToCurent);
			TEST_METHOD(canSeekReadToEnding);

			TEST_METHOD(willHaveZeroWriteOffsetAtBeginning);
			TEST_METHOD(willHaveZeroReadOffsetAtBeginning);
			TEST_METHOD(willHaveFileSizeWriteOffsetAtEnding);
			TEST_METHOD(willHaveFileSizeReadOffsetAtEnding);

			TEST_METHOD(canReadSingleChar);
			TEST_METHOD(canReadAll);

			TEST_METHOD(canWriteChar);
			TEST_METHOD(canWriteAll);

			TEST_METHOD(canFlush);
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