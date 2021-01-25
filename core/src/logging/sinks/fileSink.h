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
				explicit fileSink(const char* filePath, bool truncate = false);
				inline const char* filePath()
				{
					return m_File.filePath();
				}

			protected:
				virtual void sinkMessage(const Logging::logMessage& message) override;
				virtual void flush_() override;

				GRAVEngine::io::file m_File;
			};
		}
	}
}