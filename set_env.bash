#! /usr/bin/env bash

# add prescan
export Prescan_version=Prescan_2302
export PATH=$PATH:/usr/local/${Prescan_version}/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/${Prescan_version}/lib

export Prescan_DIR=/usr/local/${Prescan_version}/lib/cmake

export PYTHONPATH=$PYTHONPATH:/usr/local/Prescan_2302/python:/usr/local/Prescan_2302/modules

export experiment_dir=/home/yang/Desktop/SimcppGenerator
export experiment_pb=/home/yang/Desktop/SimcppGenerator/SimcppGenerator.pb

export simcpp_dir=/home/yang/Desktop/SimcppGenerator/simcpp
