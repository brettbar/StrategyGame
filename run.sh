#!/bin/bash

set -e
cmake -S . -B ./build/
make -C ./build
./build/game
