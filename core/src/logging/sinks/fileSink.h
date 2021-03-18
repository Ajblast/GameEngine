#pragma once

#include "common.h"
#include "baseSink.h"
#include "io/file.h"

namespace GRAVEngine
{
	namespace Logging
	{
		namespace Sinks
		{
			// TODO: Opening/Writing/Closing files

			class fileSink final : public baseSink
			{
			public:
				explicit fileSink(const std::string& filePath, bool truncate = false);
				inline const std::string& filePath()
				{
					return m_File.filePath();
				}

			protected:
				virtual void sinkMessage(const Logging::logMessage& message) override;
				virtual void flush_() override;

				GRAVEngine::IO::file m_File;
			};
		}
	}
}