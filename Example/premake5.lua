project "Example"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir(dir)
    objdir(obj)

    files
    {
        "%{wks.location}/langf.h",  "%{wks.location}/langf.cpp", "%{wks.location}/langf_translation.h",
        "example.cpp"
    }

    includedirs
    {
        "%{wks.location}"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"