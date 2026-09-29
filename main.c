#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

#include "tusb.h"
#include "pio_usb.h"

#include "config.h"
#include "switch_descriptors.h"
#include "switch_reports.h"
#include "nxic_gyro.h"

// --------------------------------------------------------------------+
// Global State & Thread-Safe Exchange between Cores
// --------------------------------------------------------------------+
static volatile uint32_t s_active_buttons = 0;
static volatile uint16_t s_left_stick_x = STICK_CENTER;
static volatile uint16_t s_left_stick_y = STICK_CENTER;

#ifndef TUH_CFGID_RPI_PIO_USB_CONFIGURATION
#define TUH_CFGID_RPI_PIO_USB_CONFIGURATION 1
#endif

// PIO USB Host Configuration on GP2/GP3
static pio_usb_configuration_t pio_host_cfg = PIO_USB_DEFAULT_CONFIG;

// --------------------------------------------------------------------+
// CORE 1: PIO USB Host Task (Reads Keyboard & Mouse via Hub)
// --------------------------------------------------------------------+
void core1_main(void) {
    pio_host_cfg.pin_dp = PIN_PIO_USB_HOST_DP;
    tuh_configure(BOARD_HOST_RHPORT_NUM, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &pio_host_cfg);
    tuh_init(BOARD_HOST_RHPORT_NUM);

    while (1) {
        tuh_task();
    }
}

// --------------------------------------------------------------------+
// Host HID Callbacks (Invoked on Core 1)
// --------------------------------------------------------------------+
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
    (void) desc_report;
    (void) desc_len;
    tuh_hid_receive_report(dev_addr, instance);
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    (void) dev_addr;
    (void) instance;
}

static void process_keyboard_report(hid_keyboard_report_t const *report) {
    uint32_t buttons = 0;
    bool move_w = false, move_s = false, move_a = false, move_d = false;

    for (uint8_t i = 0; i < 6; i++) {
        uint8_t key = report->keycode[i];
        if (key == 0) continue;

        if (key == BIND_BUTTON_A)       buttons |= SWITCH_BUTTON_A;
        if (key == BIND_BUTTON_B)       buttons |= SWITCH_BUTTON_B;
        if (key == BIND_BUTTON_X)       buttons |= SWITCH_BUTTON_X;
        if (key == BIND_BUTTON_Y)       buttons |= SWITCH_BUTTON_Y;

        if (key == BIND_TRIGGER_ZL)     buttons |= SWITCH_BUTTON_ZL;
        if (key == BIND_TRIGGER_ZR)     buttons |= SWITCH_BUTTON_ZR;
        if (key == BIND_BUTTON_L)       buttons |= SWITCH_BUTTON_L;
        if (key == BIND_BUTTON_R)       buttons |= SWITCH_BUTTON_R;

        if (key == BIND_BUTTON_PLUS)    buttons |= SWITCH_BUTTON_PLUS;
        if (key == BIND_BUTTON_MINUS)   buttons |= SWITCH_BUTTON_MINUS;
        if (key == BIND_BUTTON_HOME)    buttons |= SWITCH_BUTTON_HOME;
        if (key == BIND_BUTTON_CAPTURE) buttons |= SWITCH_BUTTON_CAPTURE;

        if (key == BIND_BUTTON_LSTICK)  buttons |= SWITCH_BUTTON_LSTICK;
        if (key == BIND_BUTTON_RSTICK)  buttons |= SWITCH_BUTTON_RSTICK;

        if (key == BIND_DPAD_UP)        buttons |= SWITCH_BUTTON_UP;
        if (key == BIND_DPAD_DOWN)      buttons |= SWITCH_BUTTON_DOWN;
        if (key == BIND_DPAD_LEFT)      buttons |= SWITCH_BUTTON_LEFT;
        if (key == BIND_DPAD_RIGHT)     buttons |= SWITCH_BUTTON_RIGHT;

        if (key == BIND_MOVE_UP)        move_w = true;
        if (key == BIND_MOVE_DOWN)      move_s = true;
        if (key == BIND_MOVE_LEFT)      move_a = true;
        if (key == BIND_MOVE_RIGHT)     move_d = true;
    }

    if (report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT)) {
        if (BIND_TRIGGER_ZL == HID_KEY_SHIFT_LEFT) buttons |= SWITCH_BUTTON_ZL;
    }
    if (report->modifier & (KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_RIGHTCTRL)) {
        if (BIND_BUTTON_LSTICK == HID_KEY_CONTROL_LEFT) buttons |= SWITCH_BUTTON_LSTICK;
    }

    uint16_t lx = STICK_CENTER;
    uint16_t ly = STICK_CENTER;

    if (move_a && !move_d) lx = STICK_MIN;
    else if (move_d && !move_a) lx = STICK_MAX;

    if (move_w && !move_s) ly = STICK_MAX;
    else if (move_s && !move_w) ly = STICK_MIN;

    s_active_buttons = (s_active_buttons & 0xFF000000) | (buttons & 0x00FFFFFF);
    s_left_stick_x = lx;
    s_left_stick_y = ly;
}

static void process_mouse_report(hid_mouse_report_t const *report) {
    uint32_t mouse_buttons = 0;

    if (report->buttons & MOUSE_BUTTON_LEFT)     mouse_buttons |= BIND_MOUSE_LEFT_CLICK;
    if (report->buttons & MOUSE_BUTTON_RIGHT)    mouse_buttons |= BIND_MOUSE_RIGHT_CLICK;
    if (report->buttons & MOUSE_BUTTON_MIDDLE)   mouse_buttons |= BIND_MOUSE_MIDDLE_CLICK;
    if (report->buttons & MOUSE_BUTTON_BACKWARD) mouse_buttons |= BIND_MOUSE_BACK_CLICK;
    if (report->buttons & MOUSE_BUTTON_FORWARD)  mouse_buttons |= BIND_MOUSE_FORWARD_CLICK;

    s_active_buttons = (s_active_buttons & 0x00FFFFFF) | mouse_buttons;
    nxic_gyro_process_mouse_delta(report->x, report->y);
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
    uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

    if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD) {
        if (len >= sizeof(hid_keyboard_report_t)) {
            process_keyboard_report((hid_keyboard_report_t const*) report);
        }
    } else if (itf_protocol == HID_ITF_PROTOCOL_MOUSE) {
        if (len >= sizeof(hid_mouse_report_t)) {
            process_mouse_report((hid_mouse_report_t const*) report);
        }
    }

    tuh_hid_receive_report(dev_addr, instance);
}

// --------------------------------------------------------------------+
// TinyUSB Device Callbacks (Switch Console Link on Core 0)
// --------------------------------------------------------------------+
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    (void) itf;
    (void) report_id;
    (void) report_type;
    switch_handle_subcommand(buffer, bufsize);
}

uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    (void) itf;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;
    return 0;
}

// --------------------------------------------------------------------+
// CORE 0: Main Routine & Switch Report Dispatcher
// --------------------------------------------------------------------+
int main(void) {
    // 120MHz system clock for stable PIO USB Host timing
    set_sys_clock_khz(120000, true);

    stdio_init_all();

    gpio_init(PIN_STATUS_LED);
    gpio_set_dir(PIN_STATUS_LED, GPIO_OUT);
    gpio_put(PIN_STATUS_LED, 1);

    switch_reports_init();
    nxic_gyro_init();

    tud_init(BOARD_DEVICE_RHPORT_NUM);
    multicore_launch_core1(core1_main);

    uint32_t last_report_time = 0;
    uint32_t last_led_blink = 0;
    bool led_state = false;

    while (1) {
        tud_task();

        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        if (current_time - last_led_blink >= 500) {
            last_led_blink = current_time;
            led_state = !led_state;
            gpio_put(PIN_STATUS_LED, led_state);
        }

        // Nintendo Switch 15ms frame dispatch (~66Hz / 120Hz)
        if (current_time - last_report_time >= 15) {
            last_report_time = current_time;

            int16_t imu_samples[3][6];
            nxic_gyro_get_imu_samples(imu_samples);

            uint16_t rx = STICK_CENTER;
            uint16_t ry = STICK_CENTER;
            nxic_gyro_get_stick_fallback(&rx, &ry);

            switch_send_report(
                s_active_buttons,
                s_left_stick_x,
                s_left_stick_y,
                rx,
                ry,
                imu_samples
            );
        }
    }

    return 0;
}
