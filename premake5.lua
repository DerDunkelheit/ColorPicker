-- premake5.lua
workspace "ColorPicker"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "ColorPicker"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "WalnutExternal.lua"
include "ColorPicker"