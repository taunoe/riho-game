# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

# Pico-W board
set(PICO_BOARD pico_w) # väga tähtis!

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

include(pico_extras_import_optional.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(main C CXX ASM)
set(CMAKE_C STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(
    ${PROJECT_NAME}
    main.c
    dhcpserver/dhcpserver.c
    dnsserver/dnsserver.c
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
    ${CMAKE_CURRENT_LIST_DIR}/dhcpserver
    ${CMAKE_CURRENT_LIST_DIR}/dnsserver
)

target_link_libraries(
    ${PROJECT_NAME}
    pico_cyw43_arch_lwip_threadsafe_background
    pico_stdlib
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
