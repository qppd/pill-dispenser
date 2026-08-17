# Automated Pill Dispenser for PWDs

An Arduino Mega 2560–based automated pill dispenser for Persons with Disabilities (PWDs). The system combines RTC-based medication scheduling, servo-driven dispensing, load-cell intake monitoring, audiovisual reminders (LCD, buzzer, LEDs), and UVC sterilization.

## Features

- ⏰ **Scheduled dispensing** — DS3231 RTC (±2 ppm, battery-backed) triggers doses at set times
- 💊 **Servo-driven mechanism** — dispenses exactly one pill per sweep; chute agitator clears jams
- ⚖️ **Intake monitoring** — HX711 + load cell detects the dispensed pill and confirms it was taken
- 🔔 **Reminders** — 16×2 I2C LCD, active buzzer, and red/green LED indicators
- ☀️ **UVC sterilization** — relay-controlled germicidal lamp with a safety interlock

## Repository layout

```
├── README.md                   ← this file
├── .gitignore
└── docs/                       ← full project documentation
    ├── README.md               ← docs index
    ├── implementation-guide.md ← build, wire, calibrate, deploy
    ├── bom.md                  ← bill of materials (PHP)
    ├── block-diagram.md        ← hardware block diagram
    ├── flowchart.md            ← operating flowchart
    ├── system-architecture.md  ← architecture + firmware state machine
    ├── stack.md                ← full tech stack
    └── testing-evaluation.md   ← testing & evaluation methodology
```

## Getting started

1. Read [docs/system-architecture.md](docs/system-architecture.md) to understand the system.
2. Buy the parts listed in [docs/bom.md](docs/bom.md).
3. Follow [docs/implementation-guide.md](docs/implementation-guide.md) to build, wire, and program the device.
4. Run the formal evaluation protocol in [docs/testing-evaluation.md](docs/testing-evaluation.md).

## Hardware at a glance

| Aspect | Specification |
|---|---|
| Controller | Arduino Mega 2560 (ATmega2560) |
| Scheduling | DS3231 RTC (I2C, CR2032 backup) |
| Dispensing | SG90 servos (upgrade: MG995/MG996R) |
| Sensing | HX711 + load cell (~0.1 g resolution) |
| Interface | 16×2 I2C LCD, buzzer, LEDs, buttons |
| Sterilization | UVC lamp via 5 V relay, interlock protected |
| Power | 9–12 V for the Mega; separate 5 V/2 A for actuators |

## Status

Prototype documentation phase — firmware sketch and physical build pending. See [docs/implementation-guide.md](docs/implementation-guide.md) for the next steps.
