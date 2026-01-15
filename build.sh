#!/bin/bash
set -e

BUILD_PATH=build
BIN_PATH=bin
RUN_PATH=build/qemu

# Clean previous build
rm -rf ${BUILD_PATH}
rm -fr ${BIN_PATH}

# Build
mkdir -p ${BUILD_PATH}
cd ${BUILD_PATH}
cmake ..
make
cd ..