#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

class Console {
public:
    static void WriteLine(const std::string& text);
    static void WriteLine(const std::wstring& text);
    static void WriteLine(const char* text);
    static void WriteLine(const wchar_t* text);
    static void WriteLine();

    static void Write(const std::string& text);
    static void Write(const std::wstring& text);
    static void Write(const char* text);
    static void Write(const wchar_t* text);

    static std::string ReadLine();
    static int Read();
};

#endif // CONSOLE_H
