
#include "UEFIConsole.h"
#include <stdarg.h>

UEFIConsole::UEFIConsole(EFI_SYSTEM_TABLE* systemTable)
    : ST(systemTable) {}

void UEFIConsole::print(const CHAR16* message) {
    Print(message);
}

void UEFIConsole::printf(const CHAR16* format, ...) {
    va_list args;
    va_start(args, format);
    VPrint(format, args);
    va_end(args);
}

void UEFIConsole::readLine(CHAR16* buffer, UINTN maxLen) {
    EFI_INPUT_KEY key;
    UINTN index = 0;

    while (index < maxLen - 1) {
        uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &key);

        if (key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
            break;
        }

        if (key.UnicodeChar != 0) {
            buffer[index++] = key.UnicodeChar;
            CHAR16 echo[2] = { key.UnicodeChar, L'\0' };
            Print(echo);
        }
    }

    buffer[index] = L'\0';
    Print(L"\n");
}
