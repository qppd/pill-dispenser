# Wiring Diagrams

Circuit/wiring diagrams for the pill dispenser, drawn in **Cirkit Designer**.

## What goes here

- Cirkit Designer project files (`.cirkit` / exported JSON)
- Exported images (`PNG`/`SVG`) for embedding in the manuscript and [docs/tutorial.md](../docs/tutorial.md)

## Workflow

1. Open [Cirkit Designer](https://www.cirkitdesigner.com/) and create a new project.
2. Add the components below and connect them per the pin map.
3. Save the project file in this folder as `pill-dispenser-wiring.cirkit`.
4. Export a clean PNG/SVG (`File → Export` in Cirkit) and name it `wiring-overview.png`.

## Component list (matches [docs/bom.md](../docs/bom.md))

Arduino Mega 2560 · DS3231 RTC module · 16×2 LCD with I2C backpack · HX711 + load cell · SG90 servo ×2 · active buzzer · LEDs (red/green) + 220 Ω resistors · push buttons ×2 + 10 kΩ resistors · 5 V relay module · UVC lamp · 9–12 V adapter · 5 V/2 A adapter · breadboard/jumper wires

## Pin reference (from [docs/block-diagram.md](../docs/block-diagram.md))

| Module | Mega 2560 pin | Power |
|---|---|---|
| DS3231 RTC | SDA → 20, SCL → 21 | 5 V, GND |
| 16×2 I2C LCD | SDA → 20, SCL → 21 | 5 V, GND |
| HX711 | DT → 3, SCK → 2 | 5 V, GND |
| Servo — dispenser | signal → 9 | **5 V adapter** (not Mega 5 V) |
| Servo — chute | signal → 10 | **5 V adapter** |
| Buzzer | + → 6 (100 Ω in series) | GND |
| Red LED | + → 7 via 220 Ω | GND |
| Green LED | + → 8 via 220 Ω | GND |
| Relay (UVC) | IN → 11 | 5 V, GND; lamp on relay contacts |
| Snooze button | → 4 (10 kΩ pulldown) | GND |
| Manual button | → 5 (10 kΩ pulldown) | GND |
| Interlock switch | → 12 (10 kΩ pulldown) | GND |
| Power | 9–12 V → VIN | 5 V/2 A adapter shares GND with Mega |

> ⚠️ **Critical rule when drawing:** servos and the relay must be powered from the separate **5 V / 2 A** supply, sharing GND with the Mega — never from the Mega's 5 V pin. Keep the HX711 signal wires away from servo/power traces.
