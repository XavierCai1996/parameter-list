#!/bin/bash

#build
cmake .
make

#clear
rm ./cmake_install.cmake
rm -r ./CMakeFiles
rm ./Makefile
rm ./CMakeCache.txt
rm -r ./src.o

#run
echo
echo "***** Example Start *****"
./example
echo "***** Example  End  *****"
