cmake_minimum_required (VERSION 3.15)

# Building with ccache

if (WIN32 OR APPLE)
	if (CCACHE_BUILD)
		message ("Cannot use ccache on this system! Sorry!")
	endif ()
	set (CCACHE_BUILD OFF)
elseif (NOT DEFINED CCACHE_BUILD)
	message ("Defaulting to building with ccache, if possible...")
	set (CCACHE_BUILD ON)
endif ()

if (CCACHE_BUILD)
	find_program (CCACHE_PROGRAM "ccache")
	if (CCACHE_PROGRAM)
		set_property (GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
	else ()
		message ("Couldn't find ccache! Sorry!")
	endif ()
endif ()

# Building with gold linker

if (NOT DEFINED GOLD_BUILD)
	message ("Defaulting to building without gold linker...")
	set (GOLD_BUILD OFF)
elseif (UNIX AND NOT APPLE)
	message ("Warning! Make sure you have gold linker installed!")
	set (GOLD_BUILD ON)
else ()
	message ("Gold linker is unavailable! Sorry!")
endif ()

# Project definition

project (PxtoneOpenALExample)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_C_STANDARD 99)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_C_EXTENSIONS OFF)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Targets

add_executable (example)

# Configure

add_subdirectory ("cmake")

# Source

add_subdirectory ("source")

# Libraries

add_subdirectory ("pxtone")
