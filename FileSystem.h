#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>

extern "C" {
#include <efi.h>
}

class FileSystem {
public:
    static bool Exists(const std::wstring& path);
    static bool Exists(const std::string& path);

    static void WriteAllText(const std::wstring& path, const std::string& text);
    static void WriteAllText(const std::string& path, const std::string& text);
    static void WriteAllText(const std::string& path, const char* text);

    static std::string ReadAllText(const std::wstring& path);
    static std::string ReadAllText(const std::string& path);

    static void AppendAllText(const std::wstring& path, const std::string& text);
    static void AppendAllText(const std::string& path, const std::string& text);
    static void AppendAllText(const std::string& path, const char* text);

    static void Create(const std::wstring& path);
    static void Create(const std::string& path);

    static void Delete(const std::wstring& path);
    static void Delete(const std::string& path);

private:
    static void WriteAllText(const std::wstring& path, const void* buffer, UINTN size);
    static void AppendAllText(const std::wstring& path, const void* buffer, UINTN size);

    static EFI_STATUS GetRoot(EFI_FILE_HANDLE* root);
    static std::wstring ToWString(const std::string& s);
};

#endif // FILESYSTEM_H
