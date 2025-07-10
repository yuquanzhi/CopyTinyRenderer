project "preTest"
		location "preTest"
		kind "ConsoleAPP"
		language "C++"
		staticruntime "On"
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")	

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"src/**.h",
			"src/**.cpp",

		}

		includedirs
		{
			"src"
		}

        filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"


		filter "configurations:Debug"
		defines "LE_DEBUG"
		runtime "Debug"
		symbols "On"
		filter "configurations:Release"
		defines "LE_RELEASE"
		runtime "Release"
		optimize "On"
		filter "configurations:Dist"
		defines "LE_DIST"
		runtime "Release"
		optimize "On"