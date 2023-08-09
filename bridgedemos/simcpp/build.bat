@ECHO OFF
setlocal

cmake -B build -S ./ -G "Visual Studio 16 2019" -DCMAKE_TOOLCHAIN_FILE=%vcpkg%

cmake --build build --config Release

