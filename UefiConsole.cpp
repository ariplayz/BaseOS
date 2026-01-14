//UefiConsole.cpp
#include "UefiConsole.h"
#include <string>
#include <efi.h>
#include <efilib.h>

namespace UefiConsole
{
    // Wide string literal or CHAR16*
    UefiConsole& UefiConsole::operator<<(const CHAR16* text) {
        if (text) {
            Print((CHAR16*)L"%s", text);
        } else {
            Print((CHAR16*)L"(null)");
        }
        return *this;
    }

    // std::string
    UefiConsole& UefiConsole::operator<<(const std::string& text) {
        // UEFI uses CHAR16 for its console output. 
        // For simplicity, we convert char to CHAR16 by casting/widening.
        // Proper conversion would be needed for non-ASCII characters.
        for (char c : text) {
            CHAR16 buffer[2] = {(CHAR16)c, 0};
            Print((CHAR16*)L"%c", buffer[0]);
        }
        return *this;
    }

    // std::wstring
    UefiConsole& UefiConsole::operator<<(const std::wstring& text) {
        Print((CHAR16*)L"%s", (CHAR16*)text.c_str());
        return *this;
    }

    // Integer
    UefiConsole& UefiConsole::operator<<(int value) {
        CHAR16 buffer[32];
        SPrint(buffer, sizeof(buffer), (CHAR16*)L"%d", value);
        Print(buffer);
        return *this;
    }

    // Double/float
    UefiConsole& UefiConsole::operator<<(double value) {
        CHAR16 buffer[64];
        SPrint(buffer, sizeof(buffer), (CHAR16*)L"%g", value);
        Print(buffer);
        return *this;
    }

    // Manipulators like endl
    UefiConsole& UefiConsole::operator<<(UefiConsole& (*manip)(UefiConsole&)) {
        return manip(*this);
    }

    // Global instance like std::cout
    UefiConsole writeLine;

    // Newline manipulator
    UefiConsole& endl(UefiConsole& console) {
        Print((CHAR16*)L"\r\n");
        return console;
    }
} // UefiConsole