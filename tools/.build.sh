#!/bin/sh
set -e
set -x

cd kernel && cmake -G "Unix Makefiles" && make clean && make 
cd..
cd user && cmake -G "Unix Makefiles" && make clean && make 
cd ..
mv kernel/vsh.skprx . && mv user/vsh.suprx .
rm .travis.yml LICENSE README.md
rm -rf kernel user tools