#!/bin/bash

mkdir -p tmp && cd tmp
git clone https://github.com/raysan5/raylib
cd raylib
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON -DCUSTOMIZE_BUILD=ON -DSUPPORT_FILEFORMAT_BMP=ON ..
sudo make install
