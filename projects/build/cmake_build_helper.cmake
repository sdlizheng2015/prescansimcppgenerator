file(TO_NATIVE_PATH "${DemoTarget}" native_demo_path)
file(TO_NATIVE_PATH "C:/Users/yiyan5ez/Desktop/PrescanSimcppGenerator" native_experiment_path)
file(TO_NATIVE_PATH "D:/Simcenter Prescan/Prescan_2302/bin;D:/Simcenter Prescan/Prescan_2302/Plugins/FullWaveformLidarPlugin/./bin;D:/Simcenter Prescan/Prescan_2302/Plugins/PBRadarPlugin/./bin;D:/Simcenter Prescan/Prescan_2302/Plugins/PointCloudLidarPlugin/./bin;D:/Simcenter Prescan/Prescan_2302/Plugins/ProbabilisticSensorsPlugin/bin;D:/Simcenter Prescan/Prescan_2302/Plugins/V2XPlugin/bin" native_prescan_runtime_paths)
file(WRITE "C:/Users/yiyan5ez/Desktop/PrescanSimcppGenerator/projects/build/psrun.bat"
   "@ECHO OFF\n"
   "setlocal\n"
   "set \"PATH=${native_prescan_runtime_paths};%PATH%\"\n"
   "echo Starting ${native_demo_path}...\n"
   "start \"SimCppProject\" /D \"${native_experiment_path}\" /WAIT /B \"${native_demo_path}\"\n"
)