#ifndef PIO_USB_CONFIGURATION_H_
#define PIO_USB_CONFIGURATION_H_

#include "config.h"

// Configuration parameters for the pico-pio-usb library
#define PIO_USB_DP_PIN_DEFAULT         PIN_PIO_USB_HOST_DP
#define PIO_USB_DM_PIN_DEFAULT         PIN_PIO_USB_HOST_DM

// Number of host ports enabled on PIO
#define PIO_USB_HOST_PORT_NUM          1

// Use PIO0 for USB Host
#define PIO_USB_PIO_INSTANCE           0

#endif // PIO_USB_CONFIGURATION_H_
