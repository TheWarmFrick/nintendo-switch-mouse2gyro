cmake_minimum_required(VERSION 3.13)

# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake OPTIONAL)

project(switch_km_pico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Raspberry Pi Pico SDK
if (NOT PICO_SDK_PATH)
    if (DEFINED ENV{PICO_SDK_PATH})
        set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
    else()
        message(FATAL_ERROR "PICO_SDK_PATH is not set. Please set PICO_SDK_PATH environment variable or pass -DPICO_SDK_PATH=path/to/pico-sdk")
    endif()
endif()

include(${PICO_SDK_PATH}/external/pico_sdk_init.cmake)
pico_sdk_init()

# Include pico-pio-usb library
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/lib/pico-pio-usb/CMakeLists.txt")
    add_subdirectory(lib/pico-pio-usb)
elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/pico-pio-usb/CMakeLists.txt")
    add_subdirectory(pico-pio-usb)
else()
    message(STATUS "pico-pio-usb directory not found directly; assuming provided via include/external or submodule")
endif()

add_executable(switch_km_pico
    main.c
    switch_descriptors.c
    switch_reports.c
    nxic_gyro.c
)

# Include directories
target_include_directories(switch_km_pico PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${PICO_SDK_PATH}/lib/tinyusb/src
)

# Compile definitions
target_compile_definitions(switch_km_pico PRIVATE
    PICO_PIO_USE_GPIO_BASE=0
)

# Link hardware libraries, TinyUSB device, TinyUSB host, and multicore
target_link_libraries(switch_km_pico PRIVATE
    pico_stdlib
    pico_multicore
    hardware_pio
    hardware_dma
    hardware_irq
    hardware_clocks
    tinyusb_device
    tinyusb_host
    tinyusb_board
)

# If pico-pio-usb target is available, link it
if (TARGET pico-pio-usb)
    target_link_libraries(switch_km_pico PRIVATE pico-pio-usb)
endif()

# Enable USB serial output for debug if needed, disable default UART
pico_enable_stdio_usb(switch_km_pico 0)
pico_enable_stdio_uart(switch_km_pico 0)

# Create map/bin/hex/uf2 file
pico_add_extra_outputs(switch_km_pico)
