#include "pch.h"
#include "sinksTest.h"
#include "logging/sinks/sinks.h"

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<>
			static std::wstring ToString<GRAVEngine::Logging::verbosity>(const GRAVEngine::Logging::verbosity& verbosity)
			{
				return stringToWString(GRAVEngine::Logging::toString(verbosity));
			}
		}
	}
}

void GRAVCore::Logging::Sinks::fileSink::canBeCreated()
{
	GRAVEngine::Logging::Sinks::fileSink sut(m_FilePath);

	Assert::IsTrue(sut.isOpen());

}
void GRAVCore::Logging::Sinks::fileSink::hasProperFilePath()
{
	GRAVEngine::Logging::Sinks::fileSink sut(m_FilePath);
	Assert::AreEqual(m_FilePath, sut.filePath());
}
void GRAVCore::Logging::Sinks::fileSink::canSetVerbosity()
{
	GRAVEngine::Logging::verbosity verbosity = GRAVEngine::Logging::verbosity::critical;
	GRAVEngine::Logging::Sinks::fileSink sut(m_FilePath);
	sut.setVerobisty(verbosity);

	Assert::AreEqual(verbosity, sut.verbosity());
}

void GRAVCore::Logging::Sinks::ostreamSink::canSetVerbosity()
{
	GRAVEngine::Logging::verbosity verbosity = GRAVEngine::Logging::verbosity::critical;
	GRAVEngine::Logging::Sinks::ostreamSink sut(std::cout, true);
	sut.setVerobisty(verbosity);

	Assert::AreEqual(verbosity, sut.verbosity());
}

