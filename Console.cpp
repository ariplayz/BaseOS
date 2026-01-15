#include "Console.h"

extern "C" {
#include <efi.h>
#include <efilib.h>
}

void Console::Write(const std::wstring& text) {
    Write(text.c_str());
}

void Console::Write(const wchar_t* text) {
    if (text) {
        Print((CHAR16*)L"%s", (CHAR16*)text);
    }
}

void Console::Write(const std::string& text) {
    Write(text.c_str());
}

void Console::Write(const char* text) {
    if (text) {
        Print((CHAR16*)L"%a", (char*)text);
    }
}

void Console::WriteLine(const std::wstring& text) {
    Write(text);
    WriteLine();
}

void Console::WriteLine(const wchar_t* text) {
    Write(text);
    WriteLine();
}

void Console::WriteLine(const std::string& text) {
    Write(text);
    WriteLine();
}

void Console::WriteLine(const char* text) {
    Write(text);
    WriteLine();
}

void Console::WriteLine() {
    Print((CHAR16*)L"\r\n");
}

std::string Console::ReadLine() {
    std::string input;
    EFI_INPUT_KEY key;
    UINTN event_index;

    while (true) {
        // Wait for key event
        uefi_call_wrapper((void*)ST->BootServices->WaitForEvent, 3, 1, &ST->ConIn->WaitForKey, &event_index);
        // Read the key
        EFI_STATUS status = uefi_call_wrapper((void*)ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &key);
        
        if (status != EFI_SUCCESS) continue;

        if (key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
            WriteLine();
            break;
        } else if (key.UnicodeChar == CHAR_BACKSPACE) {
            if (!input.empty()) {
                input.pop_back();
                Print((CHAR16*)L"\b \b");
            }
        } else if (key.UnicodeChar != 0) {
            input.push_back((char)key.UnicodeChar);
            CHAR16 buffer[2] = {key.UnicodeChar, 0};
            Print((CHAR16*)L"%c", buffer[0]);
        }
    }
    return input;
}

int Console::Read() {
    EFI_INPUT_KEY key;
    UINTN event_index;
    
    // Wait for key event
    uefi_call_wrapper((void*)ST->BootServices->WaitForEvent, 3, 1, &ST->ConIn->WaitForKey, &event_index);
    // Read the key
    EFI_STATUS status = uefi_call_wrapper((void*)ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &key);
    
    if (status == EFI_SUCCESS) {
        return (int)key.UnicodeChar;
    }
    return -1;
}
