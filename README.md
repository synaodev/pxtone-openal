# Pxtone OpenAL Example
This shows how to use Daisuke Amaya's Pxtone library in conjunction with OpenAL.
## Required Dependencies
- [fmt](https://github.com/fmtlib/fmt)
- [OpenAL](https://github.com/kcat/openal-soft)
## Included Dependencies
- [Pxtone](https://pxtone.org/developer)
## Building From Source
- General:
  - CMake version must be at least 3.15.
  - Compiler must support at least C++11 and C99.
  - To manage dependencies outside of Linux, I recommend [vcpkg](https://github.com/microsoft/vcpkg).
  - Install dependencies on vcpkg like this: `<vcpkg-cli> install openal-soft fmt`.
  - Then, when running cmake, pass the toolchain file: `cmake <build-root> -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake`.
- Linux:
  - You should use your package manager to install dependencies:
    - Apt: `apt-get install libopenal-dev libfmt-dev`
    - Pacman: `pacman -S openal fmt`
    - Yum: `yum install openal-soft-devel fmt-devel`
- MacOS:
  - Vcpkg is absolutely required.
  - Supports only AppleClang.
  - When installing dependencies with vcpkg, only install fmt. openal-soft usually gets ignored in favor of OpenAL.Framework.
- Windows:
  - Vcpkg is absolutely required.
  - Supports MSVC, Clang, and MinGW (Posix threading model only). Cygwin environment is not supported. You can also use MinGW & vcpkg on Linux for cross-compiling.
  - Additionally, for MinGW, you need to build vcpkg dependencies using a MinGW triplet.
  - To do this, open `<vcpkg-root>/scripts/toolchains/mingw.cmake` and add the following lines:
    - `set(CMAKE_C_COMPILER "<mingw-gcc-posix>")`
    - `set(CMAKE_CXX_COMPILER "<mingw-g++-posix>")`
    - `set(CMAKE_FIND_ROOT_PATH "<paths-to-additional-dependencies>")`
    - `set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)`
    - `set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)`
    - `set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)`
    - `set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)`
  - The "additional dependencies" variable needs to be set to directories containing these libraries:
    - libwinpthread-1.dll
    - libgcc_s_seh-1.dll
    - libstdc++-6.dll
  - When running cmake, you then need to specifiy your target triplet and your compiler locations like this: `cmake <build-root> -G "MinGW Makefiles" -DCMAKE_C_COMPILER=<mingw-gcc-posix> -DCMAKE_CXX_COMPILER=<mingw-g++-posix> -DCMAKE_RC_COMPILER=<mingw-windres> -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-<linking-type>`
  - If cross-compiling, leave out the `-G "MinGW Makefiles"` argument.
