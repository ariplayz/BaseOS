cmake_minimum_required(VERSION 3.16.3)
project(cppefi C CXX)

# Compiler settings
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Architecture and paths
set(ARCH x86_64)
set(EFI_INCLUDE_PATH /usr/local/include/efi)
set(EFI_LIB_PATH /usr/local/lib)
set(EFI_CRT_OBJS ${EFI_LIB_PATH}/crt0-efi-${ARCH}.o)
set(EFI_LDS ${EFI_LIB_PATH}/elf_${ARCH}_efi.lds)

# Compile flags
set(COMPILE_FLAGS
    -I${EFI_INCLUDE_PATH}
    -I${EFI_INCLUDE_PATH}/${ARCH}
    -I${EFI_INCLUDE_PATH}/protocol
    -fno-stack-protector
    -fpic
    -fshort-wchar
    -mno-red-zone
    -Wall
    -DEFI_FUNCTION_WRAPPER
    -fno-exceptions
    -fno-rtti
)

# Create object file from main.cpp
add_library(main_obj OBJECT main.cpp)
target_compile_options(main_obj PRIVATE ${COMPILE_FLAGS})

# Custom command to link with ld and convert to EFI
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/cppefi.so
    COMMAND ld
        -nostdlib
        --warn-common
        --no-undefined
        --fatal-warnings
        --build-id=sha1
        -z nocombreloc
        -shared
        -Bsymbolic
        -L${EFI_LIB_PATH}
        ${EFI_CRT_OBJS}
        $<TARGET_OBJECTS:main_obj>
        -o ${CMAKE_BINARY_DIR}/cppefi.so
        -lefi
        -lgnuefi
        /usr/lib/gcc/x86_64-linux-gnu/13/libgcc.a
        -T ${EFI_LDS}
    DEPENDS main_obj
    COMMENT "Linking cppefi.so"
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/cppefi.efi
    COMMAND objcopy
        -j .text
        -j .sdata
        -j .data
        -j .dynamic
        -j .rodata
        -j .rel
        -j .rela
        -j .rel.*
        -j .rela.*
        -j .rel*
        -j .rela*
        -j .areloc
        -j .reloc
        --target efi-app-${ARCH}
        ${CMAKE_BINARY_DIR}/cppefi.so
        ${CMAKE_BINARY_DIR}/cppefi.efi
    DEPENDS ${CMAKE_BINARY_DIR}/cppefi.so
    COMMENT "Converting cppefi.so to cppefi.efi"
)

add_custom_target(cppefi ALL DEPENDS ${CMAKE_BINARY_DIR}/cppefi.efi)
