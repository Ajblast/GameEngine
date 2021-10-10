project "UnitTesting"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("..\\bin\\" .. outputdir .. "\\%{prj.name}")
	objdir ("..\\bin-int\\" .. outputdir .. "\\%{prj.name}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
		"src\\**.h",
		"src\\**.cpp"
	}

	includedirs
	{
		"src",
		"..\\core\\src",
		"..\\core\\vendor",
		"..\\core\\%{IncludeDir.glm}",
		"$(VCInstallDir)Auxiliary\\VS\\UnitTest\\include"
	}

	links
	{
		"GRAVCore"
	}

	libdirs
	{
		"$(VCInstallDir)Auxiliary\\VS\\UnitTest\\lib"
	}

	postbuildcommands
	{
		"{COPYDIR} ..\\bin\\" .. outputdir .. "\\" .. coreName .. " ..\\bin\\" .. outputdir .. "\\%{prj.name}"
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
			"%{LibDir.pytorchlib_debug}\\c10d",
			"%{LibDir.pytorchlib_debug}\\caffe2_detectron_ops",
			"%{LibDir.pytorchlib_debug}\\caffe2_module_test_dynamic",
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
			"%{LibDir.pytorchlib_debug}\\XNNPACK"		
		}
		
		postbuildcommands
		{
			"{COPYDIR} %{LibDir.pytorchbin_debug} " .. "..\\bin\\" .. outputdir .. "\\%{prj.name}"
		}
		
	filter "configurations:Release"
		defines "GRAVCORE_RELEASE"
		runtime "Release"
        optimize "on"
		
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
			"%{LibDir.pytorchlib_release}\\c10d",
			"%{LibDir.pytorchlib_release}\\caffe2_detectron_ops",
			"%{LibDir.pytorchlib_release}\\caffe2_module_test_dynamic",
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
			"%{LibDir.pytorchlib_release}\\XNNPACK"		
		}
		
		postbuildcommands
		{
			"{COPYDIR} %{LibDir.pytorchbin_release} " .. "..\\bin\\" .. outputdir .. "\\%{prj.name}"
		}