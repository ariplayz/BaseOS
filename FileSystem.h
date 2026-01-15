#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>

extern "C" {
#include <efi.h>
}

class FileSystem {
public:
    static bool Exists(const char* path);

    static EFI_STATUS WriteAllText(const char* path, const std::string& text);
    static EFI_STATUS WriteAllText(const char* path, const char* text);

    static std::string ReadAllText(const char* path);

    static EFI_STATUS AppendAllText(const char* path, const std::string& text);
    static EFI_STATUS AppendAllText(const char* path, const char* text);

    static EFI_STATUS Create(const char* path);
    static EFI_STATUS Delete(const char* path);

private:
    static EFI_STATUS WriteAllText(const CHAR16* path, const void* buffer, UINTN size);
    static EFI_STATUS AppendAllText(const CHAR16* path, const void* buffer, UINTN size);
    static EFI_STATUS GetRoot(EFI_FILE_HANDLE* root);
};

#endif // FILESYSTEM_H
