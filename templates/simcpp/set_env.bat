@ECHO OFF
setlocal

set "PATH=D:\Simcenter Prescan\Prescan_2302\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\FullWaveformLidarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\PBRadarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\PointCloudLidarPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\ProbabilisticSensorsPlugin\bin;D:\Simcenter Prescan\Prescan_2302\Plugins\V2XPlugin\bin;%PATH%"

set "Prescan_DIR=D:\Simcenter Prescan\Prescan_2302\lib\cmake"

set "PATH=D:\Python38\Scripts\;D:\Python38\;%PATH%"

set "PYTHONPATH=D:\Simcenter Prescan\Prescan_2302\python;D:\Simcenter Prescan\Prescan_2302\modules;D:\Simcenter Prescan\Prescan_2302\Plugins;%PYTHONPATH%"

set experiment_dir="C:\\Users\\yiyan5ez\\Desktop\\SimcppGenerator"

set experiment_pb="C:\\Users\\yiyan5ez\\Desktop\\SimcppGenerator\\SimcppGenerator.pb"

set simcpp_dir="C:\\Users\\yiyan5ez\\Desktop\\SimcppGenerator\\simcpp"

echo set path ...

cmd