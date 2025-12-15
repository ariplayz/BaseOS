#include <ostream>

extern "C" {
#include <efi.h>
#include <efilib.h>
}

#include "UefiConsole.h"
#include "UefiCin.h"
#include <string>

int main();

extern "C"{
EFI_STATUS EFIAPI
efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table)
{
    InitializeLib(image_handle, system_table);
    int status = main();
    return EFI_SUCCESS;
}
}

std::string version = "0.1 ALPHA";

int main() {

    UefiConsole::writeLine << L"Welcome to BaseOS!" << UefiConsole::endl;
    UefiConsole::writeLine << L"Version " << version << UefiConsole::endl;

    UefiConsole::UefiCin cin;

    return 0;
}
