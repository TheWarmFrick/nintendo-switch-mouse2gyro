# This is entirely vibecoded And not even working . Look at these instead: https://github.com/Tejasarus/SwitchKMAdapter and https://github.com/mizuyoukanao/nxic-pico

[![Build SwitchKM-Pico Firmware](https://github.com/your-username/switch-km-pico/actions/workflows/build.yml/badge.svg)](https://github.com/your-username/switch-km-pico/actions/workflows/build.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Platform: RP2040](https://img.shields.io/badge/Platform-RP2040-red.svg)](https://www.raspberrypi.com/products/raspberry-pi-pico/)

**High-performance Raspberry Pi Pico (RP2040) firmware that connects USB Keyboards and Mice to the Nintendo Switch with real-time 6-axis Gyroscope / IMU motion controls.**

Built with **Pico SDK**, **TinyUSB**, and **`pico-pio-usb`**, and featuring **100% automated cloud builds via GitHub Actions**. You never need to install CMake, Python, or the ARM toolchain on your computer.

---

## ⚡ The Zero-Install Build & Flash Workflow

You can customize your keybindings and generate a freshly compiled `.uf2` binary in under 2 minutes directly on GitHub:

```
  1. FORK THIS REPO
     Click "Fork" at the top right of this GitHub page.
            │
            ▼
  2. EDIT IN YOUR BROWSER (Press ".")
     On your forked repository page, press the "." key on your keyboard.
     This opens GitHub's web editor right in your browser.
            │
            ▼
  3. CUSTOMIZE config.h
     Open config.h, change whatever button bindings or mouse sensitivity
     values you want, and click "Commit changes".
            │
            ▼
  4. DOWNLOAD YOUR FRESH .UF2
     Click the "Actions" tab at the top. The "Build SwitchKM-Pico Firmware"
     workflow will finish compiling in ~40 seconds.
     Click the run and download the "switch_km_pico.uf2" artifact!
            │
            ▼
  5. FLASH YOUR PICO
     Hold the BOOTSEL button on your Pico, plug it in via USB (mounts as RPI-RP2),
     and drag-and-drop switch_km_pico.uf2. You're done!
```

---

## 🌟 Key Features

* **Official Switch Pro Controller Emulation**: Connects to the Switch dock or console via the Pico's native USB port (VID: `0x057E`, PID: `0x2009`), implementing standard 49-byte `0x30` input reports and SPI sensor calibration handshakes.
* **Precise Mouse-to-Gyro Math ([`nxic-pico`](https://github.com/mizuyoukanao/nxic-pico))**:
  * Mouse relative deltas ($\Delta X, \Delta Y$) are mathematically mapped directly into 16-bit signed angular velocity values (Yaw and Pitch) within the 3-sample 6-axis IMU packet.
  * Experience native, fluid gyro aiming in *Splatoon 3*, *Fortnite*, *Apex Legends*, *Overwatch 2*, *DOOM*, and *Zelda: Tears of the Kingdom*.
* **Dual-Core USB Architecture**:
  * **Core 0**: TinyUSB Device Stack (`tud_task()`) streaming 15ms Switch motion reports.
  * **Core 1**: `pico-pio-usb` Host Stack (`tuh_task()`) on **GP2 / GP3** polling keyboard and mouse HID reports concurrently through an external USB hub.
* **Single Configuration Header (`config.h`)**: All pins, keycodes, and sensitivity floats are neatly isolated with clear comments for quick editing.

---

## 🔌 Hardware Setup & Wiring

```
       +---------------------------------------------+
       |             Raspberry Pi Pico               |
       |                                             |
       |  [Native USB] ---> USB-A Cable to Switch    |
       |                                             |
       |  GP2 (Pin 4)  ---> USB D+ (Green)           |
       |  GP3 (Pin 5)  ---> USB D- (White)           |
       |  VBUS (Pin 40)---> USB 5V (Red)             |
       |  GND  (Pin 38)---> USB GND (Black)          |
       +---------------------------------------------+
                             |
                   [Female USB-A Port]
                             |
                     [USB 2.0 Hub]
                      /         \
              [Keyboard]       [Mouse]
```

### Pin Solder Map
| Female USB-A Breakout | Wire Color | Pico Pin # | Pico Function |
|:---|:---|:---|:---|
| **VBUS (5V)** | Red | **Pin 40** | VBUS (5V power from Switch) |
| **D- (Data Minus)** | White | **Pin 5** | **GP3** (PIO USB Host D-) |
| **D+ (Data Positive)**| Green | **Pin 4** | **GP2** (PIO USB Host D+) |
| **GND (Ground)** | Black | **Pin 38** | System Common Ground |

*Optional but recommended: Add two **15kΩ pull-down resistors** from GP2 to GND and GP3 to GND for strict USB Host specification compliance.*

See [HARDWARE.md](HARDWARE.md) for full physical wiring diagrams, resistor networks, and powered hub notes.

---

## 🎮 Default Keybindings

| Switch Controller Input | Keyboard / Mouse Binding | `config.h` Constant |
|:---|:---|:---|
| **ZR Trigger (Primary Fire)** | `Spacebar` or `Left Mouse Click` | `BIND_TRIGGER_ZR` |
| **ZL Trigger (Aim Down Sights)** | `Left Shift` or `Right Mouse Click` | `BIND_TRIGGER_ZL` |
| **A Button (Confirm / Jump)** | `L` Key | `BIND_BUTTON_A` |
| **B Button (Cancel / Crouch)** | `K` Key | `BIND_BUTTON_B` |
| **X Button (Jump / Menu)** | `I` Key | `BIND_BUTTON_X` |
| **Y Button (Reload / Camera Reset)** | `J` Key | `BIND_BUTTON_Y` |
| **L Shoulder** | `Q` Key | `BIND_BUTTON_L` |
| **R Shoulder** | `E` Key | `BIND_BUTTON_R` |
| **Plus (+)** | `Enter` | `BIND_BUTTON_PLUS` |
| **Minus (-)** | `Tab` | `BIND_BUTTON_MINUS` |
| **Home** | `Escape` | `BIND_BUTTON_HOME` |
| **Capture** | `F12` | `BIND_BUTTON_CAPTURE` |
| **Left Stick Analog (Movement)** | `W, A, S, D` Keys | `BIND_MOVE_*` |
| **Right Stick Click (Gyro Reset)** | `C` Key or `Middle Mouse Click` | `BIND_BUTTON_RSTICK` |

See [CONFIG.md](CONFIG.md) for game-specific sensitivity tuning recommendations (*Splatoon 3*, *Apex*, *Zelda*).

---

## 🛠️ Local Compilation (Optional)

If you prefer building locally on your machine instead of using GitHub Actions:

```bash
# 1. Install toolchain (Ubuntu / Debian)
sudo apt update && sudo apt install -y cmake ninja-build gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential

# 2. Clone repository with submodules
git clone https://github.com/your-username/switch-km-pico.git
cd switch-km-pico
git submodule update --init --recursive

# 3. Build with CMake
cmake -B build -GNinja
ninja -C build

# Output binary: build/switch_km_pico.uf2
```

See [BUILD.md](BUILD.md) for macOS and Windows (WSL2) commands.

---

## 📚 Acknowledgements & References

* **[`mizuyoukanao/nxic-pico`](https://github.com/mizuyoukanao/nxic-pico)**: Definitive architecture for mouse-to-gyro mathematics, 6-axis report structures, and Switch IMU scaling.
* **[`Tejasarus/SwitchKMAdapter`](https://github.com/Tejasarus/SwitchKMAdapter)**: Architecture for TinyUSB Host HID processing, keyboard scancode mapping, and multicore synchronization.
* **Raspberry Pi Pico SDK & TinyUSB**: Foundation for high-performance RP2040 embedded systems.

---

## 📜 License

Distributed under the **MIT License**. Free for personal and commercial use.
