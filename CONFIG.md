# Firmware Configuration Guide (`config.h`)

All user-tunable parameters for pins, button bindings, analog stick ranges, mouse sensitivity, and gyro calculations are contained inside **`config.h`**.

You can adjust these settings without modifying any core USB or protocol logic. After changing `config.h`, simply recompile and re-flash the `.uf2` binary to your Pico.

---

## 🎮 Default Keybindings

The adapter maps standard USB HID keyboard scan codes and mouse clicks directly to official Nintendo Switch Pro Controller inputs:

| Nintendo Switch Input | Default Keyboard / Mouse Binding | `config.h` Define | Notes |
|:---|:---|:---|:---|
| **A Button** | `L` Key | `BIND_BUTTON_A` | Confirm / Primary Action |
| **B Button** | `K` Key | `BIND_BUTTON_B` | Cancel / Jump |
| **X Button** | `I` Key | `BIND_BUTTON_X` | Inventory / Weapon Swap |
| **Y Button** | `J` Key | `BIND_BUTTON_Y` | Reload / Secondary Attack |
| **ZL Trigger** | `Left Shift` / `Right Mouse Click` | `BIND_TRIGGER_ZL` | Aim Down Sights (ADS) / Secondary |
| **ZR Trigger** | `Spacebar` / `Left Mouse Click` | `BIND_TRIGGER_ZR` | Primary Fire / Attack |
| **L Shoulder** | `Q` Key | `BIND_BUTTON_L` | Tactical / Sub-weapon |
| **R Shoulder** | `E` Key | `BIND_BUTTON_R` | Grenade / Special |
| **Plus (+)** | `Enter` / `Return` | `BIND_BUTTON_PLUS` | Start / Pause Menu |
| **Minus (-)** | `Tab` Key | `BIND_BUTTON_MINUS` | Map / Scoreboard |
| **Home** | `Esc` Key | `BIND_BUTTON_HOME` | Return to Switch OS Home |
| **Capture** | `F12` Key | `BIND_BUTTON_CAPTURE` | Screenshot / Video Clip |
| **Left Stick Click (L3)** | `Left Ctrl` Key | `BIND_BUTTON_LSTICK` | Sprint / Crouch |
| **Right Stick Click (R3)**| `C` Key / `Middle Mouse` | `BIND_BUTTON_RSTICK` | Melee / Reset Gyro View |
| **D-Pad Up** | `Arrow Up` Key | `BIND_DPAD_UP` | Menu Navigation / Emote |
| **D-Pad Down** | `Arrow Down` Key | `BIND_DPAD_DOWN` | Menu Navigation / Emote |
| **D-Pad Left** | `Arrow Left` Key | `BIND_DPAD_LEFT` | Menu Navigation / Emote |
| **D-Pad Right** | `Arrow Right` Key | `BIND_DPAD_RIGHT` | Menu Navigation / Emote |
| **Left Stick Analog UP** | `W` Key | `BIND_MOVE_UP` | Forward Movement |
| **Left Stick Analog DOWN**| `S` Key | `BIND_MOVE_DOWN` | Backward Movement |
| **Left Stick Analog LEFT**| `A` Key | `BIND_MOVE_LEFT` | Strafe Left |
| **Left Stick Analog RIGHT**| `D` Key | `BIND_MOVE_RIGHT` | Strafe Right |

---

## 🖱️ Mouse & Gyro Tuning Parameters

The gyroscope engine relies on the mathematical conversion principles established in **`mizuyoukanao/nxic-pico`**:

```c
// ============================================================================
// MOUSE & GYRO CONFIGURATION
// ============================================================================
#define MOUSE_SENSITIVITY_X       1.50f   // Horizontal Gyro Yaw Multiplier
#define MOUSE_SENSITIVITY_Y       1.50f   // Vertical Gyro Pitch Multiplier
#define GYRO_SCALE_FACTOR         16.6f   // Switch IMU raw units (~16.6 counts/deg/sec)
#define GYRO_INVERT_Y             false   // Invert vertical pitch axis
#define GYRO_DEADZONE_PIXELS      0.0f    // Minimum mouse delta before reporting gyro
#define GYRO_SAMPLE_DECAY         0.85f   // 3-sample rolling distribution smoothing
```

### Tuning Recommendations by Game

1. **Splatoon 3**:
   * Set your mouse hardware DPI to **800–1600 DPI**.
   * In-game Motion Sensitivity: `+3.0` to `+5.0`.
   * Set `MOUSE_SENSITIVITY_X` to `1.20f`, `MOUSE_SENSITIVITY_Y` to `1.20f`.
   * Bind `BIND_BUTTON_RSTICK` to Middle Click or `C` key for immediate camera reset (`Y` button reset in Splatoon).

2. **Fortnite / Apex Legends / Overwatch 2**:
   * Set mouse to **1000Hz or 500Hz polling rate**.
   * In-game settings: Enable **Gyro Aiming**, set Motion Aiming Style to *Yaw (World Roll)*.
   * `MOUSE_SENSITIVITY_X`: `1.50f` – `2.00f`.

3. **Analog Stick Emulation Mode**:
   If a game does not support native gyro motion controls, set:
   ```c
   #define ENABLE_MOUSE_RIGHT_STICK_FALLBACK   true
   ```
   This routes mouse $\Delta X$ and $\Delta Y$ into the right analog stick 12-bit coordinate space ($0$ to $4095$, center at $2048$) with configurable curve response and deadzone compensation.

---

## 📌 Pin Configuration

If you prefer using different GPIO pins for the PIO USB Host port, simply change:

```c
#define PIN_PIO_USB_HOST_DP    2  // Default GP2 (D+)
#define PIN_PIO_USB_HOST_DM    3  // Default GP3 (D-)
```

*Note: For optimal PIO instruction efficiency, `PIN_PIO_USB_HOST_DM` should ideally be `PIN_PIO_USB_HOST_DP + 1`.*
