@ECHO OFF
setlocal

set "Prescan_path=D:\Simcenter Prescan\Prescan_2307"
set "Prescan_plugin_path=D:\Simcenter Prescan\Prescan_2307\Plugins"
set "Generator_dir=E:\Siemens\1_Prescan\3_Experiments\33_SimcppGenerator\PrescanSimcppGenerator"

:::: add Prescan modules and plugin dependency path(please comment out(::) these plugins which are not installed on your machine)
set "PATH=%PATH%;%Prescan_path%\bin;%"
set "PATH=%PATH%;%Prescan_plugin_path%\FullWaveformLidarPlugin\bin;"
set "PATH=%PATH%;%Prescan_plugin_path%\PBRadarPlugin\bin;"
set "PATH=%PATH%;%Prescan_plugin_path%\PointCloudLidarPlugin\bin;"
set "PATH=%PATH%;%Prescan_plugin_path%\ProbabilisticSensorsPlugin\bin;"
set "PATH=%PATH%;%Prescan_plugin_path%\PhysicsBasedCameraUnreal;"
set "PATH=%PATH%;%Prescan_plugin_path%\V2XPlugin\bin;"

:::: add cmake config path
set "Prescan_DIR=%Prescan_path%\lib\cmake"
set "Eigen3_DIR=%Generator_dir%\libs\Eigen3\share\eigen3\cmake"

:::: if you have python in system, please ignor and delete it
set "PATH=D:\Python38\Scripts\;D:\Python38\;%PATH%"

:::: add Prescan python API path
set "PYTHONPATH=%PYTHONPATH%;%Prescan_path%\python;"
set "PYTHONPATH=%PYTHONPATH%;%Prescan_path%\modules;"
set "PYTHONPATH=%PYTHONPATH%;%Prescan_plugin_path%;"

:: add current Prescan experiment and destination simcpp project path
set "experiment_dir=%Generator_dir%\prescandemos\SimcppGenerator"
set "experiment_pb=%Generator_dir%\prescandemos\SimcppGenerator\SimcppGenerator.pb"
set "simcpp_dir=%Generator_dir%\prescandemos\SimcppGenerator\simcpp"

echo "set path ..."
cmd