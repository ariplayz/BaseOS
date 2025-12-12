#!/bin/bash
set -e

BUILD_PATH=build
RUN_PATH=build/qemu

OVMF_DISK_IMG=/usr/share/ovmf/OVMF.fd
BINARY_PATH=${BUILD_PATH}/cppefi.efi
DISK_PATH=${RUN_PATH}/uefi.img

# Clean previous build
rm -rf ${BUILD_PATH}

# Build
mkdir -p ${BUILD_PATH}
cd ${BUILD_PATH}
cmake ..
make
cd ..