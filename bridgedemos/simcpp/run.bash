#! /usr/bin/env bash

source set_env.bash

cd $experiment_dir

prescanrun $simcpp_dir/build/SimCppProject $experiment_pb 10

