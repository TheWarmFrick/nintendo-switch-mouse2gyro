# Build & Flashing Guide

There are two ways to compile and flash **SwitchKM-Pico**:
1. **GitHub Actions (Recommended - Zero Setup)**: Cloud build that produces ready-to-flash `.uf2` binaries automatically on every push or release tag.
2. **Local CMake Build**: Compile directly on your local machine using the Raspberry Pi Pico SDK.

---

## ☁️ Method 1: Automated GitHub Actions Build (Recommended)

You do **not** need to install any compilers or dependencies on your machine.

### Step 1: Fork the Repository
Click the **Fork** button at the top right of this repository to create your own copy on GitHub.

### Step 2: Customize Keybindings & Sensitivity in Browser
1. In your forked repository, press the **`.`** (period) key on your keyboard.
2. GitHub will launch an online Visual Studio Code editor directly in your web browser.
3. Open **`config.h`**.
4. Customize your keybindings (e.g., `BIND_TRIGGER_ZR`, `BIND_BUTTON_A`) and mouse sensitivity multipliers (`MOUSE_SENSITIVITY_X`, `MOUSE_SENSITIVITY_Y`).
5. Open the Source Control tab on the left (or press `Ctrl+Shift+G`), type a commit message, and click **Commit & Push**.

### Step 3: Download Your Compiled `.uf2`
1. Navigate to the **Actions** tab in your repository.
2. You will see a running workflow named **"Build SwitchKM-Pico Firmware"**.
3. Once completed (~45 seconds), click on the workflow run.
4. Scroll down to the **Artifacts** section at the bottom.
5. Click **`switch_km_pico.uf2`** to download your customized binary!

### Step 4: Flash the Pico
1. Press and hold down the white **BOOTSEL** button on your Raspberry Pi Pico.
2. While holding the button, plug the Pico's native USB port into your computer.
3. Release the button. A removable drive named **`RPI-RP2`** will appear on your computer.
4. Drag and drop `switch_km_pico.uf2` into the `RPI-RP2` drive.
5. The drive will automatically unmount, and the Pico will reboot running your custom firmware!

---

## 💻 Method 2: Local Terminal Build

If you are developing locally or prefer working offline:

### Prerequisites
1. **ARM GNU Embedded Toolchain** (`arm-none-eabi-gcc`)
2. **CMake** (version 3.13 or newer)
3. **Ninja** or GNU **Make**
4. **Git** and **Python 3**

#### Linux (Debian / Ubuntu / Raspberry Pi OS)
```bash
sudo apt update
sudo apt install -y cmake ninja-build gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential python3
```

#### macOS (Homebrew)
```bash
brew install cmake ninja
brew tap arm-none-eabi-gcc/arm-none-eabi-gcc
brew install arm-none-eabi-gcc
```

#### Windows (WSL2)
Use an Ubuntu terminal under WSL2 and follow the Linux instructions above.

---

### Cloning & Compilation

```bash
# Clone the repository with submodules
git clone https://github.com/your-username/switch-km-pico.git
cd switch-km-pico
git submodule update --init --recursive

# Clone the Pico SDK if not already installed locally
git clone --depth 1 --branch 1.5.1 https://github.com/raspberrypi/pico-sdk.git ../pico-sdk
cd ../pico-sdk && git submodule update --init && cd ../switch-km-pico
export PICO_SDK_PATH="$(pwd)/../pico-sdk"

# Configure and compile with Ninja
cmake -B build -GNinja -DCMAKE_BUILD_TYPE=Release
ninja -C build
```

Upon completion, `build/switch_km_pico.uf2` will be ready for flashing in BOOTSEL mode.
