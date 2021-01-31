project "GRAVCore"
	kind "StaticLib"	-- StaticLib for .lib
	language "C++"		-- C++
	cppdialect "C++17"	-- C++17
	staticruntime "on"	-- MultiThreaded

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gravpch.h"
	pchsource "src/gravpch.cpp"

	-- Files that should be included into this project
	files
	{
		"src/**.h",
		"src/**.cpp"
		--"vendor/stb_image/**.h",
		--"vendor/stb_image/**.cpp",
		--"vendor/glm/glm/**.hpp",
		--"vendor/glm/glm/**.inl",
	}

	-- Define the following
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	-- Include directories
	includedirs
	{
		"src",
		--"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}"
		--"%{IncludeDir.ImGui}",
		--"%{IncludeDir.glm}",
		--"%{IncludeDir.stb_image}"
	}

	-- Link these projects into the executable
	links 
	{ 
		"GLFW",
		"GLAD",
		--"ImGui",
		"opengl32.lib"
	}

	-- When the system is windows
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GRAVCORE_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	-- When the configuration is debug
	filter "configurations:Debug"
		defines "GRAVCORE_DEBUG"	-- Define in the project it is debug
		runtime "Debug"				-- Debug runtime optimization
		symbols "on"				-- Generate debug symbols

	-- When the configuration is Release
	filter "configurations:Release"
		defines "GRAVCORE_RELEASE"	-- Define in the project it is Release
		runtime "Release"			-- Release runtime optimization
		optimize "on"				-- Optimize the project (Balanced)