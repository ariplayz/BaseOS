
#include "Login.h"
#include <string.h> // For StrCmp

Login::Login(UEFIConsole& console)
    : console(console), loggedIn(false) {}

struct Credential {
    const CHAR16* username;
    const CHAR16* password;
};

static Credential credentials[] = {
    { L"admin", L"1234" },
    { L"aric", L"1MonKee!!" },
    { L"guest", L"guest" }
};

bool Login::validate(const CHAR16* username, const CHAR16* password) {
    for (auto& cred : credentials) {
        if (StrCmp((CHAR16*)username, (CHAR16*)cred.username) == 0 &&
            StrCmp((CHAR16*)password, (CHAR16*)cred.password) == 0) {
            return true;
            }
    }
    return false;
}

void Login::start() {
    CHAR16 username[64];
    CHAR16 password[64];

    while (!loggedIn) {
        console.print(L"\n=== Login ===\n");

        console.print(L"Username: ");
        console.readLine(username, 64);

        console.print(L"Password: ");
        console.readLine(password, 64);

        if (validate(username, password)) {
            console.print(L"\nLogin successful!\n");
            loggedIn = true;
        } else {
            console.print(L"\nInvalid credentials. Try again.\n");
        }
    }
}
