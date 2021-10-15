project "GRAVCore"
	kind "SharedLib"	-- SharedLib for .lib
	language "C++"		-- C++
	cppdialect "C++17"	-- C++17
	staticruntime "off"	-- MultiThreaded

	targ = "..\\bin\\" .. outputdir .. "\\" .. coreName
	targetdir (targ)
	out = "..\\bin-int\\" .. outputdir .. "\\" .. coreName
	objdir (out)

	pchheader "gravpch.h"
	pchsource "src/gravpch.cpp"

	-- Files that should be included into this project
	files
	{
		"src\\**.h",
		"src\\**.cpp",
		"vendor\\stb_image\\**.h",
		"vendor\\stb_image\\**.cpp",
		"vendor\\glm\\glm\\**.hpp",
		"vendor\\glm\\glm\\**.inl"
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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	-- Link these projects into the executable
	links 
	{ 
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	-- When the system is windows
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GRAVCORE_PLATFORM_WINDOWS",
			"GRAV_DLL",
			"GLFW_INCLUDE_NONE"
		}

	-- When the configuration is debug
	filter "configurations:Debug"
		defines "GRAVCORE_DEBUG"	-- Define in the project it is debug
		runtime "Debug"				-- Debug runtime optimization
		symbols "on"				-- Generate debug symbols

		includedirs
		{
			"%{IncludeDir.pytorch_debug}",
			"%{IncludeDir.pytorch2_debug}"
		}

		links 
		{
			-- PyTorch lib files
			"%{LibDir.pytorchlib_debug}\\asmjit",
			"%{LibDir.pytorchlib_debug}\\c10",
			"%{LibDir.pytorchlib_debug}\\c10_cuda",
			"%{LibDir.pytorchlib_debug}\\c10d",
			"%{LibDir.pytorchlib_debug}\\caffe2_detectron_ops",
			"%{LibDir.pytorchlib_debug}\\caffe2_detectron_ops_gpu",
			"%{LibDir.pytorchlib_debug}\\caffe2_module_test_dynamic",
			"%{LibDir.pytorchlib_debug}\\caffe2_nvrtc",
			"%{LibDir.pytorchlib_debug}\\Caffe2_perfkernels_avx",
			"%{LibDir.pytorchlib_debug}\\Caffe2_perfkernels_avx2",
			"%{LibDir.pytorchlib_debug}\\Caffe2_perfkernels_avx512",
			"%{LibDir.pytorchlib_debug}\\clog",
			"%{LibDir.pytorchlib_debug}\\cpuinfo",
			"%{LibDir.pytorchlib_debug}\\dnnl",
			"%{LibDir.pytorchlib_debug}\\fbgemm",
			"%{LibDir.pytorchlib_debug}\\fbjni",
			"%{LibDir.pytorchlib_debug}\\kineto",
			"%{LibDir.pytorchlib_debug}\\libprotobufd",
			"%{LibDir.pytorchlib_debug}\\libprotobuf-lited",
			"%{LibDir.pytorchlib_debug}\\libprotocd",
			"%{LibDir.pytorchlib_debug}\\mkldnn",
			"%{LibDir.pytorchlib_debug}\\pthreadpool",
			"%{LibDir.pytorchlib_debug}\\pytorch_jni",
			"%{LibDir.pytorchlib_debug}\\torch",
			"%{LibDir.pytorchlib_debug}\\torch_cpu",
			"%{LibDir.pytorchlib_debug}\\torch_cuda",
			"%{LibDir.pytorchlib_debug}\\XNNPACK"		
		}
		
	-- When the configuration is Release
	filter "configurations:Release"
		defines "GRAVCORE_RELEASE"	-- Define in the project it is Release
		runtime "Release"			-- Release runtime optimization
		optimize "on"				-- Optimize the project (Balanced)
		
		includedirs
		{
			"%{IncludeDir.pytorch_release}",
			"%{IncludeDir.pytorch2_release}"
		}

		links 
		{
			-- PyTorch lib files
			"%{LibDir.pytorchlib_release}\\asmjit",
			"%{LibDir.pytorchlib_release}\\c10",
			"%{LibDir.pytorchlib_release}\\c10_cuda",
			"%{LibDir.pytorchlib_release}\\c10d",
			"%{LibDir.pytorchlib_release}\\caffe2_detectron_ops",
			"%{LibDir.pytorchlib_release}\\caffe2_detectron_ops_gpu",
			"%{LibDir.pytorchlib_release}\\caffe2_module_test_dynamic",
			"%{LibDir.pytorchlib_release}\\caffe2_nvrtc",
			"%{LibDir.pytorchlib_release}\\Caffe2_perfkernels_avx",
			"%{LibDir.pytorchlib_release}\\Caffe2_perfkernels_avx2",
			"%{LibDir.pytorchlib_release}\\Caffe2_perfkernels_avx512",
			"%{LibDir.pytorchlib_release}\\clog",
			"%{LibDir.pytorchlib_release}\\cpuinfo",
			"%{LibDir.pytorchlib_release}\\dnnl",
			"%{LibDir.pytorchlib_release}\\fbgemm",
			"%{LibDir.pytorchlib_release}\\fbjni",
			"%{LibDir.pytorchlib_release}\\kineto",
			"%{LibDir.pytorchlib_release}\\libprotobuf",
			"%{LibDir.pytorchlib_release}\\libprotobuf-lite",
			"%{LibDir.pytorchlib_release}\\libprotoc",
			"%{LibDir.pytorchlib_release}\\mkldnn",
			"%{LibDir.pytorchlib_release}\\pthreadpool",
			"%{LibDir.pytorchlib_release}\\pytorch_jni",
			"%{LibDir.pytorchlib_release}\\torch",
			"%{LibDir.pytorchlib_release}\\torch_cpu",
			"%{LibDir.pytorchlib_release}\\torch_cuda",
			"%{LibDir.pytorchlib_release}\\XNNPACK"		
		}

		