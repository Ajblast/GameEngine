#pragma once

#include "CppUnitTest.h"
#include "CppUnitTestAssert.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <codecvt>
#include <string>

inline std::wstring stringToWString(const std::string& string)
{
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(string);
}