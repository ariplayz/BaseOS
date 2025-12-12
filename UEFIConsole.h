
#ifndef UEFI_CONSOLE_H
#define UEFI_CONSOLE_H

extern "C" {
#include <efi.h>
#include <efilib.h>
}

class UEFIConsole {
public:
    explicit UEFIConsole(EFI_SYSTEM_TABLE* systemTable);

    void print(const CHAR16* message);
    void printf(const CHAR16* format, ...);
    void readLine(CHAR16* buffer, UINTN maxLen);

private:
    EFI_SYSTEM_TABLE* ST;
};

#endif
