@ECHO OFF
setlocal

set "Generator_dir=E:\Siemens\1_Prescan\3_Experiments\33_SimcppGenerator\PrescanSimcppGenerator"

:::: add cmake config path
set "Eigen3_DIR=%Generator_dir%\libs\Eigen3\share\eigen3\cmake"

echo "set path ..."
cmd