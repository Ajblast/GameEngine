#pragma once
#include <fstream>

namespace GRAVEngine
{
	namespace IO
	{
		// Bitmask file modes to open a file with.
		enum class fileMode : unsigned
		{
			NONE		= 0,
			INPUT		= std::fstream::in,		// Allow input
			OUTPUT		= std::fstream::out,	// Allow output
			END			= std::fstream::ate,	// Move the output position to the end of the file
			APPEND		= std::fstream::app,	// Output operations happen at the end of the file no matter what
			TRUNCATE	= std::fstream::trunc,	// Remove existing contents before discarding
			BINARY		= std::fstream::binary,	// Open the file in binary mode
		};

		fileMode operator |(fileMode lhs, fileMode rhs);
		fileMode operator &(fileMode lhs, fileMode rhs);
		fileMode operator ^(fileMode lhs, fileMode rhs);
		fileMode operator ~(fileMode rhs);
		fileMode& operator |=(fileMode& lhs, fileMode rhs);
		fileMode& operator &=(fileMode& lhs, fileMode rhs);
		fileMode& operator ^=(fileMode& lhs, fileMode rhs);

	}
}