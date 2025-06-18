workspace "Feur"
    architecture "x64"

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Feur/vendor/GLFW/include"
IncludeDir["glm"] = "Feur/vendor/glm"

group "Dependencies"
    include "Feur/vendor/GLFW"
group ""

project "Feur"
    location "Feur"
    kind "ConsoleApp"

    language "C++"
    cppdialect "C++17"

    buildoptions "/utf-8"
    staticruntime "on"


    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "fpch.h"
    pchsource "Feur/src/fpch.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs{
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{os.getenv('VULKAN_SDK')}/Include"
    }
    
    libdirs {
        "%{os.getenv('VULKAN_SDK')}/Lib"
    }
    
    links{
        "GLFW",
        "glm",
        "Vulkan-1"
    }


    filter "system:windows"
        systemversion "latest"

        defines{
            "F_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "F_DEBUG"
        runtime "Debug"
        symbols "on"

        
    filter "configurations:Release"
        defines "F_RELEASE"
        runtime "Release"
        optimize "on"

    
    filter "configurations:Dist"
        defines "F_DIST"
        runtime "Release"
        optimize "on"

