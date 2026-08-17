# Bill of Materials (BOM)

Complete parts list for one working unit. Prices are **indicative Philippine pesos (PHP)**, 2025 street prices from local electronics shops (e-Gizmo, Robu, Alexan) and online (Shopee/Lazada). Actual prices vary by seller and batch. **Total ≈ ₱2,300 – ₱3,500.**

## Components

| # | Qty | Item | Specification | Role | Est. each | Est. total |
|---|---|---|---|---|---|---|
| 1 | 1 | Arduino Mega 2560 | ATmega2560 clone board (USB + barrel jack) | Main controller | ₱450 | ₱450 |
| 2 | 1 | DS3231 RTC module | I2C, ±2 ppm, CR2032 holder | Accurate timekeeping with backup | ₱120 | ₱120 |
| 3 | 1 | CR2032 battery | 3 V lithium coin cell | RTC backup power | ₱40 | ₱40 |
| 4 | 1 | 16×2 LCD + I2C backpack | PCF8574 backpack, 5 V | Status and reminder display | ₱180 | ₱180 |
| 5 | 2 | SG90 micro servo | 4.8–6 V, ~1.8 kg·cm, 9 g | Dispenser rotation + chute agitator | ₱100 | ₱200 |
| 6 | 1 | HX711 module | 24-bit ADC, gain 128 | Load-cell amplifier | ₱90 | ₱90 |
| 7 | 1 | Load cell | 1 kg straight-bar (or 5 kg) | Measures pill presence/removal | ₱130 | ₱130 |
| 8 | 1 | Active buzzer | 5 V, digital | Audible reminder | ₱30 | ₱30 |
| 9 | 6 | LEDs | 5 mm red ×3, green ×3 | Reminder/status indicators | ₱10 | ₱60 |
| 10 | 8 | Resistors | 220 Ω (LEDs), 10 kΩ (buttons) | Current limiting / pull-downs | ₱2 | ₱16 |
| 11 | 2 | Capacitors | 100 nF + 470 µF | Decoupling for servo/HX711 power | ₱10 | ₱20 |
| 12 | 1 | UVC lamp | 254 nm tube + ballast, **or** 260–280 nm UVC LED module | Sterilization of cup/dispenser | ₱350 | ₱350 |
| 13 | 1 | Relay module | 5 V, 1-channel, 10 A contacts | Switches the UVC lamp | ₱60 | ₱60 |
| 14 | 2 | Push buttons | Momentary, tactile | Snooze + manual dispense | ₱15 | ₱30 |
| 15 | 1 | Power adapter | 9–12 V DC, ≥ 1 A, barrel plug | Powers the Mega (VIN) | ₱200 | ₱200 |
| 16 | 1 | Power adapter | 5 V DC, ≥ 2 A (phone charger OK) | Dedicated supply for servos + relay | ₱150 | ₱150 |
| 17 | 1 | Breadboard + jumper wires | 830-point + M/M, M/F sets | Prototype wiring | ₱120 | ₱120 |
| 18 | 1 | Enclosure | Acrylic sheet / 3D print / PVC | Houses mechanics + electronics | ₱400 | ₱400 |
| 19 | 1 | UVC safety glass / shield | UV-blocking acrylic or tinted window | Lets user see the cup without UV exposure | ₱150 | ₱150 |
| 20 | 1 | Fasteners & misc | Screws, standoffs, hot glue, tape | Mounting servos, cell, lamp | ₱100 | ₱100 |
| — | — | **Total** | — | — | — | **≈ ₱2,896** |

> Range reflects seller/brand differences (e.g., original vs. clone Mega, 3D-printed vs. acrylic enclosure). Add **₱150–₱250 per servo** if upgrading to MG995/MG996R.

## Consumables & tools (not counted in the unit cost)

| Item | Purpose |
|---|---|
| Soldering iron + solder | Relay/power joints |
| Multimeter | Continuity, voltage checks |
| Heat-shrink / electrical tape | Insulating splices |
| Calibration weights | Load-cell calibration and verification |
| Dupont connector housings | Removable wiring harness |

## Selection notes

- **Servo power is the #1 trap.** SG90 stall current can reach ~700 mA each. Two servos **must** run from the separate 5 V/2 A supply, sharing GND with the Mega. Running them off the Mega's 5 V regulator resets the board mid-dispense.
- **Load cell capacity** must exceed the tare of the cup + mechanism plus the pill weight. A 1 kg cell is right for a single-pill cup; choose 5 kg only if the mechanism is heavy.
- **UVC**: prefer a lamp module with a documented power (W) so the UV dose can be computed; add the UV-blocking window so the user can observe the cycle safely.
- **RTC**: buy a module that includes the DS3231 (not the older DS1307) — the ±2 ppm accuracy is what makes daily scheduling reliable.
