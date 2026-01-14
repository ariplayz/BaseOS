#include <ostream>
#include "Console.h"

extern "C" {
#include <efi.h>
#include <efilib.h>
}

#include <string>

// Freestanding C/C++ stubs for EFI
extern "C" {
    size_t strlen(const char* s) {
        size_t len = 0;
        while (s[len]) len++;
        return len;
    }

    size_t wcslen(const wchar_t* s) {
        size_t len = 0;
        while (s[len]) len++;
        return len;
    }

    wchar_t* wmemcpy(wchar_t* dest, const wchar_t* src, size_t n) {
        wchar_t* d = dest;
        const wchar_t* s = src;
        while (n--) *d++ = *s++;
        return dest;
    }

    void* memmove(void* dest, const void* src, size_t n) {
        unsigned char* d = (unsigned char*)dest;
        const unsigned char* s = (const unsigned char*)src;
        if (d < s) {
            while (n--) *d++ = *s++;
        } else {
            d += n;
            s += n;
            while (n--) *--d = *--s;
        }
        return dest;
    }

    void _ZSt25__glibcxx_assert_failPKciS0_S0_(char const*, int, char const*, char const*) { while(1); }
}

namespace std {
    void __glibcxx_assert_fail(char const* file, int line, char const* function, char const* condition) noexcept {
        while(1);
    }
}

extern "C" {
    void* __dso_handle = (void*)0;

    // Name mangled versions of std::__throw_*
    void _ZSt19__throw_logic_errorPKc(char const*) { while(1); }
    void _ZSt20__throw_length_errorPKc(char const*) { while(1); }
    void _ZSt17__throw_bad_allocv() { while(1); }
    void _ZSt28__throw_bad_array_new_lengthv() { while(1); }
}

void* operator new(size_t size) {
    void* ptr = NULL;
    if (ST && ST->BootServices) {
        ST->BootServices->AllocatePool(EfiLoaderData, size, &ptr);
    }
    return ptr;
}

void operator delete(void* ptr, size_t) noexcept {
    if (ptr && ST && ST->BootServices) ST->BootServices->FreePool(ptr);
}

void operator delete(void* ptr) noexcept {
    if (ptr && ST && ST->BootServices) ST->BootServices->FreePool(ptr);
}

int main();

extern "C" {
EFI_STATUS EFIAPI
efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table)
{
    InitializeLib(image_handle, system_table);
    main();
    return EFI_SUCCESS;
}
}

std::string version = "0.1 ALPHA";

int main() {
    Console::WriteLine(L"Welcome to BaseOS!");
    Console::Write("Version: ");
    Console::WriteLine(version);

    Console::Write("Enter your name: ");
    std::string name = Console::ReadLine();

    Console::Write("Hello, ");
    Console::Write(name);
    Console::WriteLine("!");

    Console::WriteLine("Press any key to continue...");
    Console::Read();

    return 0;
}
