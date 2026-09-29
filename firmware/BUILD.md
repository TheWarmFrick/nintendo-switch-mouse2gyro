# Build & Flashing Guide

You can build the firmware binary (`switch_km_pico.uf2`) in two ways:
1. **☁️ GitHub Actions (Recommended — Zero Local Installation)**: Build directly in the cloud via GitHub's **Actions** tab and download the compiled `.uf2` artifact.
2. **💻 Local Compilation**: Compile on your own machine using the official **Raspberry Pi Pico SDK** and CMake/Ninja.

---

## ☁️ Method 1: Cloud Build via GitHub Actions (No Toolchain Needed)

The repository includes a ready-to-run GitHub Actions workflow (`.github/workflows/build.yml`) that automatically sets up the ARM compiler, pulls all submodules, builds the project, and provides a downloadable `.uf2` binary.

### How to Trigger the Build in GitHub:
1. **Push to GitHub**: Push this repository (or your fork) to GitHub.
2. **Open the Actions Tab**: Click on the **Actions** tab in the top navigation bar of your GitHub repository.
3. **Select Workflow**: In the left sidebar under "Workflows", select **`Build Pico UF2 Firmware`**.
4. **Run Manually**:
   * Click the **`Run workflow`** dropdown on the right side.
   * Select your branch (e.g. `main` or `master`).
   * Click the green **`Run workflow`** button.
5. **Download Your `.uf2` Binary**:
   * Wait ~45–60 seconds for the workflow run to complete (green checkmark).
   * Click into the completed run.
   * Scroll down to the **Artifacts** section at the bottom.
   * Click **`switch-km-pico-uf2`** to download a zip archive containing `switch_km_pico.uf2`, `switch_km_pico.bin`, and `switch_km_pico.elf`!

> **Automated Tag Releases**: Pushing any tag starting with `v` (e.g. `git tag v1.0.0 && git push origin v1.0.0`) automatically triggers a full build and publishes a GitHub Release with `switch_km_pico.uf2` attached directly to the release page.

---

## 💻 Method 2: Local Compilation

If you prefer building locally on your machine:

### 1. Prerequisites
Ensure the following tools are installed on your build machine (Linux, macOS, or Windows via WSL2):
* **Raspberry Pi Pico SDK** (version 1.5.1 or newer)
* **ARM GNU Embedded Toolchain** (`arm-none-eabi-gcc`, `arm-none-eabi-g++`, `arm-none-eabi-newlib`)
* **CMake** (version 3.13 or newer)
* **Ninja** or GNU **Make**
* **Git** (for submodule management)
* **Python 3**

#### Linux (Debian / Ubuntu / Raspberry Pi OS)
```bash
sudo apt update
sudo apt install -y git cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential libstdc++-arm-none-eabi-newlib ninja-build
```

#### macOS (Homebrew)
```bash
brew install cmake ninja
brew tap arm-none-eabi-gcc/arm-none-eabi-gcc
brew install arm-none-eabi-gcc
```

#### Windows
We recommend building inside **WSL2 (Ubuntu)** or using the official Raspberry Pi Pico Windows Installer.

---

### 2. Cloning & Submodules
```bash
# Clone the repository
git clone https://github.com/your-username/switch-km-pico.git
cd switch-km-pico

# Fetch all submodules (including pico-pio-usb)
git submodule update --init --recursive
```

If you do not have the Raspberry Pi Pico SDK installed locally:
```bash
git clone https://github.com/raspberrypi/pico-sdk.git ../pico-sdk
cd ../pico-sdk
git submodule update --init
cd ../switch-km-pico
export PICO_SDK_PATH="$(pwd)/../pico-sdk"
```

---

### 3. Compiling the Binary
```bash
# Create build directory
mkdir build && cd build

# Configure CMake with Ninja
cmake -DPICO_SDK_PATH=$PICO_SDK_PATH -GNinja ..

# Build target
ninja
```

Upon successful compilation, `switch_km_pico.uf2` will be created in the `build/` directory.

---

## ⚡ Flashing the Raspberry Pi Pico

1. **Disconnect** the Pico from all USB cables.
2. Press and hold down the **white BOOTSEL button** on top of the Pico board.
3. While holding the button, connect the Pico's native USB port to your PC.
4. Release the **BOOTSEL** button.
5. Your OS will mount a removable USB drive named **`RPI-RP2`**.
6. Drag and drop `switch_km_pico.uf2` onto the **`RPI-RP2`** drive.
7. The drive will unmount automatically and the onboard green LED will blink twice. The firmware is now running!

---

## 🎮 Nintendo Switch Setup

1. On your Nintendo Switch console, go to:  
   **System Settings** → **Controllers and Sensors** → **`Pro Controller Wired Communication`** → Set to **ON**.
2. Plug your keyboard and mouse into the USB hub connected to GP2/GP3.
3. Connect the Pico native port to the Switch dock. Your keyboard and mouse are now active with 6-axis gyro motion!
