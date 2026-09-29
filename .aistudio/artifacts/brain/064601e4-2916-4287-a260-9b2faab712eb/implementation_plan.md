# Pure C/C++ Firmware & GitHub Actions Automated Build Pipeline

A clean, production-ready Raspberry Pi Pico (RP2040) firmware repository for Nintendo Switch keyboard and mouse control with 6-axis gyro motion, featuring fully automated cloud compilation via GitHub Actions.

---

### User Review & Critical Decisions

> [!IMPORTANT]
> **Approved Direction**: All web and desktop application layers are stripped away. 
> 
> The project will be a lean, standalone **C/C++ firmware repository** configured with **GitHub Actions CI/CD**. Users can fork the repository, customize `config.h` directly in their browser (or locally), and automatically receive a freshly compiled `switch_km_pico.uf2` binary from GitHub Actions with zero local toolchain setup.

- **Zero Software Bloat**: No desktop apps, no web servers, and no binary patching.
- **Automated Cloud Compilation**: A `.github/workflows/build.yml` workflow triggers on every push, pull request, manual workflow run, or release tag to compile the authentic `.uf2` binary.
- **Seamless User Journey**: Anyone can fork the repo, press `.` to edit `config.h` in GitHub's web editor, commit, and download the resulting `switch_km_pico.uf2` artifact in ~45 seconds.

---

### 1. Overview & Repository Architecture

- **What It Delivers**:
  1. **Complete Embedded C/C++ Firmware**:
     - Dual-core RP2040 execution: Core 0 handles the Nintendo Switch USB device link (HORI / Pro Controller 0x30 reports), while Core 1 runs `pico-pio-usb` software host mode on **GP2 (D+)** and **GP3 (D-)** to read standard USB keyboards and mice via a USB hub.
     - Mathematical mouse-to-gyro conversion strictly per [`mizuyoukanao/nxic-pico`](https://github.com/mizuyoukanao/nxic-pico), translating mouse relative deltas ($\Delta X, \Delta Y$) into 16-bit signed angular velocity rates (Yaw and Pitch) within a rolling 3-sample (5ms) IMU packet.
     - HID keyboard scancode mapping and multicore synchronization per [`Tejasarus/SwitchKMAdapter`](https://github.com/Tejasarus/SwitchKMAdapter).
  2. **Automated GitHub Actions CI/CD Workflow (`.github/workflows/build.yml`)**:
     - Automatically fetches the Pico SDK (v1.5.1+) and `pico-pio-usb` submodule.
     - Installs `arm-none-eabi-gcc` and `ninja-build`.
     - Compiles the source into `switch_km_pico.uf2`, `switch_km_pico.elf`, and `switch_km_pico.bin`.
     - Uploads the `.uf2` binary as a downloadable workflow artifact.
     - Automatically publishes the `.uf2` file to GitHub Releases when a version tag (e.g., `v1.0.0`) is pushed.
  3. **Comprehensive Markdown Documentation**:
     - `README.md`: Architecture overview, wiring ASCII diagram, and GitHub Fork-and-Build instructions.
     - `HARDWARE.md`: Physical wiring schematic for GP2/GP3, 15kΩ pull-down resistors, and passive vs. powered hub guide.
     - `CONFIG.md`: Keybinding reference tables and game tuning presets (*Splatoon 3*, *Fortnite/Apex*, *Zelda*).
     - `BUILD.md`: Instructions for both GitHub Actions cloud builds and local terminal builds.

---

### 2. User Workflow (The GitHub-Native Journey)

```
┌────────────────────────────────────────────────────────────────────────┐
│               The Zero-Install GitHub User Workflow                    │
└────────────────────────────────────────────────────────────────────────┘

  1. Fork Repository
     User navigates to the repository on GitHub and clicks "Fork".
                           │
                           ▼
  2. Edit in Browser (GitHub Web Editor)
     User presses the "." key on their keyboard inside the repo to open 
     the online editor, then tweaks keybindings or mouse sensitivity in config.h.
                           │
                           ▼
  3. Commit Changes
     User commits changes directly to the main branch.
                           │
                           ▼
  4. Automated Cloud Build (GitHub Actions)
     GitHub Actions runner (Ubuntu 22.04) spins up:
     • Clones Pico SDK & pico-pio-usb
     • Installs ARM GNU Embedded Toolchain
     • Runs: cmake -GNinja -DPICO_SDK_PATH=... && ninja
     • Packages switch_km_pico.uf2
                           │
                           ▼
  5. Download & Flash
     User clicks the completed workflow run under the "Actions" tab, 
     downloads switch_km_pico.uf2, and drags it onto the Raspberry Pi 
     Pico in BOOTSEL mode.
```

---

### 3. Implementation Steps & Deliverables

#### Step 1: GitHub Actions CI/CD Workflow (`.github/workflows/build.yml`)
- Trigger events: `push`, `pull_request`, and `workflow_dispatch` (allows triggering a build manually from the GitHub UI with one click).
- Environment: `ubuntu-latest`.
- Steps:
  1. `actions/checkout@v4` with `submodules: recursive`.
  2. Setup ARM GCC toolchain via `pkg-config` or standard `apt-get install gcc-arm-none-eabi libnewlib-arm-none-eabi`.
  3. Cache Pico SDK repository to keep build times under 45 seconds.
  4. Execute CMake configure and Ninja build.
  5. Use `actions/upload-artifact@v4` to store `switch_km_pico.uf2`.
  6. Optional release step (`softprops/action-gh-release@v1`) to automatically create releases on tags.

#### Step 2: Solidify Core C/C++ Firmware Files
- `CMakeLists.txt`: Configured for Pico SDK, multicore, TinyUSB dual device/host, and `pico-pio-usb`.
- `config.h`: Heavily commented standalone header with isolated sections:
  - Hardware pins (`PIN_PIO_USB_HOST_DP 2`, `PIN_PIO_USB_HOST_DM 3`).
  - Keybindings for A, B, X, Y, ZL, ZR, Bumpers, D-Pad, Sticks, and WASD.
  - Floating-point sensitivity multipliers: `MOUSE_SENSITIVITY_X`, `MOUSE_SENSITIVITY_Y`, `GYRO_SCALE_FACTOR` (16.6 counts/deg/sec).
- `switch_descriptors.h` & `switch_descriptors.c`: Official Switch Pro Controller USB VID/PID (`0x057E:0x2009`), endpoints, and HID descriptors.
- `switch_reports.h` & `switch_reports.c`: Subcommand parser (handshake `0x80`, SPI flash sensor calibration mock `0x10`, report mode `0x03`, IMU enable `0x40`) and standard 49-byte `0x30` packet builder.
- `nxic_gyro.h` & `nxic_gyro.c`: Exact mathematical model from `mizuyoukanao/nxic-pico` translating mouse deltas into 16-bit signed angular velocity rates (Yaw & Pitch) across a rolling 3-sample (5ms) window.
- `tusb_config.h` & `pio_usb_configuration.h`: Dual-port TinyUSB configuration (RHPort 0 Device, RHPort 1 Host).
- `main.c`: Multicore task loops (`tud_task` on Core 0, `tuh_task` on Core 1).

#### Step 3: Complete Documentation Set
- `README.md`: Includes clear badges for the GitHub Actions build status, quick-start guide, and ASCII wiring layout.
- `HARDWARE.md`: Soldering pinout, 15kΩ pull-downs, and USB hub power considerations.
- `CONFIG.md`: Keymapping reference table and game sensitivity tuning recommendations.
- `BUILD.md`: Full instructions for both GitHub Actions automated builds and manual local toolchain builds.
- `.gitignore`: Ignoring CMake build artifacts (`build/`, `*.uf2`, `*.elf`, `*.bin`, `*.map`).

---

### 4. Technical Architecture Diagram

```
┌────────────────────────────────────────────────────────────────────────┐
│               Raspberry Pi Pico (RP2040 Dual-Core)                     │
├───────────────────────────────────┬────────────────────────────────────┤
│   CORE 0: Switch Controller Link  │    CORE 1: PIO USB Host (KBM Hub)  │
│  - Native USB Port (RHPort 0)     │  - Software Host on GP2/GP3        │
│  - TinyUSB Device Stack           │  - pico-pio-usb & TinyUSB Host     │
│  - Handshake (0x80) & Subcommands │  - Keyboard Scancode Decoder       │
│  - 15ms Full Report 0x30 Stream   │  - Mouse Delta (dX, dY) Producer   │
└─────────────────▲─────────────────┴─────────────────┬──────────────────┘
                  │                                   │
                  │ Shared Thread-Safe Exchange       │
                  └───────────────────────────────────┘
                                    ▲
                                    │
┌───────────────────────────────────┴────────────────────────────────────┐
│                    nxic-pico Gyro Mathematical Engine                  │
│   • Yaw Angular Velocity   = ΔX × SENSITIVITY_X × SCALE_FACTOR (16.6)  │
│   • Pitch Angular Velocity = -ΔY × SENSITIVITY_Y × SCALE_FACTOR (16.6) │
│   • Rolling 3-Sample IMU Window (5ms intervals with decay)             │
│   • Accelerometer Gravity Vector: 1.0G (Az = 4096 LSB)                 │
└────────────────────────────────────────────────────────────────────────┘
```
