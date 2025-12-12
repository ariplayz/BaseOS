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

# Prepare image directories
mkdir -p ${RUN_PATH}

# Create EFI partition image (FAT16)
dd if=/dev/zero of=/tmp/part.img bs=512 count=91669
mformat -i /tmp/part.img -h 32 -t 32 -n 64 -c 1

# Copy EFI binary into EFI/BOOT and create startup script
mmd -i /tmp/part.img ::/EFI
mmd -i /tmp/part.img ::/EFI/BOOT
mcopy -i /tmp/part.img ${BINARY_PATH} ::/EFI/BOOT/BOOTX64.EFI

# Optional: startup.nsh to auto-run (not needed if BOOTX64.EFI is present)
echo "EFI\\BOOT\\BOOTX64.EFI" > startup.nsh
mcopy -i /tmp/part.img startup.nsh ::/

# Create full disk image with GPT and EFI partition
dd if=/dev/zero of=${DISK_PATH} bs=512 count=93750
parted ${DISK_PATH} -s -a minimal mklabel gpt
parted ${DISK_PATH} -s -a minimal mkpart EFI FAT16 2048s 93716s
parted ${DISK_PATH} -s -a minimal toggle 1 boot

# Copy EFI partition into disk image
dd if=/tmp/part.img of=${DISK_PATH} bs=512 count=91669 seek=2048 conv=notrunc

# Run QEMU with OVMF firmware
qemu-system-x86_64 \
  -cpu qemu64 \
  -net none \
  -drive if=pflash,format=raw,unit=0,file=${OVMF_DISK_IMG},readonly=on \
  -drive if=ide,format=raw,file=${DISK_PATH} \
  -m 512M \
  -serial stdio