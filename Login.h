
#ifndef LOGIN_H
#define LOGIN_H

#include "UEFIConsole.h"

class Login {
public:
    explicit Login(UEFIConsole& console);
    void start();

private:
    UEFIConsole& console;
    bool loggedIn;
    bool validate(const CHAR16* username, const CHAR16* password);
};

#endif
