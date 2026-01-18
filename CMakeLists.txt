# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(tiny_mnist C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(lib/pico-tflmicro EXCLUDE_FROM_ALL)

# Add executable. Default name is the project name, version 0.1

add_executable(tiny_mnist 
    tiny_mnist.c
    tflm_wrapper.cpp
    )

pico_set_program_name(tiny_mnist "tiny_mnist")
pico_set_program_version(tiny_mnist "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(tiny_mnist 1)
pico_enable_stdio_usb(tiny_mnist 1)

# Add the standard library to the build
target_link_libraries(tiny_mnist
        pico_stdlib
        hardware_sync
        pico-tflmicro
        )

# Add the standard include files to the build
target_include_directories(tiny_mnist PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/pico-tflmicro/src
)

pico_add_extra_outputs(tiny_mnist)

