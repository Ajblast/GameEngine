#pragma once

#include <cstdint>
#include "common.h"

namespace GRAVEngine
{

#pragma region Normal Integers
	// Unsigned Integers
	typedef uint8_t		uint8;
	typedef uint16_t	uint16;
	typedef uint32_t	uint32;
	typedef uint64_t	uint64;

	// Signed Integers
	typedef int8_t	int8;
	typedef int16_t	int16;
	typedef int32_t int32;
	typedef int64_t int64;
#pragma endregion

#pragma region Least Integers
	// Unsigned Integers
	typedef uint_least8_t	uint_least8;
	typedef uint_least16_t	uint_least16;
	typedef uint_least32_t	uint_least32;
	typedef uint_least64_t	uint_least64;

	// Signed Integers
	typedef int_least8_t	int_least8;
	typedef int_least16_t	int_least16;
	typedef int_least32_t	int_least32;
	typedef int_least64_t	int_least64;
#pragma endregion

#pragma region Fast Integers
	// Unsigned Integers
	typedef uint_fast8_t	uint_fast8;
	typedef uint_fast16_t	uint_fast16;
	typedef uint_fast32_t	uint_fast32;
	typedef uint_fast64_t	uint_fast64;

	// Signed Integers
	typedef int_fast8_t		int_fast8;
	typedef int_fast16_t	int_fast16;
	typedef int_fast32_t	int_fast32;
	typedef int_fast64_t	int_fast64;
#pragma endregion


	// TODO: Radian
	// TODO: Degree
	// TODO: Angle - Dynamic data type that is default setting of either radian or degree

	// TODO: Endianness and auto conversions

	/// <summary>
	/// Swap the endianness.
	/// </summary>
	/// <param name="value">The Value.</param>
	/// <returns></returns>
	GRAVAPI inline uint16 swapU16(uint16 value)
	{
		return	((value & 0x00FF) << 8) 
			|	((value & 0xFF00) >> 8);
	}

	/// <summary>
	/// Swap the endianness.
	/// </summary>
	/// <param name="value">The Value.</param>
	/// <returns></returns>
	GRAVAPI inline uint32 swapU32(uint32 value)
	{
		return	((value & 0x000000FF) << 24) 
			|	((value & 0x0000FF00) << 8)
			|	((value & 0x00FF0000) >> 8)
			|	((value & 0xFF000000) >> 24);
	}

	/// <summary>
	/// Swap the endianness.
	/// </summary>
	/// <param name="value">The Value.</param>
	/// <returns></returns>
	GRAVAPI inline uint64 swapU64(uint64 value)
	{
		return	((value & 0x00000000000000FF) << 56) 
			|	((value & 0x000000000000FF00) << 40)
			|	((value & 0x0000000000FF0000) << 24)
			|	((value & 0x00000000FF000000) << 8)
			|	((value & 0x000000FF00000000) >> 8)
			|	((value & 0x0000FF0000000000) >> 24)
			|	((value & 0x00FF000000000000) >> 40)
			|	((value & 0xFF00000000000000) >> 56);
	}
}
