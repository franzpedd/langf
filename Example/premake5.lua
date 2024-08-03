project "Example"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir(dir)
    objdir(obj)

    files
    {
        "%{wks.location}/langf.h",
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