# Wiring Diagrams

Circuit/wiring diagrams for the pill dispenser, drawn in **Cirkit Designer**.

## What goes here

- Cirkit Designer project files (`.cirkit` / exported JSON)
- Exported images (`PNG`/`SVG`) for embedding in the manuscript and [docs/implementation-guide.md](../docs/implementation-guide.md)

## Workflow

1. Open [Cirkit Designer](https://www.cirkitdesigner.com/) and create a new project.
2. Add the components below and connect them per the pin map.
3. Save the project file in this folder as `pill-dispenser-wiring.cirkit`.
4. Export a clean PNG/SVG (`File → Export` in Cirkit) and name it `wiring-overview.png`.

## Component list (matches [docs/bom.md](../docs/bom.md))

Arduino Mega 2560 · DS3231 RTC module · 16×2 LCD with I2C backpack · HX711 + load cell · MG90S metal-gear servo ×2 · active buzzer · LEDs (red/green) + 220 Ω resistors · push buttons ×2 (internal pull-up) · interlock microswitch · 5 V relay module · UVC lamp · 12 V adapter · LM2596S buck converter (12 V → 5 V, 7-seg voltmeter) · breadboard/jumper wires

## Pin reference (from [docs/block-diagram.md](../docs/block-diagram.md))

| Module | Mega 2560 pin | Power |
|---|---|---|
| DS3231 RTC | SDA → 20, SCL → 21 | 5 V, GND |
| 16×2 I2C LCD | SDA → 20, SCL → 21 | 5 V, GND |
| HX711 | DT → 3, SCK → 2 | 5 V, GND |
| Servo — dispenser | signal → 9 | **5 V rail (LM2596S)** (not Mega 5 V) |
| Servo — chute | signal → 10 | **5 V rail (LM2596S)** |
| Buzzer | + → 6 (100 Ω in series) | GND |
| Red LED | + → 7 via 220 Ω | GND |
| Green LED | + → 8 via 220 Ω | GND |
| Relay (UVC) | IN → 11 | 5 V, GND; lamp on relay contacts |
| Snooze button | → 4 (INPUT_PULLUP) | other leg → GND |
| Manual button | → 5 (INPUT_PULLUP) | other leg → GND |
| Interlock switch | → 12 (INPUT_PULLUP) | other leg → GND |
| Power | 12 V → VIN | 12 V → LM2596S in; 5 V out → servo/relay rail; GND shared |

> **Critical rule when drawing:** servos and the relay must be powered from the **LM2596S buck converter's 5 V output** (12 V in), sharing GND with the Mega — never from the Mega's 5 V pin. Keep the HX711 signal wires away from servo/power traces.

## Power via VIN (decision)

The Mega 2560 is powered through the **VIN pin** (not USB, not the barrel jack):

- Connect the 12 V adapter's `+` to **VIN** and `−` to **GND**. The onboard regulator steps the input down to 5 V for the logic.
- **VIN accepts 7–12 V; do not exceed 12 V.**
- Unplug USB when running on VIN power.
- Servos and the relay run from the **LM2596S buck converter** (12 V in → 5 V out) — the Mega's 5 V output cannot supply their inrush current.
- **LM2596S setup:** set the output to exactly **5.0 V** (trimmer + multimeter) before connecting any load; rated 3 A max (add a heatsink for continuous use); the 7-segment display shows the output voltage for verification.
