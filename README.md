# SiWx917 HC-SR04 Ultrasonic Distance Measurement

Measures object distance using an **HC-SR04 ultrasonic sensor** on the **SiWx917 Dev Kit (BRD2605A)**. An Arduino generates the trigger pulse while the SiWx917 reads the echo signal and outputs distance readings over serial. Uses `sl_si91x_driver_gpio` and `sl_sleeptimer` SDK components via Simplicity Studio 5.

---

## Hardware

| Component | Details |
|-----------|---------|
| Dev Kit | SiWx917 (BRD2605A) |
| Sensor | HC-SR04 Ultrasonic Sensor |
| Trigger Source | Arduino (any model) |
| Other | 1kΩ + 2kΩ resistors (voltage divider), breadboard, connecting wires |

**Pin Connections:**

| HC-SR04 Pin | Connection |
|-------------|-----------|
| VCC | 5V (Arduino) |
| GND | Common GND (Arduino + SiWx917) |
| TRIG | Arduino Pin 9 |
| ECHO | ULP_GPIO_7 on SiWx917 (**via voltage divider**) |

> ⚠️ **Voltage Divider Required:** HC-SR04 Echo outputs 5V but SiWx917 GPIO is 3.3V tolerant. Use R1 = 1kΩ and R2 = 2kΩ to step down safely.

---

## Software / Tooling

- **IDE:** Simplicity Studio 5 (SSv5) for SiWx917 firmware
- **IDE:** Arduino IDE for trigger sketch
- **SDK:** WiSeConnect 3 SDK (Gecko SDK with SI91x support)

**Required SSv5 Components (install via Component Editor):**

| Component | Purpose |
|-----------|---------|
| `sl_si91x_driver_gpio` | Echo pin GPIO input |
| `sl_sleeptimer` | Echo pulse timing and periodic measurement |
| IO Stream (VCOM) / `iostream_vuart_si91x` | Enables `printf()` serial output |
| SI91X IO Stream Log | Redirects printf to serial console |
| Board Config (`sl_gpio_board.h`) | GPIO pin mapping for BRD2605A |

---

## Project File Structure

```
siwx917-hcsr04-ultrasonic/
├── app.c           # Main app — periodic measurement loop
├── hcsr04.c        # HC-SR04 driver — echo timing and distance calculation
├── hcsr04.h        # Driver header
├── arduino_trigger.ino  # Arduino sketch — generates TRIG pulse
└── README.md
```

---

## Build & Flash Instructions

**Arduino (Trigger):**
1. Open `arduino_trigger.ino` in Arduino IDE
2. Upload to Arduino — it continuously generates 10µs trigger pulses on Pin 9

**SiWx917 (Echo + Distance):**
1. Open Simplicity Studio 5 and create a new empty SI91x project targeting **BRD2605A**
2. Install required components via Component Editor (see table above)
3. Add `hcsr04.c` and `hcsr04.h` to the project, replace `app.c` with the provided file
4. Build the project (**Project → Build**)
5. Flash to the board (**Run → Debug** or flash programmer)
6. Open **Serial Console** in SSv5 and observe distance readings

---

## Expected Output

```
Distance: 12 cm
Distance: 13 cm
Distance: 12 cm
```

Returns `-1` with a timeout message if no object is detected within range.

---

## Precautions

- Always use the voltage divider on the Echo pin — direct 5V will damage the SiWx917 GPIO
- Ensure common ground between Arduino and SiWx917
- Keep the sensor stable and properly aligned

---

## License

MIT License — see [LICENSE](./LICENSE)
