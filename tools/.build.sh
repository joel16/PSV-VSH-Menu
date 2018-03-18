#!/bin/sh
set -e
set -x

cmake -G "Unix Makefiles"
make clean && make
rm CMakeCache.txt cmake_install.cmake LICENSE README.md vsh.elf vsh.velf CMakeLists.txt Makefile
rm -rf exports CMakeFiles include source tools
