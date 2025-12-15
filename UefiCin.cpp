
#include "UefiCin.h"

namespace UefiConsole
{
    UefiCin& UefiCin::operator>>(std::wstring& input) {
        input.clear();
        EFI_INPUT_KEY key;
        while (true) {
            uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &key);
            if (key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
                break;
            }
            if (key.UnicodeChar != 0) {
                input.push_back(key.UnicodeChar);
                Print(&key.UnicodeChar); // Echo
            }
        }
        Print(L"\n");
        return *this;
    }

    UefiCin& UefiCin::operator>>(CHAR16* buffer) {
        UINTN pos = 0;
        EFI_INPUT_KEY key;
        while (true) {
            uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &key);
            if (key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
                break;
            }
            if (key.UnicodeChar != 0) {
                buffer[pos++] = key.UnicodeChar;
                Print(&key.UnicodeChar); // Echo
            }
        }
        buffer[pos] = L'\0';
        Print(L"\n");
        return *this;
    }

    // Global instance
    UefiCin cin;
}
