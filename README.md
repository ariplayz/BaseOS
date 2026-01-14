# BaseOS

BaseOS is a simple, freestanding C++ operating system designed to run in a pure UEFI environment.

## Features
- **UEFI Native:** No legacy BIOS support, purely EFI.
- **C++ Console Library:** High-level `Console::WriteLine`, `Console::ReadLine`, etc.
- **Freestanding:** Custom memory management and ABI stubs for a no-OS environment.

## Website
Check out our website: [https://ariplayz.github.io/BaseOS/](https://ariplayz.github.io/BaseOS/)

## Build Instructions
1. **Build the EFI binary:**
   ```bash
   ./build.sh
   ```
2. **Create the bootable ISO:**
   ```bash
   ./makeimage.sh
   ```
3. **Run in QEMU:**
   ```bash
   ./run.sh
   ```

## Requirements
- `cmake`
- `clang`
- `gcc` / `g++`
- `gnu-efi`
- `xorriso`
- `mtools`
- `ovmf` (for testing with QEMU)
- `mtools`
- `dosfstools`
- `qemu-system-x86_64` (for testing with QEMU)
