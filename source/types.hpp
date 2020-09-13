#ifndef EXAMPLE_INCLUDED_TYPES_HPP
#define EXAMPLE_INCLUDED_TYPES_HPP

#if defined(_WIN32) || defined(__MINGW32__)
	#define EXAMPLE_PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
	#include <TargetConditionals.h>
	#if defined(TARGET_OS_MAC)
		#define EXAMPLE_PLATFORM_MACOS
		#define EXAMPLE_POSIX_COMPLIANT
	#else
		#error "Apple platform is not supported!"
	#endif
#elif defined(__unix__)
	#define EXAMPLE_POSIX_COMPLIANT
	#if defined(__linux__)
		#define EXAMPLE_PLATFORM_LINUX
	#elif defined(__bsdi__)
		#if defined(__FreeBSD__)
			#define EXAMPLE_PLATFORM_FREEBSD
		#elif defined(__OpenBSD__)
			#define EXAMPLE_PLATFORM_OPENBSD
		#elif defined(__NetBSD__)
			#define EXAMPLE_PLATFORM_NETBSD
		#elif defined(__DragonFly__)
			#define EXAMPLE_PLATFORM_DRAGONFLY
		#else
			#error "Unknown BSD platform is not supported!"
		#endif
	#elif defined(__CYGWIN__)
		#error "Cygwin is not supported!"
	#else
		#error "Unknown unix-like platform is not supported!"
	#endif
#else
	#error "Unknown platform is not supported!"
#endif

#if defined(__clang__)
	#define EXAMPLE_COMPILER_LLVM
	#if defined(_MSC_VER)
		#define EXAMPLE_TOOLCHAIN_MSVC
	#elif defined(__apple_build_version__)
		#define EXAMPLE_TOOLCHAIN_APPLECLANG
	#else
		#define EXAMPLE_TOOLCHAIN_LLVM
	#endif
#elif defined(__GNUC__)
	#define EXAMPLE_COMPILER_GNUC
	#if defined(__MINGW32__)
		#define EXAMPLE_TOOLCHAIN_MINGW
	#else
		#define EXAMPLE_TOOLCHAIN_GNUC
	#endif
#elif defined(_MSC_VER)
	#define EXAMPLE_COMPILER_MSVC
	#define EXAMPLE_TOOLCHAIN_MSVC
#else
	#error "Target compiler and toolchain are not supported!"
#endif

#if defined(EXAMPLE_PLATFORM_WINDOWS)
	#if defined(_WIN64)
		#define EXAMPLE_MACHINE_64BIT
	#else
		#define EXAMPLE_MACHINE_32BIT
	#endif
#elif defined(EXAMPLE_PLATFORM_MACOS)
	#define EXAMPLE_MACHINE_64BIT
#elif defined(EXAMPLE_COMPILER_GNUC) || defined(EXAMPLE_COMPILER_LLVM)
	#if __x86_64__ || __ppc64__
		#define EXAMPLE_MACHINE_64BIT
	#else
		#define EXAMPLE_MACHINE_32BIT
	#endif
#else
	#error "Can't determine if architecture is 32-bit or 64-bit!"
#endif

#if !defined(NDEBUG)
	#define EXAMPLE_BUILD_DEBUG
#endif

#include <cstdint>

#endif // EXAMPLE_INCLUDED_TYPES_HPP
