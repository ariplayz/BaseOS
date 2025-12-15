//UefiConsole.cpp
#include "UefiConsole.h"
#include <string>
#include <efi.h>
#include <efilib.h>

namespace UefiConsole
{
    class UefiConsole {
    public:
        // Wide string literal or CHAR16*
        UefiConsole& operator<<(const CHAR16* text) {
            Print(text);
            return *this;
        }

        // std::wstring
        UefiConsole& operator<<(const std::wstring& text) {
            Print(text.c_str());
            return *this;
        }

        // Integer
        UefiConsole& operator<<(int value) {
            CHAR16 buffer[32];
            SPrint(buffer, sizeof(buffer)/sizeof(CHAR16), L"%d", value);
            Print(buffer);
            return *this;
        }

        // Double/float
        UefiConsole& operator<<(double value) {
            CHAR16 buffer[64];
            SPrint(buffer, sizeof(buffer)/sizeof(CHAR16), L"%g", value);
            Print(buffer);
            return *this;
        }

        // Manipulators like endl
        UefiConsole& operator<<(UefiConsole& (*manip)(UefiConsole&)) {
            return manip(*this);
        }
    };

    // Global instance like std::cout
    UefiConsole writeLine;

    // Newline manipulator
    UefiConsole& endl(UefiConsole& console) {
        Print(L"\n");
        return console;
    }
} // UefiConsole