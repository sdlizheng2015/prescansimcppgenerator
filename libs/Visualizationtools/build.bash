#! /usr/bin/env bash

source set_env.bash

if [ -e install ]; then
   rm -r install
fi

cmake -B build -S ./

cmake --build build --config Release --target INSTALL

if [ -e build ]; then
   rm -r build
fi