@ECHO OFF
setlocal

set "PATH=D:\Simcenter Prescan\Prescan_2302\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\FullWaveformLidarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\PBRadarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\PointCloudLidarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\ProbabilisticSensorsPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\V2XPlugin\bin;%PATH%"

echo set path ...

cd "C:\Users\yiyan5ez\Desktop\SimcppGenerator"


prescanrun "C:\Users\yiyan5ez\Desktop\PrescanSimcppGenerator\projects\build\Debug\SimCppProject.exe" SimcppGenerator.pb 2 && cmd

