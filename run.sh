#!/bin/bash

BUILD_PATH=build
RUN_PATH=build/qemu

OVMF_DISK_IMG=/usr/share/ovmf/OVMF.fd
BINARY_PATH=${BUILD_PATH}/cppefi.efi
DISK_PATH=${RUN_PATH}/uefi.img

qemu-system-x86_64 \
  -cpu qemu64 \
  -net none \
  -drive if=pflash,format=raw,unit=0,file=${OVMF_DISK_IMG},readonly=on \
  -drive if=ide,format=raw,file=${DISK_PATH} \
  -m 512M \
  -serial stdio