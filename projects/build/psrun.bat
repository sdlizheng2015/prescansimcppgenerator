@ECHO OFF
setlocal
set "PATH=D:\Simcenter Prescan\Prescan_2302\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\FullWaveformLidarPlugin\.\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\PBRadarPlugin\.\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\PointCloudLidarPlugin\.\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\ProbabilisticSensorsPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\V2XPlugin\bin;%PATH%"
echo Starting C:\Users\yiyan5ez\Desktop\PrescanSimcppGenerator\projects\build\Debug\SimCppProject.exe...
start "SimCppProject" /D "C:\Users\yiyan5ez\Desktop\PrescanSimcppGenerator" /WAIT /B "C:\Users\yiyan5ez\Desktop\PrescanSimcppGenerator\projects\build\Debug\SimCppProject.exe"
