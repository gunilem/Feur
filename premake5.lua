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

include "Feur/vendor/GLFW"
include "Feur/vendor/GLAD"
include "Feur/vendor/imgui"

project "Feur"
    location "Feur"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "fpch.h"
    pchsource "Feur/src/fpch.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.Imgui}",
        "%{IncludeDir.glm}",
    }

    links{
        "GLFW",
        "GLAD",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
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
        runtime "Debug"
        buildoptions "/utf-8"

        
    filter "configurations:Release"
        defines "F_RELEASE"
        optimize "On"
        runtime "Release"
        buildoptions "/utf-8"

    
    filter "configurations:Dist"
        defines "F_DIST"
        optimize "On"
        runtime "Release"
        buildoptions "/utf-8"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "Feur/vendor/spdlog/include",
        "Feur/src",
        "%{IncludeDir.glm}",
    }

    links{
        "Feur"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        buildoptions "/utf-8"

        defines{
            "F_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "F_DEBUG"
        symbols "On"
        runtime "Debug"
        buildoptions "/utf-8"

        
    filter "configurations:Release"
        defines "F_RELEASE"
        optimize "On"
        runtime "Release"
        buildoptions "/utf-8"

    
    filter "configurations:Dist"
        defines "F_DIST"
        optimize "On"
        runtime "Release"
        buildoptions "/utf-8"