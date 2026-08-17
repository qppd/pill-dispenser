# Bill of Materials (BOM)

Complete parts list for one working unit. Prices are **indicative Philippine pesos (PHP)**, 2025 street prices from local electronics shops (e-Gizmo, Robu, Alexan) and online (Shopee/Lazada). Actual prices vary by seller and batch. **Total ≈ ₱2,400 – ₱3,600.**

## Components

| # | Qty | Item | Specification | Role | Est. each | Est. total |
|---|---|---|---|---|---|---|
| 1 | 1 | Arduino Mega 2560 | ATmega2560 clone board (USB + barrel jack) | Main controller | ₱450 | ₱450 |
| 2 | 1 | DS3231 RTC module | I2C, ±2 ppm, CR2032 holder | Accurate timekeeping with backup | ₱120 | ₱120 |
| 3 | 1 | CR2032 battery | 3 V lithium coin cell | RTC backup power | ₱40 | ₱40 |
| 4 | 1 | 16×2 LCD + I2C backpack | PCF8574 backpack, 5 V | Status and reminder display | ₱180 | ₱180 |
| 5 | 2 | MG90S micro servo (metal gear) | 4.8–6 V, ~2.2 kg·cm, 13 g | Dispenser rotation + chute agitator | ₱120 | ₱240 |
| 6 | 1 | HX711 module | 24-bit ADC, gain 128 | Load-cell amplifier | ₱90 | ₱90 |
| 7 | 1 | Load cell | 1 kg straight-bar (or 5 kg) | Measures pill presence/removal | ₱130 | ₱130 |
| 8 | 1 | Active buzzer | 5 V, digital | Audible reminder | ₱30 | ₱30 |
| 9 | 2 | LEDs | 5 mm red + green (buy spares) | Reminder/status indicators (D7 red, D8 green) | ₱10 | ₱20 |
| 10 | 4 | Resistors | 220 Ω (LEDs), 100 Ω (buzzer) | Current limiting (buttons use Mega's internal pull-ups) | ₱2 | ₱8 |
| 11 | 2 | Capacitors | 100 nF + 470 µF | Decoupling for servo/HX711 power | ₱10 | ₱20 |
| 12 | 1 | UVC lamp | 254 nm tube + ballast, **or** 260–280 nm UVC LED module | Sterilization of cup/dispenser | ₱350 | ₱350 |
| 13 | 1 | Relay module | 5 V, 1-channel, 10 A contacts | Switches the UVC lamp | ₱60 | ₱60 |
| 14 | 2 | Push buttons | Momentary, tactile | Snooze + manual dispense | ₱15 | ₱30 |
| 15 | 1 | Microswitch (interlock) | Momentary, normally-open | Cover-closed safety switch for the UVC lamp | ₱30 | ₱30 |
| 16 | 1 | Power adapter | 12 V DC, ≥ 2 A (3 A recommended) | Powers the Mega via VIN and feeds the LM2596S | ₱250 | ₱250 |
| 17 | 1 | LM2596S buck converter module | 12 V → 5 V, up to 3 A, 7-segment voltmeter | Steps 12 V down to 5 V for servos + relay | ₱120 | ₱120 |
| 18 | 1 | Breadboard + jumper wires | 830-point + M/M, M/F sets | Prototype wiring | ₱120 | ₱120 |
| 19 | 1 | Enclosure | Acrylic sheet / 3D print / PVC | Houses mechanics + electronics | ₱400 | ₱400 |
| 20 | 1 | UVC safety glass / shield | UV-blocking acrylic or tinted window | Lets user see the cup without UV exposure | ₱150 | ₱150 |
| 21 | 1 | Fasteners & misc | Screws, standoffs, hot glue, tape | Mounting servos, cell, lamp | ₱100 | ₱100 |
| — | — | **Total** | — | — | — | **≈ ₱2,938** |

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

- **Servo power is the #1 trap.** MG90S stall current can reach ~700 mA each. Two servos **must** run from the **LM2596S buck converter's 5 V output** (12 V in), sharing GND with the Mega. Running them off the Mega's 5 V regulator resets the board mid-dispense.
- **MG90S over SG90:** the MG90S uses **metal gears**, so it survives repeated sweeps and jam-clear taps far better than the plastic-gear SG90 (which strips teeth under load).
- **No pull-down resistors needed** — the snooze/manual buttons and the interlock switch use the Mega's built-in internal pull-ups (`INPUT_PULLUP`), so the 10 kΩ resistors are dropped from the BOM. Only the 220 Ω (LEDs) and 100 Ω (buzzer) remain.
- **LEDs:** exactly two are wired — red on D7 and green on D8. The 220 Ω resistors match that; buy a couple of spares since they are cheap.
- **LM2596S setup:** set the output to exactly **5.0 V** with its trimmer + multimeter *before* connecting any load. Rated 3 A max (add a heatsink for continuous servo use); the 7-segment display shows the output voltage for verification.
- **Load cell capacity** must exceed the tare of the cup + mechanism plus the pill weight. A 1 kg cell is right for a single-pill cup; choose 5 kg only if the mechanism is heavy.
- **UVC**: prefer a lamp module with a documented power (W) so the UV dose can be computed; add the UV-blocking window so the user can observe the cycle safely.
- **RTC**: buy a module that includes the DS3231 (not the older DS1307) — the ±2 ppm accuracy is what makes daily scheduling reliable.
