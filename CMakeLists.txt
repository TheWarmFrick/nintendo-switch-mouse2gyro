cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico CACHE STRING "Board type")
include(pico_sdk_import.cmake)

# Configure Pico-PIO-USB BEFORE pico_sdk_init()
# This allows Pico SDK to automatically configure TinyUSB with PIO-USB host support
if (NOT DEFINED PICO_PIO_USB_PATH)
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/lib/Pico-PIO-USB/src/pio_usb.c)
        set(PICO_PIO_USB_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/Pico-PIO-USB)
    elseif (EXISTS ${CMAKE_CURRENT_LIST_DIR}/lib/pico-pio-usb/src/pio_usb.c)
        set(PICO_PIO_USB_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/pico-pio-usb)
    else()
        include(FetchContent)
        FetchContent_Declare(
            pico_pio_usb_fetch
            GIT_REPOSITORY https://github.com/sekigon-gonnoc/Pico-PIO-USB
            GIT_TAG main
        )
        FetchContent_GetProperties(pico_pio_usb_fetch)
        if (NOT pico_pio_usb_fetch_POPULATED)
            FetchContent_Populate(pico_pio_usb_fetch)
        endif()
        set(PICO_PIO_USB_PATH ${pico_pio_usb_fetch_SOURCE_DIR})
    endif()
endif()

message("Pico-PIO-USB path: ${PICO_PIO_USB_PATH}")

project(switch_km_pico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

add_executable(switch_km_pico
    main.c
    switch_descriptors.c
    switch_reports.c
    nxic_gyro.c
)

target_include_directories(switch_km_pico PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${PICO_PIO_USB_PATH}/src
)

target_link_libraries(switch_km_pico
    pico_stdlib
    pico_multicore
    pico_unique_id
    hardware_pio
    tinyusb_device
    tinyusb_host
    tinyusb_pico_pio_usb
)

# Disable default stdio UART/USB to prevent pin conflicts
pico_enable_stdio_uart(switch_km_pico 0)
pico_enable_stdio_usb(switch_km_pico 0)

# Generate UF2 artifact
pico_add_extra_outputs(switch_km_pico)
