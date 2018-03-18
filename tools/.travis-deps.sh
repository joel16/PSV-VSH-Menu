#!/bin/sh
set -e
set -x

# Build and install VITASDK using VDPM
git clone https://github.com/vitasdk/vdpm
cd vdpm
./bootstrap-vitasdk.sh
./install-all.sh
cd ../
rm -rf vdpm
