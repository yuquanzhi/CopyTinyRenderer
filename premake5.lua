workspace "tinyRenderer"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
outputdir="%{cfg.buildcfg}-%{cfg.syetem}-%{cfg.architecture}"

    include "_0_preTest.lua"
	include "_1_Bresenham.lua"
	include "_2_trigngle.lua"
	include "_3_zbuffer.lua"