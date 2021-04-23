#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace Tools
	{
		namespace Importing
		{
			// Description of an importer
			struct importerDescription
			{			
				const char* m_Name;			// Name of the importer
				const char* m_Notes;		// Name of the importer

				uint32 m_Flags;				// Flags for the importer

				// Both format versions are 0 if the importer doesn't care or there is not a version scheme
				uint32 m_MinFormatMajor;	// Minimum major version for the importer's format.
				uint32 m_MinFormatMinor;	// Minimum minor version for the importer's format.

				// Both format versions are 0 if the importer doesn't care or there is not a version scheme
				uint32 m_MaxFormatMajor;	// Maximum major version for the importer's format.
				uint32 m_MaxFormatMinor;	// Maximum minor version for the importer's format.
			};
		}
	}
}