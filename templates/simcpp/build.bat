@ECHO OFF
setlocal

cmake -B build -S ./ -G "Visual Studio 16 2019" 

cmake --build build --config Release

