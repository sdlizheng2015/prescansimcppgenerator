#! /usr/bin/env bash

source set_env.bash

cmake -B build -S ./

cmake --build build --config Release

