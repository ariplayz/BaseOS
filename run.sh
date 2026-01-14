#!/bin/bash
set -e

# Paths
RUN_PATH=build/qemu
OVMF_DISK_IMG=/usr/share/OVMF/OVMF_CODE.fd
ISO_FILE=${RUN_PATH}/uefi.iso

# Check ISO exists
if [ ! -f "${ISO_FILE}" ]; then
    echo "ERROR: ISO not found at ${ISO_FILE}. Run makeimage.sh first."
    exit 1
fi

# Run QEMU
echo "[+] Starting QEMU..."
qemu-system-x86_64 \
  -cpu qemu64 \
  -net none \
  -drive if=pflash,format=raw,unit=0,file=${OVMF_DISK_IMG},readonly=on \
  -cdrom ${ISO_FILE} \
  -m 512M \
  -serial stdio \
  -boot d
