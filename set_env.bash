#! /usr/bin/env bash

export Prescan_path=/usr/local/Prescan_2307
export Prescan_plugin_path=/usr/local/Prescan_2307/Plugins
export Generator_dir=/home/yang/Desktop/PrescanSimcppGenerator

### add Prescan modules and plugin dependency path(please comment out(::) these plugins which are not installed on your machine)
export PATH=$PATH:$Prescan_path/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$Prescan_path/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$Prescan_plugin_path/FullWaveformLidarPlugin/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$Prescan_plugin_path/PBRadarPlugin/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$Prescan_plugin_path/PointCloudLidarPlugin/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$Prescan_plugin_path/ProbabilisticSensorsPlugin/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$Prescan_plugin_path/PhysicsBasedCameraUnreal
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$Prescan_plugin_path/V2X/(not supported on linux)

### add cmake config path
export Prescan_DIR=$Prescan_path/lib/cmake
export Eigen3_DIR=$Generator_dir/libs/Eigen3/share/eigen3/cmake

### add Prescan python API path
export PYTHONPATH=$PYTHONPATH:$Prescan_path/python
export PYTHONPATH=$PYTHONPATH:$Prescan_path/modules
export PYTHONPATH=$PYTHONPATH:$Prescan_plugin_path

### add current Prescan experiment and destination simcpp project path
export experiment_dir=$Generator_dir/prescandemos/SimcppGenerator
export experiment_pb=$Generator_dir/prescandemos/SimcppGenerator/SimcppGenerator.pb
export simcpp_dir=$Generator_dir/prescandemos/SimcppGenerator/simcpp

echo "set path ..."