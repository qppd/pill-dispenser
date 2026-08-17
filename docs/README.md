# Automated Pill Dispenser for PWDs — Documentation

An Arduino Mega 2560–based automated pill dispenser for Persons with Disabilities (PWDs). The system combines **RTC-based medication scheduling**, **servo-driven dispensing**, **load-cell intake monitoring**, **audiovisual reminders** (LCD, buzzer, LEDs), and **UVC sterilization**.

This folder is the single source of truth for building, wiring, coding, and evaluating the device.

## Documents

| Document | What it covers |
|---|---|
| [implementation-guide.md](implementation-guide.md) | Step-by-step implementation guide: mechanical assembly, wiring, firmware, calibration, deployment |
| [bom.md](bom.md) | Bill of materials — quantities, specifications, roles, indicative prices (PHP) |
| [block-diagram.md](block-diagram.md) | Hardware block diagram: every module and the interface that connects it to the MCU |
| [flowchart.md](flowchart.md) | Operating flowchart: power-on → monitoring → alert → dispense → intake confirmation → UVC |
| [system-architecture.md](system-architecture.md) | Layered architecture (hardware / firmware / data / interface) and the firmware state machine |
| [stack.md](stack.md) | Full tech stack: hardware, firmware libraries, development and evaluation tools |
| [testing-evaluation.md](testing-evaluation.md) | Formal testing & evaluation methodology: variables, metrics, trials, statistics, UVC validation |

## System at a glance

| Aspect | Specification |
|---|---|
| Controller | Arduino Mega 2560 (ATmega2560, 5 V, 16 MHz) |
| Scheduling | DS3231 RTC module (I2C, ±2 ppm, CR2032 battery backup) |
| Dispensing | SG90 servo–driven mechanism (upgrade option: MG995/MG996R) |
| Intake monitoring | HX711 24-bit ADC + load cell (~0.1 g resolution) |
| Reminders | 16×2 I2C LCD, active buzzer, red/green LEDs |
| Sterilization | UVC lamp (254 nm mercury or 260–280 nm LED) switched by a 5 V relay |
| Power | 9–12 V adapter for the Mega; separate 5 V / 2 A supply for servos & relay |

## Suggested reading order

1. [system-architecture.md](system-architecture.md) — understand what the system does and how it is layered
2. [block-diagram.md](block-diagram.md) — see how the modules connect
3. [bom.md](bom.md) — buy the parts
4. [implementation-guide.md](implementation-guide.md) — build, wire, and program it
5. [testing-evaluation.md](testing-evaluation.md) — run the formal tests that produce research data

## Notes

- Diagrams are written in **Mermaid**. They render on GitHub, in VS Code (Markdown Preview Mermaid extension), or at [mermaid.live](https://mermaid.live). Each diagram file also includes a plain-ASCII fallback.
- This documentation follows the study's methodology workflow: materials gathering → hardware development → assembly → mechanical adjustment → coding → software development → deployment and evaluation.
