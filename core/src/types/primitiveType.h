#pragma once

namespace GRAVEngine
{

	enum class primitiveType
	{
		NONE		= 0,	// No primitive type
		POINT		= 1,	// Single vertex
		LINE		= 2,	// Line with a start and end
		TRIANGLE	= 3,	// A triangle
		QUAD		= 4,	// A quad
		NGON		= 5		// A polygon with 5 or more edges from 5 or more vertices
	};
}