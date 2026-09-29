#ifndef TUSB_CONFIG_H_
#define TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// COMMON CONFIGURATION
// ============================================================================
#ifndef CFG_TUSB_MCU
#define CFG_TUSB_MCU                OPT_MCU_RP2040
#endif

#ifndef CFG_TUSB_OS
#define CFG_TUSB_OS                 OPT_OS_NONE
#endif

// Native USB port (RHPort 0) is configured as USB DEVICE (Switch Link)
#define CFG_TUD_ENABLED             1
#define BOARD_DEVICE_RHPORT_NUM     0

// PIO USB Host (RHPort 1) is configured as USB HOST (Keyboard & Mouse Link)
#define CFG_TUH_ENABLED             1
#define CFG_TUH_RPI_PIO_USB         1
#define BOARD_HOST_RHPORT_NUM       1

#ifndef CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_SECTION
#endif

#ifndef CFG_TUSB_MEM_ALIGN
#define CFG_TUSB_MEM_ALIGN          __attribute__((aligned(4)))
#endif

// ============================================================================
// DEVICE CONFIGURATION (Nintendo Switch Controller Emulation)
// ============================================================================
#define CFG_TUD_ENDPOINT0_SIZE      64
#define CFG_TUD_HID                 1
#define CFG_TUD_CDC                 0
#define CFG_TUD_MSC                 0
#define CFG_TUD_MIDI                0
#define CFG_TUD_VENDOR              0
#define CFG_TUD_HID_EP_BUFSIZE      64

// ============================================================================
// HOST CONFIGURATION (PIO-USB Host for Keyboard & Mouse via Hub)
// ============================================================================
#define CFG_TUH_DEVICE_MAX          4
#define CFG_TUH_ENUMERATION_BUFSIZE 256
#define CFG_TUH_HUB                 1
#define CFG_TUH_HID                 4
#define CFG_TUH_CDC                 0
#define CFG_TUH_MSC                 0
#define CFG_TUH_HID_EPIN_BUFSIZE    64
#define CFG_TUH_HID_EPOUT_BUFSIZE   64

#ifdef __cplusplus
}
#endif

#endif // TUSB_CONFIG_H_
