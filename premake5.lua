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
	coreName = "GRAVCore"

	-- Include directories relative to OpenGL-Core
	IncludeDir = {}
	IncludeDir["GLFW"]	=				"vendor\\glfw\\include"
	IncludeDir["GLAD"]	=				"vendor\\glad\\include"
	IncludeDir["ImGui"] =				"vendor\\imgui"
	IncludeDir["glm"]	=				"vendor\\glm"
	IncludeDir["stb_image"] =			"vendor\\stb_image"
	IncludeDir["pytorch_debug"]	=		"%{wks.location}\\core\\vendor\\pytorch-debug\\include"
	IncludeDir["pytorch2_debug"]	=	"%{wks.location}\\core\\vendor\\pytorch-debug\\include\\torch\\csrc\\api\\include"
	IncludeDir["pytorch_release"]	=	"%{wks.location}\\core\\vendor\\pytorch-release\\include"
	IncludeDir["pytorch2_release"]	=	"%{wks.location}\\core\\vendor\\pytorch-release\\include\\torch\\csrc\\api\\include"

	LibDir = {}
	LibDir["pytorchlib_debug"] = 		"%{wks.location}\\core\\vendor\\pytorch-debug\\lib"
	LibDir["pytorchbin_debug"] = 		"%{wks.location}\\core\\vendor\\pytorch-debug\\bin"
	LibDir["pytorchlib_release"] = 		"%{wks.location}\\core\\vendor\\pytorch-release\\lib"
	LibDir["pytorchbin_release"] = 		"%{wks.location}\\core\\vendor\\pytorch-release\\bin"

	--Projects
	group "Dependencies"
		include "core\\vendor\\glfw"
		include "core\\vendor\\glad"
		include "core\\vendor\\imgui"
		include "core\\vendor\\glm"
	group ""

	include "core"
	include "test"

	group "UnitTests"
		include "testing"
