# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(../FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/pico_sdk_import.cmake)
include(../FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(application C CXX ASM)

set(PARAM_ASSERTIONS_ENABLE_ALL 1)

pico_sdk_init()

add_subdirectory(../freertos build)

add_executable(application
        application.c
        hw_config.c
        stdio_cli.c
        filesystem_cmds.c
)

pico_set_program_name(application "application")
pico_set_program_version(application "0.1")

pico_enable_stdio_uart(application 1)
pico_enable_stdio_usb(application 0)

target_link_libraries(application 
        FreeRTOS+FAT+CLI
)

target_include_directories(application PUBLIC 
        #${FREERTOS_KERNEL_PATH}/include 
        #${FREERTOS_KERNEL_PATH}/${FREERTOS_KERNEL_RP2040_RELATIVE_PATH}/include
)

target_compile_options(application PUBLIC
    -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-type-limits
    -Wno-sign-compare -Wno-unused-variable -Wno-format-truncation -Wno-format-overflow
    -Wno-maybe-uninitialized -Wno-unused-but-set-variable
)

target_compile_definitions(application PUBLIC DEBUG)

target_compile_definitions(application PUBLIC DEBUG N_SD_CARDS=1)

pico_add_extra_outputs(application)

