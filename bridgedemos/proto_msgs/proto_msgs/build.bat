@echo off

echo --------------------cmake----------------------------------------

cmake -B build -S ./ -G "Visual Studio 16 2019" -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows


echo --------------------make-----------------------------------------

cmake --build build --config Release

pause