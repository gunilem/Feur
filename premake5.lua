workspace "Feur"
    architecture "x64"

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Feur"
    location "Feur"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "fpch.h"
    pchsource "Feur/src/fpch.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        buildoptions "/utf-8"

        defines{
            "F_PLATFORM_WINDOWS",
            "F_BUILD_DLL"
        }

        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "F_DEBUG"
        symbols "On"
        buildoptions "/utf-8"

        
    filter "configurations:Release"
        defines "F_RELEASE"
        optimize "On"
        buildoptions "/utf-8"

    
    filter "configurations:Dist"
        defines "F_DIST"
        optimize "On"
        buildoptions "/utf-8"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "Feur/vendor/spdlog/include",
        "Feur/src"
    }

    links{
        "Feur"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        buildoptions "/utf-8"

        defines{
            "F_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "F_DEBUG"
        symbols "On"
        buildoptions "/utf-8"

        
    filter "configurations:Release"
        defines "F_RELEASE"
        optimize "On"
        buildoptions "/utf-8"

    
    filter "configurations:Dist"
        defines "F_DIST"
        optimize "On"
        buildoptions "/utf-8"