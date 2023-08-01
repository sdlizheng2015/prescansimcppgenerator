@ECHO OFF
setlocal


cd %experiment_dir%


prescanrun %simcpp_dir%\\build\\Release\\SimCppProject.exe %experiment_pb% 10

