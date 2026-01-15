#include "FileSystem.h"
#include "Console.h"

extern "C" {
#include <efi.h>
#include <efilib.h>
size_t strlen(const char* s);
extern EFI_HANDLE LibImageHandle;
}

EFI_STATUS FileSystem::GetRoot(EFI_FILE_HANDLE* root) {
    EFI_LOADED_IMAGE *loaded_image;
    EFI_GUID loaded_image_protocol = LOADED_IMAGE_PROTOCOL;
    EFI_STATUS status;

    status = uefi_call_wrapper((void*)ST->BootServices->HandleProtocol, 3, LibImageHandle, &loaded_image_protocol, (void**)&loaded_image);
    if (EFI_ERROR(status)) return status;

    EFI_FILE_IO_INTERFACE *file_system;
    EFI_GUID fs_protocol = SIMPLE_FILE_SYSTEM_PROTOCOL;
    status = uefi_call_wrapper((void*)ST->BootServices->HandleProtocol, 3, loaded_image->DeviceHandle, &fs_protocol, (void**)&file_system);
    if (EFI_ERROR(status)) return status;

    return uefi_call_wrapper((void*)file_system->OpenVolume, 2, file_system, root);
}

static void ToWString(const char* s, CHAR16* out, UINTN max) {
    if (!s || !out) return;
    UINTN i = 0;
    while (s[i] && i < max - 1) {
        out[i] = (CHAR16)s[i];
        i++;
    }
    out[i] = 0;
}

bool FileSystem::Exists(const char* path) {
    EFI_FILE_HANDLE root;
    if (EFI_ERROR(GetRoot(&root))) return false;

    CHAR16 wpath[256];
    ToWString(path, wpath, 256);

    EFI_FILE_HANDLE file;
    EFI_STATUS status = uefi_call_wrapper((void*)root->Open, 5, root, &file, wpath, EFI_FILE_MODE_READ, 0);
    
    if (status == EFI_SUCCESS) {
        uefi_call_wrapper((void*)file->Close, 1, file);
        uefi_call_wrapper((void*)root->Close, 1, root);
        return true;
    }
    
    uefi_call_wrapper((void*)root->Close, 1, root);
    return false;
}

EFI_STATUS FileSystem::WriteAllText(const char* path, const std::string& text) {
    EFI_FILE_HANDLE root;
    EFI_STATUS status = GetRoot(&root);
    if (EFI_ERROR(status)) return status;

    CHAR16 wpath[256];
    ToWString(path, wpath, 256);

    EFI_FILE_HANDLE file;
    status = uefi_call_wrapper((void*)root->Open, 5, root, &file, wpath, 
                                        EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    
    if (status == EFI_SUCCESS) {
        UINTN size = text.size();
        status = uefi_call_wrapper((void*)file->Write, 3, file, &size, (void*)text.c_str());
        uefi_call_wrapper((void*)file->Flush, 1, file);
        uefi_call_wrapper((void*)file->Close, 1, file);
    }
    
    uefi_call_wrapper((void*)root->Close, 1, root);
    return status;
}

EFI_STATUS FileSystem::WriteAllText(const char* path, const char* text) {
    if (!text) return EFI_INVALID_PARAMETER;
    return WriteAllText(path, std::string(text));
}

std::string FileSystem::ReadAllText(const char* path) {
    EFI_FILE_HANDLE root;
    if (EFI_ERROR(GetRoot(&root))) return "";

    CHAR16 wpath[256];
    ToWString(path, wpath, 256);

    EFI_FILE_HANDLE file;
    EFI_STATUS status = uefi_call_wrapper((void*)root->Open, 5, root, &file, wpath, EFI_FILE_MODE_READ, 0);
    if (status != EFI_SUCCESS) {
        uefi_call_wrapper((void*)root->Close, 1, root);
        return "";
    }

    std::string contents;
    char info_buf[512];
    UINTN info_size = sizeof(info_buf);
    EFI_FILE_INFO *info = (EFI_FILE_INFO *)info_buf;
    EFI_GUID info_id = EFI_FILE_INFO_ID;
    
    status = uefi_call_wrapper((void*)file->GetInfo, 4, file, &info_id, &info_size, info);
    
    if (status == EFI_SUCCESS) {
        UINTN size = (UINTN)info->FileSize;
        if (size > 0) {
            char* buffer = NULL;
            // Allocate slightly more just in case of off-by-one in FileSize reported
            if (uefi_call_wrapper((void*)ST->BootServices->AllocatePool, 3, EfiLoaderData, size + 8, (void**)&buffer) == EFI_SUCCESS) {
                uefi_call_wrapper((void*)file->SetPosition, 2, file, 0);
                UINTN read_size = size;
                status = uefi_call_wrapper((void*)file->Read, 3, file, &read_size, buffer);
                if (status == EFI_SUCCESS) {
                    contents = std::string(buffer, read_size);
                }
                uefi_call_wrapper((void*)ST->BootServices->FreePool, 1, buffer);
            }
        }
    }

    uefi_call_wrapper((void*)file->Close, 1, file);
    uefi_call_wrapper((void*)root->Close, 1, root);
    
    return contents;
}

EFI_STATUS FileSystem::AppendAllText(const char* path, const std::string& text) {
    EFI_FILE_HANDLE root;
    EFI_STATUS status = GetRoot(&root);
    if (EFI_ERROR(status)) return status;

    CHAR16 wpath[256];
    ToWString(path, wpath, 256);

    EFI_FILE_HANDLE file;
    status = uefi_call_wrapper((void*)root->Open, 5, root, &file, wpath, 
                                        EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    
    if (status == EFI_SUCCESS) {
        uefi_call_wrapper((void*)file->SetPosition, 2, file, 0xFFFFFFFFFFFFFFFFULL);
        UINTN size = text.size();
        status = uefi_call_wrapper((void*)file->Write, 3, file, &size, (void*)text.c_str());
        uefi_call_wrapper((void*)file->Flush, 1, file);
        uefi_call_wrapper((void*)file->Close, 1, file);
    }
    
    uefi_call_wrapper((void*)root->Close, 1, root);
    return status;
}

EFI_STATUS FileSystem::AppendAllText(const char* path, const char* text) {
    if (!text) return EFI_INVALID_PARAMETER;
    return AppendAllText(path, std::string(text));
}

EFI_STATUS FileSystem::Create(const char* path) {
    return WriteAllText(path, "");
}

EFI_STATUS FileSystem::Delete(const char* path) {
    EFI_FILE_HANDLE root;
    EFI_STATUS status = GetRoot(&root);
    if (EFI_ERROR(status)) return status;

    CHAR16 wpath[256];
    ToWString(path, wpath, 256);

    EFI_FILE_HANDLE file;
    status = uefi_call_wrapper((void*)root->Open, 5, root, &file, wpath, 
                                        EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
    
    if (status == EFI_SUCCESS) {
        status = uefi_call_wrapper((void*)file->Delete, 1, file);
    } else {
        uefi_call_wrapper((void*)root->Close, 1, root);
    }
    
    return status;
}
