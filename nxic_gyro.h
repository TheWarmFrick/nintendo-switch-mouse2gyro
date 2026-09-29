#ifndef NXIC_GYRO_H_
#define NXIC_GYRO_H_

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// 1G gravity representation in Switch IMU units (4096 LSB)
#define SWITCH_IMU_1G_ACCEL          4096

typedef struct {
    float current_yaw_rate;
    float current_pitch_rate;
    float accumulated_dx;
    float accumulated_dy;
    int16_t last_gyro_x;
    int16_t last_gyro_y;
    int16_t last_gyro_z;
    int16_t last_accel_x;
    int16_t last_accel_y;
    int16_t last_accel_z;
} nxic_gyro_state_t;

void nxic_gyro_init(void);
void nxic_gyro_process_mouse_delta(int16_t dx, int16_t dy);
void nxic_gyro_get_imu_samples(int16_t samples[3][6]);
void nxic_gyro_reset(void);
void nxic_gyro_get_stick_fallback(uint16_t *rx, uint16_t *ry);

#ifdef __cplusplus
}
#endif

#endif // NXIC_GYRO_H_
