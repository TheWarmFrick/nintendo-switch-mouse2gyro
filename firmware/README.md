# SwitchKM-Pico

**RP2040 Keyboard & Mouse to Nintendo Switch Adapter with PIO-USB Host & 6-Axis Gyro Emulation**

A high-performance, open-source embedded firmware designed for the **Raspberry Pi Pico (RP2040)**. It enables you to connect standard USB keyboards and mice (via a standard USB hub) and play games on the **Nintendo Switch** with real-time **6-axis Gyroscope / IMU motion controls** rather than sluggish analog stick simulation.

---

## 🌟 Key Architecture & Highlights

* **Official Switch Pro Controller Emulation**: Connects to the Nintendo Switch via the Pico's native micro-USB / USB-C port, fully implementing the Switch USB HID protocol (0x30 full reports with subcommands, SPI flash calibration, and handshake).
* **Dual USB Stacks on Dual Cores**:
  * **Core 0**: Runs the TinyUSB Device Stack (`tud_task()`), managing Switch handshakes, button states, and continuous 15ms 6-axis IMU packet streaming.
  * **Core 1**: Dedicated to `pico-pio-usb` Host Stack (`tuh_task()`), polling an external USB hub wired to **GP2 (D+)** and **GP3 (D-)** for simultaneous keyboard and mouse HID reports.
* **Precise Mouse-to-Gyro Math (nxic-pico Engine)**:
  * Mouse relative deltas ($\Delta X, \Delta Y$) are directly converted to 16-bit signed angular velocity rates (Yaw and Pitch) within the 3-sample 6-axis IMU frame.
  * Natural gyro aiming in competitive titles (*Splatoon 3*, *Fortnite*, *Apex Legends*, *Overwatch 2*, *DOOM*, *The Legend of Zelda: Tears of the Kingdom*).
* **Zero Input Delay**: Sub-millisecond scan rate with independent core processing ensuring your mouse tracking is never blocked by USB device packet transmission.
* **Modular Configuration (`config.h`)**: Simple, isolated header file for customizing pin assignments, sensitivity multipliers, deadzones, and keybindings without touching low-level protocol drivers.

---

## 📚 Reference Repositories

This project synthesizes and extends two foundational open-source RP2040 projects:

1. **[`mizuyoukanao/nxic-pico`](https://github.com/mizuyoukanao/nxic-pico)**:
   * Definitive reference for mouse-to-gyro mathematical conversion, 6-axis IMU payload formatting, SPI flash calibration emulation, and angular velocity scaling factors.
2. **[`Tejasarus/SwitchKMAdapter`](https://github.com/Tejasarus/SwitchKMAdapter)**:
   * Architecture for TinyUSB Host HID device enumeration, keyboard scan-code translation, and multicore synchronization.

---

## 🔌 Hardware Setup at a Glance

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

See [HARDWARE.md](HARDWARE.md) for step-by-step soldering schematics and pull-down resistor guidelines.

---

## 🚀 Quick Start

1. **Build or Download Firmware**:
   * Compile the project following [BUILD.md](BUILD.md) to generate `switch_km_pico.uf2`.
2. **Flash the Pico**:
   * Hold the **BOOTSEL** button on your Raspberry Pi Pico while plugging it into your computer.
   * Drag and drop `switch_km_pico.uf2` into the `RPI-RP2` mass storage drive.
3. **Switch Settings**:
   * On your Nintendo Switch, navigate to:  
     `System Settings` → `Controllers and Sensors` → **`Pro Controller Wired Communication`** → Turn **ON**.
4. **Plug & Play**:
   * Plug your keyboard and mouse into the USB hub attached to GP2/GP3.
   * Connect the Pico's native USB port to the Switch dock or Switch console (via USB-C OTG adapter).
   * The controller will pair automatically as a Nintendo Switch Pro Controller with active gyro!

---

## 📁 Repository Directory Structure

```
.
├── CMakeLists.txt              # Top-level build configuration (Pico SDK + PIO-USB)
├── config.h                    # User configuration (pins, bindings, sensitivity)
├── switch_descriptors.h        # Switch USB descriptors & protocol definitions
├── switch_descriptors.c        # USB Device descriptors & TinyUSB device callbacks
├── switch_reports.h            # Switch 0x30 report & IMU packet data structures
├── switch_reports.c            # Subcommand handling & Switch packet assembly
├── nxic_gyro.h                 # Mouse-to-gyro mathematical conversion header
├── nxic_gyro.c                 # Mouse delta to 6-axis IMU angular velocity math
├── tusb_config.h               # TinyUSB dual Host + Device configuration
├── pio_usb_configuration.h     # pico-pio-usb pin and timing configuration
├── main.c                      # Multicore runtime entry & main task loops
├── README.md                   # Project overview & quick start
├── HARDWARE.md                 # Detailed soldering & wiring schematic
├── CONFIG.md                   # Customization & keybinding reference
└── BUILD.md                    # Compilation & flashing instructions
```

---

## 📜 License & Credits

* Special thanks to **mizuyoukanao** for `nxic-pico` and **Tejasarus** for `SwitchKMAdapter`.
* Uses **Raspberry Pi Pico SDK**, **TinyUSB** by Ha Thach, and **pico-pio-usb** by sekigon-gonnoc.
* Distributed under the MIT License.
