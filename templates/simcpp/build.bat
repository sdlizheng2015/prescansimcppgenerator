@ECHO OFF
setlocal

set "PATH=D:\Simcenter Prescan\Prescan_2302\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\FullWaveformLidarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\PBRadarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\PointCloudLidarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\ProbabilisticSensorsPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\V2XPlugin\bin;%PATH%"

cmake -B build -S ./ -G "Visual Studio 16 2019" 

cmake --build build --config Debug

cmd