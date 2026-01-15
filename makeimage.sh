#!/bin/bash
set -e

BUILD_PATH=build
RUN_PATH=build/qemu
BINARY_PATH=${BUILD_PATH}/cppefi.efi
ISO_ROOT=${RUN_PATH}/iso
EFI_IMG=${RUN_PATH}/efiboot.img
ISO_FILE=bin/BaseOS.iso

mkdir -p bin ${RUN_PATH} ${ISO_ROOT}/EFI/BOOT

if [ ! -f "${BINARY_PATH}" ]; then
    echo "ERROR: EFI binary not found at ${BINARY_PATH}. Run build.sh first."
    exit 1
fi

echo "[+] Preparing ISO root..."
# Copy binary to the standard EFI boot path
cp ${BINARY_PATH} ${ISO_ROOT}/EFI/BOOT/BOOTX64.EFI

# Create a startup.nsh for automatic booting in UEFI shell
echo "EFI\\BOOT\\BOOTX64.EFI" > ${ISO_ROOT}/startup.nsh

echo "[+] Creating EFI boot image (for El Torito)..."
# Create a small FAT image to be used as the EFI boot catalog
# This is required for many UEFI firmwares to see the ISO as bootable
dd if=/dev/zero of=${EFI_IMG} bs=1M count=31
mkfs.vfat ${EFI_IMG}
mmd -i ${EFI_IMG} ::/EFI ::/EFI/BOOT
mcopy -i ${EFI_IMG} ${BINARY_PATH} ::/EFI/BOOT/BOOTX64.EFI

echo "[+] Creating universal bootable ISO..."
# -V: Volume label
# -J -R: Joliet and Rock Ridge for cross-platform compatibility
# -eltorito-platform 0xef: Mark as EFI bootable
# -e: Use the FAT image as the boot image
# -isohybrid-gpt-basdat: Make it a hybrid ISO (bootable from USB/HDD with GPT)
xorriso -as mkisofs \
  -o ${ISO_FILE} \
  -V "BASEOS" \
  -J -R \
  -iso-level 3 \
  -full-iso9660-filenames \
  -eltorito-platform 0xef \
  -e efiboot.img \
  -no-emul-boot \
  -isohybrid-gpt-basdat \
  -partition_offset 16 \
  -graft-points \
  efiboot.img=${EFI_IMG} \
  ${ISO_ROOT}

echo "[+] ISO created: ${ISO_FILE}"
echo "[+] You can flash this ISO to a USB stick using Rufus, Etcher, or dd."
