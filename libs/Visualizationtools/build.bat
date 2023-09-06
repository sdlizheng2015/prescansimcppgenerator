@ECHO OFF
setlocal


if exist install rmdir /s /q install

cmake -B build -S ./ -G "Visual Studio 16 2019" 

cmake --build build --config Release --target INSTALL

cmake --build build --config Debug --target INSTALL

::if exist build rmdir /s /q build