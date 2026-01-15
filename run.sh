#!/bin/bash
set -e

# Paths
ISO_FILE=bin/BaseOS.iso
OVMF_DISK_IMG=/usr/share/OVMF/OVMF_CODE.fd

# Check ISO exists
if [ ! -f "${ISO_FILE}" ]; then
    echo "ERROR: ISO not found at ${ISO_FILE}. Run build.sh and makeimage.sh first."
    exit 1
fi

# Run QEMU
echo "[+] Starting QEMU with ${ISO_FILE}..."
# We use -drive instead of -cdrom to allow the hybrid ISO to be treated as a writable disk
qemu-system-x86_64 \
  -cpu qemu64 \
  -net none \
  -drive if=pflash,format=raw,unit=0,file=${OVMF_DISK_IMG},readonly=on \
  -drive file=${ISO_FILE},format=raw \
  -m 512M \
  -serial stdio
