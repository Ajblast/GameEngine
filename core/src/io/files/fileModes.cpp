#include "gravpch.h"
#include "fileModes.h"
#include <type_traits>

namespace GRAVEngine
{
	namespace IO
	{
        fileMode operator |(fileMode lhs, fileMode rhs)
        {
            return static_cast<fileMode>(
                static_cast<std::underlying_type<fileMode>::type>(lhs) |
                static_cast<std::underlying_type<fileMode>::type>(rhs)
                );
        }
        fileMode operator &(fileMode lhs, fileMode rhs)
        {
            return static_cast<fileMode> (
                static_cast<std::underlying_type<fileMode>::type>(lhs) &
                static_cast<std::underlying_type<fileMode>::type>(rhs)
                );
        }
        fileMode operator ^(fileMode lhs, fileMode rhs)
        {
            return static_cast<fileMode> (
                static_cast<std::underlying_type<fileMode>::type>(lhs) ^
                static_cast<std::underlying_type<fileMode>::type>(rhs)
                );
        }
        fileMode operator ~(fileMode rhs)
        {
            return static_cast<fileMode> (
                ~static_cast<std::underlying_type<fileMode>::type>(rhs)
                );
        }

        fileMode& operator |=(fileMode& lhs, fileMode rhs)
        {
            lhs = static_cast<fileMode> (
                static_cast<std::underlying_type<fileMode>::type>(lhs) |
                static_cast<std::underlying_type<fileMode>::type>(rhs)
                );

            return lhs;
        }
        fileMode& operator &=(fileMode& lhs, fileMode rhs)
        {
            lhs = static_cast<fileMode> (
                static_cast<std::underlying_type<fileMode>::type>(lhs) &
                static_cast<std::underlying_type<fileMode>::type>(rhs)
                );

            return lhs;
        }
        fileMode& operator ^=(fileMode& lhs, fileMode rhs)
        {
            lhs = static_cast<fileMode> (
                static_cast<std::underlying_type<fileMode>::type>(lhs) ^
                static_cast<std::underlying_type<fileMode>::type>(rhs)
                );

            return lhs;
        }

	}
}