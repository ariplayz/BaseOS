#include "FileSystem.h"
#include "Console.h"

extern "C" {
#include <efi.h>
#include <efilib.h>
size_t strlen(const char* s);
}

extern EFI_HANDLE gImageHandle;

EFI_STATUS FileSystem::GetRoot(EFI_FILE_HANDLE* root) {
    EFI_LOADED_IMAGE *loaded_image;
    EFI_GUID loaded_image_protocol = LOADED_IMAGE_PROTOCOL;
    EFI_STATUS status;

    status = uefi_call_wrapper((void*)ST->BootServices->HandleProtocol, 3, gImageHandle, &loaded_image_protocol, (void**)&loaded_image);
    if (EFI_ERROR(status)) return status;

    EFI_FILE_IO_INTERFACE *file_system;
    EFI_GUID fs_protocol = SIMPLE_FILE_SYSTEM_PROTOCOL;
    status = uefi_call_wrapper((void*)ST->BootServices->HandleProtocol, 3, loaded_image->DeviceHandle, &fs_protocol, (void**)&file_system);
    if (EFI_ERROR(status)) return status;

    return uefi_call_wrapper((void*)file_system->OpenVolume, 2, file_system, root);
}

std::wstring FileSystem::ToWString(const std::string& s) {
    std::wstring ws;
    for (char c : s) {
        ws.push_back((wchar_t)c);
    }
    return ws;
}

bool FileSystem::Exists(const std::string& path) {
    return Exists(ToWString(path));
}

bool FileSystem::Exists(const std::wstring& path) {
    EFI_FILE_HANDLE root;
    if (EFI_ERROR(GetRoot(&root))) return false;

    EFI_FILE_HANDLE file;
    EFI_STATUS status = uefi_call_wrapper((void*)root->Open, 5, root, &file, (CHAR16*)path.c_str(), EFI_FILE_MODE_READ, 0);
    
    if (status == EFI_SUCCESS) {
        uefi_call_wrapper((void*)file->Close, 1, file);
        uefi_call_wrapper((void*)root->Close, 1, root);
        return true;
    }
    
    uefi_call_wrapper((void*)root->Close, 1, root);
    return false;
}

void FileSystem::WriteAllText(const std::string& path, const std::string& text) {
    WriteAllText(ToWString(path), text.c_str(), text.size());
}

void FileSystem::WriteAllText(const std::string& path, const char* text) {
    if (!text) return;
    WriteAllText(ToWString(path), text, strlen(text));
}

void FileSystem::WriteAllText(const std::wstring& path, const std::string& text) {
    WriteAllText(path, text.c_str(), text.size());
}

void FileSystem::WriteAllText(const std::wstring& path, const void* buffer, UINTN size) {
    EFI_FILE_HANDLE root;
    if (EFI_ERROR(GetRoot(&root))) return;

    EFI_FILE_HANDLE file;
    EFI_STATUS status = uefi_call_wrapper((void*)root->Open, 5, root, &file, (CHAR16*)path.c_str(), 
                                        EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    
    if (status == EFI_SUCCESS) {
        uefi_call_wrapper((void*)file->Write, 3, file, &size, (void*)buffer);
        uefi_call_wrapper((void*)file->Flush, 1, file);
        uefi_call_wrapper((void*)file->Close, 1, file);
    }
    
    uefi_call_wrapper((void*)root->Close, 1, root);
}

std::string FileSystem::ReadAllText(const std::string& path) {
    return ReadAllText(ToWString(path));
}

std::string FileSystem::ReadAllText(const std::wstring& path) {
    EFI_FILE_HANDLE root;
    if (EFI_ERROR(GetRoot(&root))) return "";

    EFI_FILE_HANDLE file;
    EFI_STATUS status = uefi_call_wrapper((void*)root->Open, 5, root, &file, (CHAR16*)path.c_str(), EFI_FILE_MODE_READ, 0);
    if (status != EFI_SUCCESS) {
        uefi_call_wrapper((void*)root->Close, 1, root);
        return "";
    }

    std::string contents;
    EFI_FILE_INFO *info = LibFileInfo(file);
    if (info) {
        UINTN size = (UINTN)info->FileSize;
        if (size > 0) {
            char* buffer = NULL;
            if (ST->BootServices->AllocatePool(EfiLoaderData, size + 1, (void**)&buffer) == EFI_SUCCESS) {
                // Ensure position is at start
                uefi_call_wrapper((void*)file->SetPosition, 2, file, 0);
                
                UINTN read_size = size;
                if (uefi_call_wrapper((void*)file->Read, 3, file, &read_size, buffer) == EFI_SUCCESS) {
                    buffer[read_size] = '\0';
                    contents = buffer;
                }
                ST->BootServices->FreePool(buffer);
            }
        }
        ST->BootServices->FreePool(info);
    }

    uefi_call_wrapper((void*)file->Close, 1, file);
    uefi_call_wrapper((void*)root->Close, 1, root);
    
    return contents;
}

void FileSystem::AppendAllText(const std::string& path, const std::string& text) {
    AppendAllText(ToWString(path), text.c_str(), text.size());
}

void FileSystem::AppendAllText(const std::string& path, const char* text) {
    if (!text) return;
    AppendAllText(ToWString(path), text, strlen(text));
}

void FileSystem::AppendAllText(const std::wstring& path, const std::string& text) {
    AppendAllText(path, text.c_str(), text.size());
}

void FileSystem::AppendAllText(const std::wstring& path, const void* buffer, UINTN size) {
    EFI_FILE_HANDLE root;
    if (EFI_ERROR(GetRoot(&root))) return;

    EFI_FILE_HANDLE file;
    EFI_STATUS status = uefi_call_wrapper((void*)root->Open, 5, root, &file, (CHAR16*)path.c_str(), 
                                        EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    
    if (status == EFI_SUCCESS) {
        uefi_call_wrapper((void*)file->SetPosition, 2, file, 0xFFFFFFFFFFFFFFFFULL);
        uefi_call_wrapper((void*)file->Write, 3, file, &size, (void*)buffer);
        uefi_call_wrapper((void*)file->Flush, 1, file);
        uefi_call_wrapper((void*)file->Close, 1, file);
    }
    
    uefi_call_wrapper((void*)root->Close, 1, root);
}

void FileSystem::Create(const std::string& path) {
    Create(ToWString(path));
}

void FileSystem::Create(const std::wstring& path) {
    WriteAllText(path, "", 0);
}

void FileSystem::Delete(const std::string& path) {
    Delete(ToWString(path));
}

void FileSystem::Delete(const std::wstring& path) {
    EFI_FILE_HANDLE root;
    if (EFI_ERROR(GetRoot(&root))) return;

    EFI_FILE_HANDLE file;
    EFI_STATUS status = uefi_call_wrapper((void*)root->Open, 5, root, &file, (CHAR16*)path.c_str(), 
                                        EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
    
    if (status == EFI_SUCCESS) {
        uefi_call_wrapper((void*)file->Delete, 1, file);
    }
    
    uefi_call_wrapper((void*)root->Close, 1, root);
}
