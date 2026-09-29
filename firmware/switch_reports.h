#ifndef SWITCH_REPORTS_H_
#define SWITCH_REPORTS_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Switch Input Report IDs
#define SWITCH_REPORT_ID_SUBCOMMAND_RESP 0x21
#define SWITCH_REPORT_ID_STANDARD_FULL   0x30
#define SWITCH_REPORT_ID_HANDSHAKE_RESP  0x81

// Controller Types
#define SWITCH_DEVICE_TYPE_PRO_CONTROLLER 0x03

// Subcommands sent from Switch console to controller
#define SUBCOMMAND_BLUETOOTH_PAIRING     0x01
#define SUBCOMMAND_REQUEST_DEVICE_INFO   0x02
#define SUBCOMMAND_SET_REPORT_MODE       0x03
#define SUBCOMMAND_TRIGGER_ELAPSED_TIME  0x04
#define SUBCOMMAND_SPI_FLASH_READ        0x10
#define SUBCOMMAND_SET_LIGHTS            0x30
#define SUBCOMMAND_ENABLE_IMU            0x40
#define SUBCOMMAND_SET_PLAYER_LIGHTS     0x30
#define SUBCOMMAND_ENABLE_VIBRATION      0x48

// IMU motion sample structure (6-axis: 3 accel + 3 gyro, 12 bytes total)
typedef struct __attribute__((packed)) {
    int16_t accel_x;  // Accelerometer X
    int16_t accel_y;  // Accelerometer Y
    int16_t accel_z;  // Accelerometer Z (1G gravity vector ~4096 counts)
    int16_t gyro_x;   // Gyro Pitch angular velocity
    int16_t gyro_y;   // Gyro Yaw angular velocity
    int16_t gyro_z;   // Gyro Roll angular velocity
} switch_imu_sample_t;

// Standard Full 0x30 Input Report Structure (49 bytes)
typedef struct __attribute__((packed)) {
    uint8_t report_id;               // 0x30
    uint8_t timer;                   // Rolling increment timer (0 - 255)
    uint8_t battery_status;          // Battery level (0x8) | connection status (0xE = USB)
    uint8_t button_right;            // Y, X, B, A, SR, SL, R, ZR
    uint8_t button_shared;           // Minus, Plus, RStick, LStick, Home, Capture
    uint8_t button_left;             // Down, Up, Right, Left, SR, SL, L, ZL
    uint8_t left_stick[3];           // 12-bit X & 12-bit Y packed
    uint8_t right_stick[3];          // 12-bit X & 12-bit Y packed
    uint8_t vibrator_ack;            // Vibration feedback ack
    switch_imu_sample_t imu[3];      // 3 rolling IMU samples (5ms interval)
} switch_report_0x30_t;

// Subcommand Response Report 0x21 Structure
typedef struct __attribute__((packed)) {
    uint8_t report_id;               // 0x21
    uint8_t timer;
    uint8_t battery_status;
    uint8_t button_right;
    uint8_t button_shared;
    uint8_t button_left;
    uint8_t left_stick[3];
    uint8_t right_stick[3];
    uint8_t vibrator_ack;
    uint8_t ack;                     // 0x80 | Subcommand ID
    uint8_t subcommand_id;
    uint8_t subcommand_reply[35];    // Reply payload
} switch_report_0x21_t;

// Public API
void switch_reports_init(void);
void switch_handle_subcommand(const uint8_t *data, uint16_t len);
bool switch_send_report(uint32_t buttons, uint16_t lx, uint16_t ly, uint16_t rx, uint16_t ry, const int16_t imu_samples[3][6]);
bool switch_is_imu_enabled(void);
uint8_t switch_get_report_mode(void);

#ifdef __cplusplus
}
#endif

#endif // SWITCH_REPORTS_H_
