#!/bin/bash
set -e

BUILD_PATH=build
RUN_PATH=build/qemu

# Clean previous build
rm -rf ${BUILD_PATH}

# Build
mkdir -p ${BUILD_PATH}
cd ${BUILD_PATH}
cmake ..
make
cd ..