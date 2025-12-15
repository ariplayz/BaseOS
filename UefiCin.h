
#ifndef UEFI_CIN_H
#define UEFI_CIN_H

#include <efi.h>
#include <efilib.h>
#include <string>

namespace UefiConsole
{
    class UefiCin {
    public:
        // Read into std::wstring
        UefiCin& operator>>(std::wstring& input);

        // Read into CHAR16 buffer (for raw UEFI use)
        UefiCin& operator>>(CHAR16* buffer);
    };

    // Global instance like std::cin
    extern UefiCin cin;
}

#endif // UEFI_CIN_H
