#!/bin/bash

BUILD_PATH=build
RUN_PATH=build/qemu
BINARY_PATH=${BUILD_PATH}/cppefi.efi
ISO_ROOT=${RUN_PATH}/iso

# Prepare ISO directory
mkdir -p ${ISO_ROOT}/EFI/BOOT

# Copy EFI binary and startup script
cp ${BINARY_PATH} ${ISO_ROOT}/EFI/BOOT/BOOTX64.EFI
echo "EFI\\BOOT\\BOOTX64.EFI" > ${ISO_ROOT}/startup.nsh

# Create ISO image
xorriso -as mkisofs -o ${RUN_PATH}/uefi.iso -iso-level 3 -eltorito-alt-boot -e EFI/BOOT/BOOTX64.EFI -no-emul-boot ${ISO_ROOT}

mkdir bin

cp ${RUN_PATH}/uefi.iso bin/BaseOS.iso
