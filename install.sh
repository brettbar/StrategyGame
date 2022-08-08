#!/bin/bash

mkdir tmp && cd tmp
git clone https://github.com/raysan5/raylib
cd raylib
mkdir build && cd build
cmake .. -DCUSTOMIZE_BUILD=ON -DSUPPORT_FILEFORMAT_BMP=ON
sudo make install
