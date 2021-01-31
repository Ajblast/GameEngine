-- GRAVEngine
workspace "GRAVEngine"
	architecture "x64"
	startproject "GRAVTest"


	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

-- The output directory
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to OpenGL-Core
IncludeDir = {}
--IncludeDir["GLFW"]	=		"vendor/GLFW/include"
--IncludeDir["Glad"]	=		"vendor/Glad/include"
--IncludeDir["ImGui"] =		"vendor/imgui"
IncludeDir["glm"]	=		"vendor/glm"
--IncludeDir["stb_image"] =	"vendor/stb_image"


--Projects
group "Dependencies"
	--include "OpenGL-Core/vendor/GLFW"
	--include "OpenGL-Core/vendor/GLAD"
	--include "OpenGL-Core/vendor/imgui"
	include "core/vendor/glm"
group ""

include "core"
include "test"




--OpenGL Examples
--workspace "OpenGL-Examples"
--	startproject "OpenGL-Examples"
--	architecture "x64"
--	startproject "OpenGL-Examples"
--	   
--	configurations
--	{
--		"Debug",
--		"Release"
--	}
--
--	flags
--	{
--		"MultiProcessorCompile"
--	}
--
--outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
--
---- Include directories relative to OpenGL-Core
--IncludeDir = {}
--IncludeDir["GLFW"]	=		"vendor/GLFW/include"
--IncludeDir["Glad"]	=		"vendor/Glad/include"
--IncludeDir["ImGui"] =		"vendor/imgui"
--IncludeDir["glm"]	=		"vendor/glm"
--IncludeDir["stb_image"] =	"vendor/stb_image"
--
---- Projects
--group "Dependencies"
--	includeexternal "OpenGL-Core/vendor/GLFW"
--	includeexternal "OpenGL-Core/vendor/Glad"
--	includeexternal "OpenGL-Core/vendor/imgui"
--group ""
--
--includeexternal "OpenGL-Core"
--include "OpenGL-Examples"