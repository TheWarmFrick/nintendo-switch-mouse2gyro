#include "nxic_gyro.h"
#include <math.h>

static nxic_gyro_state_t s_gyro_state;
static float s_stick_accum_x = 0.0f;
static float s_stick_accum_y = 0.0f;

static inline int16_t clamp_int16(float val) {
    if (val > 32767.0f) return 32767;
    if (val < -32768.0f) return -32768;
    return (int16_t)val;
}

void nxic_gyro_init(void) {
    s_gyro_state.current_yaw_rate = 0.0f;
    s_gyro_state.current_pitch_rate = 0.0f;
    s_gyro_state.accumulated_dx = 0.0f;
    s_gyro_state.accumulated_dy = 0.0f;
    s_gyro_state.last_gyro_x = 0;
    s_gyro_state.last_gyro_y = 0;
    s_gyro_state.last_gyro_z = 0;
    s_gyro_state.last_accel_x = 0;
    s_gyro_state.last_accel_y = 0;
    s_gyro_state.last_accel_z = SWITCH_IMU_1G_ACCEL;
    s_stick_accum_x = 0.0f;
    s_stick_accum_y = 0.0f;
}

void nxic_gyro_reset(void) {
    nxic_gyro_init();
}

void nxic_gyro_process_mouse_delta(int16_t dx, int16_t dy) {
    float fdx = (float)dx;
    float fdy = (float)dy;

    // Apply pixel deadzone filter
    if (fabsf(fdx) < GYRO_DEADZONE_PIXELS) fdx = 0.0f;
    if (fabsf(fdy) < GYRO_DEADZONE_PIXELS) fdy = 0.0f;

    // Accumulate deltas within current frame window
    s_gyro_state.accumulated_dx += fdx;
    s_gyro_state.accumulated_dy += fdy;

    // nxic-pico Mathematical Mapping:
    // Mouse horizontal delta (dX) -> Gyro Yaw rate
    // Mouse vertical delta (dY)   -> Gyro Pitch rate
    float target_yaw = s_gyro_state.accumulated_dx * MOUSE_SENSITIVITY_X * GYRO_SCALE_FACTOR;
    float target_pitch = -s_gyro_state.accumulated_dy * MOUSE_SENSITIVITY_Y * GYRO_SCALE_FACTOR;

    #if GYRO_INVERT_X
    target_yaw = -target_yaw;
    #endif

    #if GYRO_INVERT_Y
    target_pitch = -target_pitch;
    #endif

    s_gyro_state.current_yaw_rate = target_yaw;
    s_gyro_state.current_pitch_rate = target_pitch;

    // Right Stick fallback integration if enabled
    #if ENABLE_MOUSE_RIGHT_STICK
    s_stick_accum_x += fdx * STICK_SENSITIVITY_X;
    s_stick_accum_y -= fdy * STICK_SENSITIVITY_Y;
    #endif
}

void nxic_gyro_get_imu_samples(int16_t samples[3][6]) {
    // Generate 3 sub-samples spaced 5ms apart across the 15ms Switch polling window
    float decay = GYRO_SAMPLE_DECAY;
    float decay_factor = 1.0f;

    for (int i = 0; i < 3; i++) {
        // Accelerometer: standard level orientation (1G down on Z axis)
        samples[i][0] = 0;                     // Accel X
        samples[i][1] = 0;                     // Accel Y
        samples[i][2] = SWITCH_IMU_1G_ACCEL;   // Accel Z (4096 LSB)

        // Gyroscope: Pitch (X), Yaw (Y), Roll (Z)
        float pitch_sub = s_gyro_state.current_pitch_rate * decay_factor;
        float yaw_sub   = s_gyro_state.current_yaw_rate * decay_factor;

        samples[i][3] = clamp_int16(pitch_sub); // Gyro X: Pitch
        samples[i][4] = clamp_int16(yaw_sub);   // Gyro Y: Yaw
        samples[i][5] = 0;                      // Gyro Z: Roll (stationary)

        decay_factor *= decay;
    }

    // Decay the accumulated deltas towards zero for the next frame
    s_gyro_state.current_yaw_rate *= 0.5f;
    s_gyro_state.current_pitch_rate *= 0.5f;
    s_gyro_state.accumulated_dx = 0.0f;
    s_gyro_state.accumulated_dy = 0.0f;
}

void nxic_gyro_get_stick_fallback(uint16_t *rx, uint16_t *ry) {
    #if ENABLE_MOUSE_RIGHT_STICK
    int32_t val_x = STICK_CENTER + (int32_t)s_stick_accum_x;
    int32_t val_y = STICK_CENTER + (int32_t)s_stick_accum_y;

    if (val_x < STICK_MIN) val_x = STICK_MIN;
    if (val_x > STICK_MAX) val_x = STICK_MAX;
    if (val_y < STICK_MIN) val_y = STICK_MIN;
    if (val_y > STICK_MAX) val_y = STICK_MAX;

    *rx = (uint16_t)val_x;
    *ry = (uint16_t)val_y;

    // Decay stick back to center
    s_stick_accum_x *= STICK_DECAY_RATE;
    s_stick_accum_y *= STICK_DECAY_RATE;
    #else
    *rx = STICK_CENTER;
    *ry = STICK_CENTER;
    #endif
}
