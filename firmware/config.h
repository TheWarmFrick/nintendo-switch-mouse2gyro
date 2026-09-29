#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdbool.h>
#include <stdint.h>
#include "tusb.h"

// ============================================================================
// HARDWARE PIN CONFIGURATION
// ============================================================================
// PIO USB Host pins for the female USB-A port breakout connected to USB Hub.
// Default: GP2 (D+) and GP3 (D-) on the Raspberry Pi Pico.
#define PIN_PIO_USB_HOST_DP        2  // Data Positive (D+, Green Wire)
#define PIN_PIO_USB_HOST_DM        3  // Data Negative (D-, White Wire)

// Status LED Pin (Pico default onboard LED is GP25)
#define PIN_STATUS_LED             25

// ============================================================================
// SWITCH CONTROLLER BUTTON BITMASKS (Internal Enum/Constants)
// ============================================================================
#define SWITCH_BUTTON_Y            (1 << 0)
#define SWITCH_BUTTON_X            (1 << 1)
#define SWITCH_BUTTON_B            (1 << 2)
#define SWITCH_BUTTON_A            (1 << 3)
#define SWITCH_BUTTON_SR_RIGHT     (1 << 4)
#define SWITCH_BUTTON_SL_RIGHT     (1 << 5)
#define SWITCH_BUTTON_R            (1 << 6)
#define SWITCH_BUTTON_ZR           (1 << 7)

#define SWITCH_BUTTON_MINUS        (1 << 8)
#define SWITCH_BUTTON_PLUS         (1 << 9)
#define SWITCH_BUTTON_RSTICK       (1 << 10)
#define SWITCH_BUTTON_LSTICK       (1 << 11)
#define SWITCH_BUTTON_HOME         (1 << 12)
#define SWITCH_BUTTON_CAPTURE      (1 << 13)

#define SWITCH_BUTTON_DOWN         (1 << 16)
#define SWITCH_BUTTON_UP           (1 << 17)
#define SWITCH_BUTTON_RIGHT        (1 << 18)
#define SWITCH_BUTTON_LEFT         (1 << 19)
#define SWITCH_BUTTON_SR_LEFT      (1 << 20)
#define SWITCH_BUTTON_SL_LEFT      (1 << 21)
#define SWITCH_BUTTON_L            (1 << 22)
#define SWITCH_BUTTON_ZL           (1 << 23)

// ============================================================================
// KEYBOARD TO SWITCH CONTROLLER BINDINGS
// ============================================================================
// Action Buttons
#define BIND_BUTTON_A              HID_KEY_L
#define BIND_BUTTON_B              HID_KEY_K
#define BIND_BUTTON_X              HID_KEY_I
#define BIND_BUTTON_Y              HID_KEY_J

// Triggers & Bumpers
#define BIND_TRIGGER_ZL            HID_KEY_SHIFT_LEFT
#define BIND_TRIGGER_ZR            HID_KEY_SPACE
#define BIND_BUTTON_L              HID_KEY_Q
#define BIND_BUTTON_R              HID_KEY_E

// System & Menu Buttons
#define BIND_BUTTON_PLUS           HID_KEY_ENTER
#define BIND_BUTTON_MINUS          HID_KEY_TAB
#define BIND_BUTTON_HOME           HID_KEY_ESCAPE
#define BIND_BUTTON_CAPTURE        HID_KEY_F12

// Stick Clicks
#define BIND_BUTTON_LSTICK         HID_KEY_CONTROL_LEFT
#define BIND_BUTTON_RSTICK         HID_KEY_C

// D-Pad Navigation
#define BIND_DPAD_UP               HID_KEY_ARROW_UP
#define BIND_DPAD_DOWN             HID_KEY_ARROW_DOWN
#define BIND_DPAD_LEFT             HID_KEY_ARROW_LEFT
#define BIND_DPAD_RIGHT            HID_KEY_ARROW_RIGHT

// Movement (Left Analog Stick)
#define BIND_MOVE_UP               HID_KEY_W
#define BIND_MOVE_DOWN             HID_KEY_S
#define BIND_MOVE_LEFT             HID_KEY_A
#define BIND_MOVE_RIGHT            HID_KEY_D

// ============================================================================
// MOUSE CLICK BINDINGS
// ============================================================================
#define BIND_MOUSE_LEFT_CLICK      SWITCH_BUTTON_ZR
#define BIND_MOUSE_RIGHT_CLICK     SWITCH_BUTTON_ZL
#define BIND_MOUSE_MIDDLE_CLICK    SWITCH_BUTTON_RSTICK
#define BIND_MOUSE_BACK_CLICK      SWITCH_BUTTON_B
#define BIND_MOUSE_FORWARD_CLICK   SWITCH_BUTTON_Y

// ============================================================================
// MOUSE & GYRO MOTION CONFIGURATION (nxic-pico Mathematical Model)
// ============================================================================
// Floating-point sensitivity multipliers for mouse delta to gyro angular velocity
#define MOUSE_SENSITIVITY_X        1.50f   // Horizontal motion multiplier (Yaw)
#define MOUSE_SENSITIVITY_Y        1.50f   // Vertical motion multiplier (Pitch)

// Scaling factor for Switch IMU units (raw IMU units: ~16.6 counts per deg/sec)
#define GYRO_SCALE_FACTOR          16.6f

// Axis Inversion Settings
#define GYRO_INVERT_X              false   // Invert Yaw (horizontal)
#define GYRO_INVERT_Y              false   // Invert Pitch (vertical)

// Gyro deadzone in pixels to eliminate sensor noise / microscopic drift
#define GYRO_DEADZONE_PIXELS       0.0f

// Sample rolling smoothing (0.0f = no decay, 0.85f = smooth 3-sample distribution)
#define GYRO_SAMPLE_DECAY          0.85f

// ============================================================================
// RIGHT STICK EMULATION FALLBACK (For non-gyro games)
// ============================================================================
// Set to true if you want mouse deltas to ALSO drive the Right Analog Stick
#define ENABLE_MOUSE_RIGHT_STICK   false
#define STICK_SENSITIVITY_X        25.0f
#define STICK_SENSITIVITY_Y        25.0f
#define STICK_DECAY_RATE           0.75f   // Decays stick back to center when mouse stops

// Analog Stick Coordinate Limits (12-bit range 0-4095, center is 2048)
#define STICK_CENTER               2048
#define STICK_MIN                  0
#define STICK_MAX                  4095

#endif // CONFIG_H_
