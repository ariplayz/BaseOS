
#include <efi.h>
#include <efilib.h>
#include "UEFIConsole.h"
#include "Login.h"

static const CHAR16* hostname = L"BaseOS";
static const CHAR16* version = L"0.1-alpha";

void runApp(EFI_SYSTEM_TABLE* system_table) {
    UEFIConsole console(system_table);

    console.print(L"Welcome to BaseOS!\n");
    console.printf(L"Version: %s\n", version);

    Login login(console);
    login.start();
}

extern "C"
EFI_STATUS EFIAPI efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table) {
    InitializeLib(image_handle, system_table);
    runApp(system_table);
    return EFI_SUCCESS;
}
