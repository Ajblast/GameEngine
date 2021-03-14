project "GRAVTest"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		--"../OpenGL-Core/vendor/spdlog/include",
		"../core/src",
		--"../OpenGL-Core/vendor",
		"../core/%{IncludeDir.glm}"
		--"../OpenGL-Core/%{IncludeDir.Glad}",
		--"../OpenGL-Core/%{IncludeDir.ImGui}"
	}

	links
	{
		"GRAVCore"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GRAVCORE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GRAVCORE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GRAVCORE_RELEASE"
		runtime "Release"
        optimize "on"