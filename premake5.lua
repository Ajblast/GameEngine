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
IncludeDir["GLFW"]	=				"vendor/glfw/include"
IncludeDir["GLAD"]	=				"vendor/glad/include"
IncludeDir["ImGui"] =				"vendor/imgui"
IncludeDir["glm"]	=				"vendor/glm"
IncludeDir["stb_image"] =			"vendor/stb_image"
IncludeDir["pytorch_debug"]	=		"vendor\\pytorch-debug\\include"
IncludeDir["pytorch2_debug"]	=	"vendor\\pytorch-debug\\include\\torch\\csrc\\api\\include"
IncludeDir["pytorch_release"]	=	"vendor\\pytorch-release\\include"
IncludeDir["pytorch2_release"]	=	"vendor\\pytorch-release\\include\\torch\\csrc\\api\\include"

LibDir = {}
LibDir["pytorchlib_debug"] = 		"vendor\\pytorch-debug\\lib"
LibDir["pytorchbin_debug"] = 		"vendor\\pytorch-debug\\bin"
LibDir["pytorchlib_release"] = 		"vendor\\pytorch-release\\lib"
LibDir["pytorchbin_release"] = 		"vendor\\pytorch-release\\bin"


--Projects
group "Dependencies"
	include "core/vendor/glfw"
	include "core/vendor/glad"
	include "core/vendor/imgui"
	--include "OpenGL-Core/vendor/GLFW"
	--include "OpenGL-Core/vendor/GLAD"
	--include "OpenGL-Core/vendor/imgui"
	include "core/vendor/glm"
group ""

include "core"
include "test"