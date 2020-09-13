cmake_minimum_required (VERSION 3.15)

# Platform

if (WIN32 OR MINGW)
	include ("${PROJECT_SOURCE_DIR}/cmake/platforms/win32.cmake")
elseif (UNIX)
	include ("${PROJECT_SOURCE_DIR}/cmake/platforms/unix.cmake")
else ()
	message (FATAL_ERROR "Undefined platform!")
endif ()

# Find Packages/Libraries

include ("${PROJECT_SOURCE_DIR}/cmake/modules/FindFMT.cmake")
if (NOT FMT_FOUND)
	message (FATAL_ERROR "Could not find fmt!")
endif ()

if (VCPKG_TOOLCHAIN)
	find_package (OpenAL CONFIG REQUIRED)
	set (OPENAL_LIBRARY OpenAL::OpenAL)
else ()
	include ("${PROJECT_SOURCE_DIR}/cmake/modules/FindOpenAL.cmake")
	if (NOT OPENAL_FOUND)
		message (FATAL_ERROR "Could not find OpenAL!")
	endif ()
endif ()

set (GENERAL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

# Link/Include

target_link_libraries (example PRIVATE
	${FMT_LIBRARY}
	${OPENAL_LIBRARY}
)
target_include_directories (example PRIVATE
	${GENERAL_INCLUDE_DIR}
	${FMT_INCLUDE_DIR}
	${OPENAL_INCLUDE_DIR}
)