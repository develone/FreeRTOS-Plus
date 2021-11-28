# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)
# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

project(example C CXX ASM)

##set(PICO_CXX_ENABLE_EXCEPTIONS 1)
##set(PICO_CXX_ENABLE_RTTI 1)

set(PARAM_ASSERTIONS_ENABLE_ALL 1)

##set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")

# Initialise the Pico SDK
pico_sdk_init()

add_subdirectory(../freertos build)

# Add executable. Default name is the project name, version 0.1
add_executable(example
        example.c
        hw_config.c
        stdio_cli.c
        tests/app4-IO_module_function_checker.c        
        tests/filesystem_test_suite.c
        tests/CreateAndVerifyExampleFiles.c        
        tests/big_file_test.c
        tests/ff_stdio_tests_with_cwd.c
        tests/my_test.c
        tests/mt_lliot.c
        data_log_demo.c
)

pico_set_program_name(example "example")
pico_set_program_version(example "0.1")

pico_enable_stdio_uart(example 1)
pico_enable_stdio_usb(example 0)

# Add the standard library to the build
target_link_libraries(example 
        FreeRTOS+FAT+CLI
)
target_include_directories(example PUBLIC 
        include/ 
        ../FreeRTOS+FAT+CLI/Lab-Project-FreeRTOS-FAT/include
        ../FreeRTOS+FAT+CLI/portable/RP2040
        ../FreeRTOS+FAT+CLI/include
        #${FREERTOS_KERNEL_PATH}/include 
        #${FREERTOS_KERNEL_PATH}/${FREERTOS_KERNEL_RP2040_RELATIVE_PATH}/include
)

target_compile_options(example PUBLIC
    -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-type-limits
    -Wno-sign-compare -Wno-unused-variable -Wno-format-truncation -Wno-format-overflow
    -Wno-maybe-uninitialized
)
target_compile_definitions(example PUBLIC DEBUG)

target_compile_definitions(example PUBLIC DEBUG N_SD_CARDS=1)

pico_add_extra_outputs(example)

