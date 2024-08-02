-- global specification
workspace "LangF"
architecture "x86_64"
configurations { "Debug", "Release" };
startproject "Example"

dir = "%{wks.location}/Bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
obj = "%{wks.location}/Bin/temp/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- example project
include "Example"