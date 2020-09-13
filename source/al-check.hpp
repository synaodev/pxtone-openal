#ifndef EXAMPLE_INCLUDED_AL_CHECK_HPP
#define EXAMPLE_INCLUDED_AL_CHECK_HPP

#include "./types.hpp"

#if defined(EXAMPLE_PLATFORM_LINUX) || defined(EXAMPLE_USES_VCPKG)
	#include <AL/al.h>
	#include <AL/alc.h>
#else
	#include <al.h>
	#include <alc.h>
#endif

#ifdef EXAMPLE_BUILD_DEBUG
	namespace priv {
		void alCheckError(const char* file, uint32_t line, const char* expression);
	}
	#define alCheck(expr) do { expr; priv::alCheckError(__FILE__, __LINE__, #expr); } while(false)
#else
	#define alCheck(expr) (expr)
#endif

#endif // EXAMPLE_INCLUDED_AL_CHECK_HPP
