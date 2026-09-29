# Hardware Wiring & Assembly Guide

This document details how to wire a standard **Female USB-A Breakout Board** (or stripped USB extension cable) to the Raspberry Pi Pico to create the PIO USB Host port for your keyboard and mouse.

---

## 🛠️ Required Components

1. **Raspberry Pi Pico (RP2040)** or Raspberry Pi Pico W / Pico 2.
2. **Female USB-A Breakout Board** (or a sliced USB female cable).
3. **USB 2.0 Hub** (A compact 2-port or 4-port hub to connect both Keyboard and Mouse).
4. **2x 15kΩ Resistors** (Optional but recommended: pull-down resistors for USB Host compliance on D+ and D-).
5. **Solid-core jumper wires** (22-26 AWG) and soldering kit.
6. **Micro-USB to USB-A Cable** (to connect Pico native port to Nintendo Switch Dock).

---

## 📐 Pin Connection Diagram

The PIO USB Host stack uses **PIO0** to emulate a USB 1.1 Full-Speed (12Mbps) / Low-Speed (1.5Mbps) host controller on any adjacent GPIO pin pair. By default, this firmware uses **GP2** and **GP3**.

| Female USB-A Pin | Wire Color | Raspberry Pi Pico Pin | Pico Physical Pin # | Description |
|:---|:---|:---|:---|:---|
| **VBUS (5V)** | Red | **VBUS** | **Pin 40** | Direct 5V power from Switch native USB |
| **D- (Data Minus)** | White | **GP3** | **Pin 5** | PIO USB Host D- signal line |
| **D+ (Data Plus)** | Green | **GP2** | **Pin 4** | PIO USB Host D+ signal line |
| **GND (Ground)** | Black | **GND** | **Pin 38 (or 3)** | System Common Ground |

```
                       Raspberry Pi Pico (Top View)
                           +------------------+
              [Micro-USB]  |     [ USB ]      |
              (To Switch)  |                  |
                    GP0 ---| 1             40 |--- VBUS (5V to USB-A VBUS Red)
                    GP1 ---| 2             39 |--- VSYS
                    GND ---| 3             38 |--- GND (Ground to USB-A GND Black)
 (D+ Green)  -----> GP2 ---| 4             37 |--- 3V3_EN
 (D- White)  -----> GP3 ---| 5             36 |--- 3V3(OUT)
                    GP4 ---| 6             35 |--- ADC_VREF
                    GP5 ---| 7             34 |--- GP28
                    GND ---| 8             33 |--- GND
                           |       ...        |
                           +------------------+

                              WIRING TO FEMALE USB PORT:
      Pico VBUS (Pin 40) ==========================> USB-A Pin 1 (VBUS, Red)
      Pico GP2  (Pin 4)  ==========================> USB-A Pin 3 (D+, Green)
      Pico GP3  (Pin 5)  ==========================> USB-A Pin 2 (D-, White)
      Pico GND  (Pin 38) ==========================> USB-A Pin 4 (GND, Black)

      Optional Pull-Down Resistors (Host Standard):
      GP2 (D+)  ---[ 15kΩ ]---> GND
      GP3 (D-)  ---[ 15kΩ ]---> GND
```

---

## ⚡ Pull-Down Resistors & Signal Integrity

### Why Pull-Downs Matter
According to the USB 2.0 / 1.1 Specification, standard USB Host ports require **15 kΩ pull-down resistors to Ground** on both `D+` and `D-` lines. When a downstream device (or hub) connects:
* A Full-Speed device (or hub) pulls `D+` high via its internal 1.5 kΩ pull-up resistor.
* A Low-Speed device pulls `D-` high via its internal 1.5 kΩ pull-up resistor.

### RP2040 Internal Pull-Downs vs. External Resistors
The RP2040 chip includes internal ~50kΩ pull-down resistors on its GPIO pins, and `pico-pio-usb` attempts to enable them in software. However:
* Internal 50kΩ pull-downs are weaker than the 15kΩ standard.
* **Best Practice**: Solder two **15kΩ 1/8W resistors** directly from GP2 to GND and from GP3 to GND on your breakout board. This ensures 100% reliable hotplug detection and prevents dropped USB packets with picky hubs.

---

## 🔌 Passive vs. Powered USB Hubs

### Passive Hubs
* **When to use**: Standard basic membrane/mechanical keyboards and optical gaming mice without aggressive RGB lighting.
* **Power Budget**: The Nintendo Switch dock supplies up to 500mA over its USB port. The Pico consumes ~50–80mA, leaving **~420mA** for the keyboard and mouse combined.
* **Recommendation**: If your keyboard does not have an LCD screen or ultra-bright RGB LED arrays, a passive 4-port USB 2.0 hub works flawlessly.

### Externally Powered Hubs
* **When to use**:
  * High-power RGB gaming keyboards (e.g. Corsair K-series, Razer Huntsman, Wooting).
  * High polling-rate mice (e.g. Razer Viper 8K, Logitech G Pro).
  * Setups where the keyboard experiences brownouts or random disconnects.
* **How it works**: Connect a powered USB hub to GP2/GP3. The hub injects its own 5V power supply to the peripherals while maintaining shared data lines and common ground with the Pico.

---

## ⚠️ Important Precautions

1. **Keep Wires Short**: High-speed digital signals on PIO are sensitive to stray capacitance. Keep the wires between the Pico GPIO pins (GP2, GP3) and the USB breakout board under **10 cm (4 inches)**.
2. **Never Swap D+ and D-**: Connecting D+ to GP3 and D- to GP2 will prevent the USB host from recognizing downstream devices. If the Pico does not detect your keyboard, double-check that **GP2 is D+** and **GP3 is D-** (or update `PIN_PIO_USB_HOST_DP` in `config.h`).
3. **Common Ground**: Ensure that the USB-A Ground (Pin 4) is securely connected to one of the Pico's GND pins (Pins 3, 8, 13, 18, 23, 28, 33, or 38).
