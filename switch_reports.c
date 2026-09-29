#include "switch_reports.h"
#include "tusb.h"
#include <string.h>

static uint8_t s_report_mode = SWITCH_REPORT_ID_STANDARD_FULL;
static bool s_imu_enabled = true;
static uint8_t s_timer_counter = 0;
static uint8_t s_subcommand_pending = 0;
static switch_report_0x21_t s_subcommand_resp;

void switch_reports_init(void) {
    s_report_mode = SWITCH_REPORT_ID_STANDARD_FULL;
    s_imu_enabled = true;
    s_timer_counter = 0;
    s_subcommand_pending = 0;
    memset(&s_subcommand_resp, 0, sizeof(s_subcommand_resp));
}

bool switch_is_imu_enabled(void) {
    return s_imu_enabled;
}

uint8_t switch_get_report_mode(void) {
    return s_report_mode;
}

void switch_handle_subcommand(const uint8_t *data, uint16_t len) {
    if (len < 1) return;

    uint8_t report_id = data[0];

    // USB Direct Handshake Commands (0x80)
    if (report_id == 0x80) {
        if (len < 2) return;
        uint8_t cmd = data[1];
        uint8_t handshake_resp[64] = {0};
        handshake_resp[0] = SWITCH_REPORT_ID_HANDSHAKE_RESP;
        handshake_resp[1] = cmd;

        if (cmd == 0x01) {
            handshake_resp[2] = 0x00;
            handshake_resp[3] = SWITCH_DEVICE_TYPE_PRO_CONTROLLER;
            handshake_resp[4] = 0x00; handshake_resp[5] = 0x0C; handshake_resp[6] = 0xBF;
            handshake_resp[7] = 0x12; handshake_resp[8] = 0x34; handshake_resp[9] = 0x56;
        } else if (cmd == 0x02) {
            handshake_resp[2] = 0x01;
        } else if (cmd == 0x03) {
            handshake_resp[2] = 0x01;
        }

        if (tud_hid_ready()) {
            tud_hid_report(0, handshake_resp, sizeof(handshake_resp));
        }
        return;
    }

    // Standard Subcommand Packets (Report 0x01 or 0x10)
    if (report_id == 0x01 || report_id == 0x10) {
        if (len < 11) return;

        uint8_t subcmd = data[10];

        memset(&s_subcommand_resp, 0, sizeof(s_subcommand_resp));
        s_subcommand_resp.report_id = SWITCH_REPORT_ID_SUBCOMMAND_RESP;
        s_subcommand_resp.timer = s_timer_counter++;
        s_subcommand_resp.battery_status = 0x8E; // Fully charged, USB powered
        s_subcommand_resp.ack = 0x80 | subcmd;
        s_subcommand_resp.subcommand_id = subcmd;

        switch (subcmd) {
            case SUBCOMMAND_BLUETOOTH_PAIRING: {
                s_subcommand_resp.subcommand_reply[0] = 0x03;
                break;
            }

            case SUBCOMMAND_REQUEST_DEVICE_INFO: {
                s_subcommand_resp.subcommand_reply[0] = 0x03;
                s_subcommand_resp.subcommand_reply[1] = 0x48; // Firmware 3.48
                s_subcommand_resp.subcommand_reply[2] = SWITCH_DEVICE_TYPE_PRO_CONTROLLER;
                s_subcommand_resp.subcommand_reply[3] = 0x02;
                s_subcommand_resp.subcommand_reply[4] = 0x00;
                s_subcommand_resp.subcommand_reply[5] = 0x0C;
                s_subcommand_resp.subcommand_reply[6] = 0xBF;
                s_subcommand_resp.subcommand_reply[7] = 0x88;
                s_subcommand_resp.subcommand_reply[8] = 0x77;
                s_subcommand_resp.subcommand_reply[9] = 0x66;
                s_subcommand_resp.subcommand_reply[10] = 0x01;
                s_subcommand_resp.subcommand_reply[11] = 0x01;
                break;
            }

            case SUBCOMMAND_SET_REPORT_MODE: {
                if (len > 11) {
                    s_report_mode = data[11];
                }
                break;
            }

            case SUBCOMMAND_TRIGGER_ELAPSED_TIME: {
                s_subcommand_resp.subcommand_reply[0] = 0x00;
                break;
            }

            case SUBCOMMAND_SPI_FLASH_READ: {
                if (len >= 15) {
                    uint32_t addr = data[11] | (data[12] << 8) | (data[13] << 16) | (data[14] << 24);
                    uint8_t read_len = data[15];
                    if (read_len > 24) read_len = 24;

                    s_subcommand_resp.subcommand_reply[0] = data[11];
                    s_subcommand_resp.subcommand_reply[1] = data[12];
                    s_subcommand_resp.subcommand_reply[2] = data[13];
                    s_subcommand_resp.subcommand_reply[3] = data[14];
                    s_subcommand_resp.subcommand_reply[4] = read_len;

                    uint8_t *payload = &s_subcommand_resp.subcommand_reply[5];

                    if (addr >= 0x6020 && addr < 0x6040) {
                        payload[0] = 0x32; payload[1] = 0x32; payload[2] = 0x32;
                        payload[3] = 0xFF; payload[4] = 0xFF; payload[5] = 0xFF;
                    } else if (addr >= 0x603D && addr <= 0x6045) {
                        payload[0] = 0x18; payload[1] = 0x18; payload[2] = 0x18;
                    } else if (addr >= 0x6050 && addr < 0x6070) {
                        // IMU Factory Sensor Calibration (Accelerometer & Gyroscope)
                        payload[0] = 0x00; payload[1] = 0x00;
                        payload[2] = 0x00; payload[3] = 0x00;
                        payload[4] = 0x00; payload[5] = 0x00;
                        // Accel Sensitivity (1G = 4096 = 0x1000)
                        payload[6] = 0x00; payload[7] = 0x10;
                        payload[8] = 0x00; payload[9] = 0x10;
                        payload[10] = 0x00; payload[11] = 0x10;
                        // Gyro Origin
                        payload[12] = 0x00; payload[13] = 0x00;
                        payload[14] = 0x00; payload[15] = 0x00;
                        payload[16] = 0x00; payload[17] = 0x00;
                        // Gyro Sensitivity Scale Factor (~13371 = 0x344B)
                        payload[18] = 0x4B; payload[19] = 0x34;
                        payload[20] = 0x4B; payload[21] = 0x34;
                        payload[22] = 0x4B; payload[23] = 0x34;
                    } else if (addr >= 0x6080 && addr < 0x60A0) {
                        payload[0] = 0x00; payload[1] = 0x08; payload[2] = 0x80;
                        payload[3] = 0x00; payload[4] = 0x08; payload[5] = 0x80;
                    } else {
                        memset(payload, 0xFF, read_len);
                    }
                }
                break;
            }

            case SUBCOMMAND_ENABLE_IMU: {
                if (len > 11) {
                    s_imu_enabled = (data[11] != 0);
                }
                break;
            }

            case SUBCOMMAND_ENABLE_VIBRATION: {
                break;
            }

            default:
                break;
        }

        s_subcommand_pending = 1;
    }
}

bool switch_send_report(uint32_t buttons, uint16_t lx, uint16_t ly, uint16_t rx, uint16_t ry, const int16_t imu_samples[3][6]) {
    if (!tud_hid_ready()) return false;

    if (s_subcommand_pending) {
        s_subcommand_pending = 0;
        return tud_hid_report(0, &s_subcommand_resp, sizeof(s_subcommand_resp));
    }

    switch_report_0x30_t report;
    memset(&report, 0, sizeof(report));

    report.report_id = SWITCH_REPORT_ID_STANDARD_FULL;
    report.timer = s_timer_counter++;
    report.battery_status = 0x8E;

    report.button_right  = (uint8_t)(buttons & 0xFF);
    report.button_shared = (uint8_t)((buttons >> 8) & 0xFF);
    report.button_left   = (uint8_t)((buttons >> 16) & 0xFF);

    if (lx > 4095) lx = 4095;
    if (ly > 4095) ly = 4095;
    if (rx > 4095) rx = 4095;
    if (ry > 4095) ry = 4095;

    report.left_stick[0] = (uint8_t)(lx & 0xFF);
    report.left_stick[1] = (uint8_t)(((lx >> 8) & 0x0F) | ((ly & 0x0F) << 4));
    report.left_stick[2] = (uint8_t)((ly >> 4) & 0xFF);

    report.right_stick[0] = (uint8_t)(rx & 0xFF);
    report.right_stick[1] = (uint8_t)(((rx >> 8) & 0x0F) | ((ry & 0x0F) << 4));
    report.right_stick[2] = (uint8_t)((ry >> 4) & 0xFF);

    report.vibrator_ack = 0x00;

    for (int i = 0; i < 3; i++) {
        report.imu[i].accel_x = imu_samples[i][0];
        report.imu[i].accel_y = imu_samples[i][1];
        report.imu[i].accel_z = imu_samples[i][2]; // 1G gravity ~4096
        report.imu[i].gyro_x  = imu_samples[i][3]; // Pitch
        report.imu[i].gyro_y  = imu_samples[i][4]; // Yaw
        report.imu[i].gyro_z  = imu_samples[i][5]; // Roll
    }

    return tud_hid_report(0, &report, sizeof(report));
}
