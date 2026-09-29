cmake_minimum_required(VERSION 3.13)

# Include Pico SDK importer
include(pico_sdk_import.cmake)

project(switch_km_pico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Locate or add pico-pio-usb library
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/lib/pico-pio-usb/CMakeLists.txt")
    add_subdirectory(lib/pico-pio-usb)
elseif (EXISTS "${CMAKE_CURRENT_LIST_DIR}/pico-pio-usb/CMakeLists.txt")
    add_subdirectory(pico-pio-usb)
endif()

# Firmware executable target
add_executable(switch_km_pico
    main.c
    switch_descriptors.c
    switch_reports.c
    nxic_gyro.c
)

# Header include paths
target_include_directories(switch_km_pico PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${PICO_SDK_PATH}/lib/tinyusb/src
)

# Hardware and TinyUSB libraries
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

# Link pico-pio-usb if present
if (TARGET pico-pio-usb)
    target_link_libraries(switch_km_pico PRIVATE pico-pio-usb)
endif()

# Disable default UART to save power and prevent pin conflicts
pico_enable_stdio_uart(switch_km_pico 0)
pico_enable_stdio_usb(switch_km_pico 0)

# Generate switch_km_pico.uf2, .hex, .bin, and .map
pico_add_extra_outputs(switch_km_pico)
