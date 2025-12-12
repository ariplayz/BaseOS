
#!/bin/bash
set -e

BUILD_PATH=build
RUN_PATH=build/qemu
BINARY_PATH=${BUILD_PATH}/cppefi.efi
ISO_ROOT=${RUN_PATH}/iso
EFI_IMG=${RUN_PATH}/efiboot.img
ISO_FILE=${RUN_PATH}/uefi.iso

mkdir -p ${BUILD_PATH} ${RUN_PATH} ${ISO_ROOT}

if [ ! -f "${BINARY_PATH}" ]; then
    echo "ERROR: EFI binary not found at ${BINARY_PATH}"
    exit 1
fi

echo "[+] Creating EFI boot image..."
dd if=/dev/zero of=${EFI_IMG} bs=1M count=10
mkfs.vfat ${EFI_IMG}
mmd -i ${EFI_IMG} ::/EFI ::/EFI/BOOT
mcopy -i ${EFI_IMG} ${BINARY_PATH} ::/EFI/BOOT/BOOTX64.EFI

echo "EFI\\BOOT\\BOOTX64.EFI" > ${ISO_ROOT}/startup.nsh

# Move EFI image into ISO root
mv ${EFI_IMG} ${ISO_ROOT}/efiboot.img

echo "[+] Creating bootable ISO..."
xorriso -as mkisofs \
  -o ${ISO_FILE} \
  -iso-level 3 \
  -full-iso9660-filenames \
  -eltorito-alt-boot \
  -e efiboot.img \
  -no-emul-boot \
  ${ISO_ROOT}

mkdir -p bin
cp ${ISO_FILE} bin/BaseOS.iso

echo "[+] ISO created: bin/BaseOS.iso"
