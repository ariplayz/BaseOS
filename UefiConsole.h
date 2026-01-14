
#ifndef UEFI_CONSOLE_H
#define UEFI_CONSOLE_H

#include <string>
#include <efi.h>
#include <efilib.h>

namespace UefiConsole
{
    class UefiConsole {
    public:
        // Overload for wide string literal or CHAR16*
        UefiConsole& operator<<(const CHAR16* text);

        // Overload for std::string
        UefiConsole& operator<<(const std::string& text);

        // Overload for std::wstring
        UefiConsole& operator<<(const std::wstring& text);

        // Overload for integer
        UefiConsole& operator<<(int value);

        // Overload for double/float
        UefiConsole& operator<<(double value);

        // Overload for manipulators (like endl)
        UefiConsole& operator<<(UefiConsole& (*manip)(UefiConsole&));
    };

    // Global instance (like std::cout)
    extern UefiConsole writeLine;

    // Manipulator for newline
    UefiConsole& endl(UefiConsole& console);
}

#endif // UEFI_CONSOLE_H
