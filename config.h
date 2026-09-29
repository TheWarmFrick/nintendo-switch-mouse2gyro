#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdbool.h>
#include <stdint.h>
#include "tusb.h"

// ============================================================================
// HARDWARE PIN CONFIGURATION
// ============================================================================
// PIO USB Host pins for the female USB-A breakout port connected to your USB hub.
// Default: GP2 (Pin 4) for D+ and GP3 (Pin 5) for D- on the Raspberry Pi Pico.
#define PIN_PIO_USB_HOST_DP        2  // Data Positive (D+, Green Wire)
#define PIN_PIO_USB_HOST_DM        3  // Data Negative (D-, White Wire)

// Status LED Pin (Pico default onboard green LED is GP25)
#define PIN_STATUS_LED             25

// ============================================================================
// SWITCH CONTROLLER BUTTON BITMASKS (Internal Protocol Definitions)
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
// Primary Face Action Buttons
#define BIND_BUTTON_A              HID_KEY_L
#define BIND_BUTTON_B              HID_KEY_K
#define BIND_BUTTON_X              HID_KEY_I
#define BIND_BUTTON_Y              HID_KEY_J

// Triggers & Shoulder Bumpers
#define BIND_TRIGGER_ZL            HID_KEY_SHIFT_LEFT   // Aim Down Sights (ADS)
#define BIND_TRIGGER_ZR            HID_KEY_SPACE        // Primary Fire / Attack
#define BIND_BUTTON_L              HID_KEY_Q            // Sub-weapon / Tactical
#define BIND_BUTTON_R              HID_KEY_E            // Special / Grenade

// Menu, System & Navigation Buttons
#define BIND_BUTTON_PLUS           HID_KEY_ENTER        // Pause / Start
#define BIND_BUTTON_MINUS          HID_KEY_TAB          // Map / Scoreboard
#define BIND_BUTTON_HOME           HID_KEY_ESCAPE       // Home Menu
#define BIND_BUTTON_CAPTURE        HID_KEY_F12          // Screenshot / Clip

// Analog Stick Clicks
#define BIND_BUTTON_LSTICK         HID_KEY_CONTROL_LEFT // Sprint / Crouch (L3)
#define BIND_BUTTON_RSTICK         HID_KEY_C            // Reset View / Melee (R3)

// Directional Pad (D-Pad)
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
#define BIND_MOUSE_LEFT_CLICK      SWITCH_BUTTON_ZR     // Left Click -> ZR Trigger
#define BIND_MOUSE_RIGHT_CLICK     SWITCH_BUTTON_ZL     // Right Click -> ZL Trigger
#define BIND_MOUSE_MIDDLE_CLICK    SWITCH_BUTTON_RSTICK // Middle Click -> R3 (Gyro Reset)
#define BIND_MOUSE_BACK_CLICK      SWITCH_BUTTON_B      // Thumb Back -> B
#define BIND_MOUSE_FORWARD_CLICK   SWITCH_BUTTON_Y      // Thumb Forward -> Y

// ============================================================================
// MOUSE & GYRO CONFIGURATION (mizuyoukanao/nxic-pico Mathematical Engine)
// ============================================================================
// Sensitivity Multipliers:
// Adjust these floating-point multipliers to match your mouse DPI and preference.
// Typical values range between 1.0f and 2.5f.
#define MOUSE_SENSITIVITY_X        1.50f   // Horizontal motion multiplier (Gyro Yaw)
#define MOUSE_SENSITIVITY_Y        1.50f   // Vertical motion multiplier (Gyro Pitch)

// Scaling factor for Switch IMU units (~16.6 counts per degree per second)
#define GYRO_SCALE_FACTOR          16.6f

// Axis Inversions (Set to true if you prefer inverted vertical aiming)
#define GYRO_INVERT_X              false   // Horizontal Yaw inversion
#define GYRO_INVERT_Y              false   // Vertical Pitch inversion

// Deadzone in pixels to eliminate microscopic sensor drift or hand tremors
#define GYRO_DEADZONE_PIXELS       0.0f

// Sample rolling smoothing (decay factor across the 3 sub-samples in each 15ms frame)
#define GYRO_SAMPLE_DECAY          0.85f

// ============================================================================
// RIGHT ANALOG STICK EMULATION FALLBACK (Optional for Non-Gyro Games)
// ============================================================================
// When true, mouse movement also drives the Right Stick in games without gyro aim.
#define ENABLE_MOUSE_RIGHT_STICK   false
#define STICK_SENSITIVITY_X        25.0f
#define STICK_SENSITIVITY_Y        25.0f
#define STICK_DECAY_RATE           0.75f   // Decays stick back to center (2048)

// 12-bit Analog Stick Coordinate Range (0 to 4095, center is 2048)
#define STICK_CENTER               2048
#define STICK_MIN                  0
#define STICK_MAX                  4095

#endif // CONFIG_H_
