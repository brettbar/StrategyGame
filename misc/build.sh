# SWITCH TO CMAKE
set -e
clang++ -g -std=c++17 -c main.cpp -o ./obj/main.o
clang++ -o main obj/main.o -s -std=c++17 -O2 -Wall -lraylib -lm -lpthread -ldl -lrt -lX11
./main
