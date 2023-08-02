#! /usr/bin/env bash

# add prescan
export Prescan_version=Prescan_2302
export PATH=$PATH:/usr/local/${Prescan_version}/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/${Prescan_version}/lib

# add Prescan Plugin path
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/${Prescan_version}/<path_to_plugin>/bin

export Prescan_DIR=/usr/local/${Prescan_version}/lib/cmake

export PYTHONPATH=$PYTHONPATH:/usr/local/${Prescan_version}/python:/usr/local/${Prescan_version}/modules:/usr/local/${Prescan_version}/Plugins

export experiment_dir=/home/yang/Desktop/PrescanSimcppGenerator/prescandemos/SimcppGenerator
export experiment_pb=/home/yang/Desktop/PrescanSimcppGenerator/prescandemos/SimcppGenerator/SimcppGenerator.pb
export simcpp_dir=/home/yang/Desktop/PrescanSimcppGenerator/prescandemos/SimcppGenerator/simcpp