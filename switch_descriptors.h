#ifndef SWITCH_DESCRIPTORS_H_
#define SWITCH_DESCRIPTORS_H_

#include <stdint.h>
#include "tusb.h"

#ifdef __cplusplus
extern "C" {
#endif

// Official Nintendo Switch Pro Controller Identifiers
#define SWITCH_USB_VID              0x057E  // Nintendo Co., Ltd.
#define SWITCH_USB_PID              0x2009  // Switch Pro Controller
#define SWITCH_USB_BCD_DEVICE       0x0200  // Version 2.00

// Endpoint addresses
#define SWITCH_EP_IN                0x81
#define SWITCH_EP_OUT               0x01
#define SWITCH_EP_SIZE              64

// HID Report Descriptor Length (88 bytes)
#define SWITCH_REPORT_DESC_LEN      88

extern const uint8_t switch_report_descriptor[];

#ifdef __cplusplus
}
#endif

#endif // SWITCH_DESCRIPTORS_H_
