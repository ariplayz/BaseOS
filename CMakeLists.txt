
cmake_minimum_required(VERSION 3.16)
project(cppefi C CXX)

# Force Linux toolchain under WSL
set(CMAKE_SYSTEM_NAME Linux)

# Compiler settings
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Architecture and paths
set(ARCH x86_64)
set(EFI_INCLUDE_PATH /usr/local/include/efi)
set(EFI_LIB_PATH /usr/local/lib)
set(EFI_CRT_OBJS ${EFI_LIB_PATH}/crt0-efi-${ARCH}.o)
set(EFI_LDS ${EFI_LIB_PATH}/elf_${ARCH}_efi.lds)

# Compile flags for UEFI
set(COMPILE_FLAGS
        -I${EFI_INCLUDE_PATH}
        -I${EFI_INCLUDE_PATH}/${ARCH}
        -I${EFI_INCLUDE_PATH}/protocol
        -fno-stack-protector
        -fPIC
        -fshort-wchar
        -mno-red-zone
        -Wall
        -DEFI_FUNCTION_WRAPPER
        -fno-exceptions
        -fno-rtti
        -static-libstdc++
)

# Object library for sources
add_library(main_obj OBJECT
        main.cpp
        UEFIConsole.cpp
        Login.cpp
)

target_compile_options(main_obj PRIVATE ${COMPILE_FLAGS})

# Link to EFI binary
add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/cppefi.efi
        COMMAND ld -nostdlib -shared -Bsymbolic
        -L${EFI_LIB_PATH}
        ${EFI_CRT_OBJS}
        $<TARGET_OBJECTS:main_obj>
        -o ${CMAKE_BINARY_DIR}/cppefi.efi
        -lefi -lgnuefi /usr/lib/gcc/x86_64-linux-gnu/13/libgcc.a
        -T ${EFI_LDS}
        DEPENDS main_obj
        COMMENT "Linking cppefi.efi"
)

add_custom_target(cppefi ALL DEPENDS ${CMAKE_BINARY_DIR}/cppefi.efi)
