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
IncludeDir["GLAD"] = "Feur/vendor/GLAD/include"
IncludeDir["Imgui"] = "Feur/vendor/imgui"
IncludeDir["glm"] = "Feur/vendor/glm"
IncludeDir["stb_image"] = "Feur/vendor/stb_image"

include "Feur/vendor/GLFW"
include "Feur/vendor/GLAD"
include "Feur/vendor/imgui"

project "Feur"
    location "Feur"
    kind "StaticLib"

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
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.Imgui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
    }

    links{
        "GLFW",
        "GLAD",
        "ImGui",
        "opengl32.lib"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
        systemversion "latest"

        defines{
            "F_PLATFORM_WINDOWS",
            "F_BUILD_DLL"
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


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"

    language "C++"
    cppdialect "C++17"

    buildoptions "/utf-8"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "Feur/vendor/spdlog/include",
        "Feur/src",
        "Feur/vendor",
        "%{IncludeDir.glm}",
    }

    links{
        "Feur",
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