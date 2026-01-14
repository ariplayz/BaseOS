# BaseOS

BaseOS is a simple, freestanding C++ operating system designed to run in a pure UEFI environment.

## Features
- **UEFI Native:** No legacy BIOS support, purely EFI.
- **C++ Console Library:** High-level `Console::WriteLine`, `Console::ReadLine`, etc.
- **Freestanding:** Custom memory management and ABI stubs for a no-OS environment.

## Website
Check out our website: [https://aric.github.io/BaseOS/](https://aric.github.io/BaseOS/) (assuming your username is aric)

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
- `gcc` / `g++`
- `gnu-efi`
- `xorriso`
- `mtools`
- `ovmf` (for testing with QEMU)
