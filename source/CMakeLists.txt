cmake_minimum_required (VERSION 3.15)

if (VCPKG_TOOLCHAIN)
	target_compile_definitions (example PRIVATE "-DEXAMPLE_USES_VCPKG")
endif ()

target_sources (example PRIVATE "al-check.cpp" "main.cpp")
