# System Architecture

The device is organized into four layers. Data flows bottom-up (hardware → firmware → data → interface) and control flows top-down (interface/firmware → hardware).

```
┌─────────────────────────────────────────────────────────────────┐
│  INTERFACE LAYER    16×2 LCD · push buttons · serial monitor     │
├─────────────────────────────────────────────────────────────────┤
│  DATA LAYER         medication schedule · intake log · errors    │
├─────────────────────────────────────────────────────────────────┤
│  FIRMWARE LAYER     scheduler · dispenser · weight monitor ·     │
│                     alert · UVC controller · state machine       │
├─────────────────────────────────────────────────────────────────┤
│  HARDWARE LAYER     Mega 2560 · DS3231 · HX711 · servos ·        │
│                     LCD · buzzer · LEDs · relay/UVC · power      │
└─────────────────────────────────────────────────────────────────┘
```

## 1. Hardware layer

| Module | Interface | Responsibility |
|---|---|---|
| Arduino Mega 2560 | — | Executes firmware; the only active brain |
| DS3231 RTC | I2C | Supplies wall-clock time; survives power loss via CR2032 |
| HX711 + load cell | DT/SCK | Reports cup weight continuously |
| MG90S servos (metal gear) | PWM | Rotates dispenser; presents spoon |
| 16×2 LCD | I2C | Renders schedule, alerts, status |
| Buzzer + LEDs | Digital | Audiovisual reminders |
| Relay + UVC LED module | Digital | Sterilization cycle (relay switches the LED module's 12 V feed) |
| Power (12 V + LM2596S) | VIN / 5 V rail | 12 V adapter feeds the MCU (VIN) and the buck converter for actuators |
| Power (UVC LED) | — (relay contacts) | 12 V from the main adapter switched by the relay to the UVC LED module (built-in driver); no ballast, no mains |

## 2. Firmware layer (Arduino sketch)

| Module | Function |
|---|---|
| `Scheduler` | Reads RTC; matches current time against the dose table; fires due doses |
| `AlertSystem` | Drives buzzer/LED state; handles snooze (LCD messages are planned) |
| `Dispenser` | Commands the dispenser servo rotation and returns to rest |
| `WeightMonitor` | Re-tares after dispensing; detects *pill removed* (weight ↓ toward tare) using the tolerance window |
| `UVCSterilizer` | Runs the LED module for the set duration; refuses to run unless the interlock is satisfied |
| `StateMachine` | Coordinates the above; owns the state transitions (below) |
| `LogStore` | Appends timestamped events (confirmed, missed, sterilized) to serial; EEPROM logging is planned |

## 3. Data layer

| Data | Source | Stored in |
|---|---|---|
| Medication schedule (time, dose) | Defined at setup | Program array (`SCHEDULE[]`) |
| Tare weight | Calibrated at boot | RAM |
| Intake log (timestamp, event) | Runtime events | Serial Monitor (CSV); EEPROM logging planned |
| Trial records | Evaluation runs | CSV via serial (for analysis) |

## 4. Interface layer

- **LCD** — shows `Pill Dispenser OK` at boot; live time display and status messages are planned.
- **Buttons** — snooze (acknowledge the alert; currently dispenses immediately — a snooze timer is planned), manual dispense (caregiver override).
- **Serial Monitor** — debug output and machine-readable log for evaluation trials.

## Firmware state machine

```
        ┌──────────► IDLE ◄────────────────────┐
        │              │  dose due / manual btn │
        │              ▼                        │
        │          ALERT ──snooze / auto──► DISPENSE
        │              │
        │              ▼
        │        DISPENSE (sweep + re-tare)
        │              │
        │              ▼
        │     WAIT_REMOVAL ──timeout──► log MISSED → IDLE
        │              │  weight returns to tare
        │              ▼
        │       CONFIRMED (green LED, log)
        │              │
        │              ▼
        │       STERILIZE (UVC, interlock checked)
        └──────────────┘
```

| State | Entry condition | Exit condition |
|---|---|---|
| `IDLE` | Boot / any state done | Dose time reached or manual button |
| `ALERT` | Dose due | Snooze pressed or auto after 30 s (both → `DISPENSE`) |
| `DISPENSE` | Alert done | Servo sweep completes (no weight check in current firmware) |
| `WAIT_REMOVAL` | Pill dispensed | Weight back near tare (taken) or timeout (logs `MISSED`, → `IDLE`) |
| `CONFIRMED` | Removal detected | Log entry written |
| `STERILIZE` | Intake confirmed | UVC duration elapsed, interlock safe |

> **Not in the current firmware (planned):** an `ERROR` retry path when no weight increase is detected, a separate `MISSED` state that re-alerts, and a snooze timer (snooze currently dispenses immediately). See [firmware-guide.md §8](firmware-guide.md).

## Key design decisions

1. **Separate servo power rail** — actuators never draw from the Mega's 5 V regulator.
2. **UVC LED on the 12 V rail** — the UVC LED module (265–280 nm) has a built-in constant-current driver, so it runs directly from the 12 V adapter through the relay contacts — no ballast and no 220 V mains anywhere in the device.
3. **Weight-delta logic, not absolute weight** — the system compares against the live tare, so cup residue and temperature drift are compensated.
4. **Interlock on UVC** — the LED module only energizes when the dispenser cover/window is closed (door switch or button state), preventing UV exposure to the user.
5. **Timeout-driven escalation** — a missed dose is not silently dropped: it is logged with a `MISSED` event for the caregiver/study record. (Automatic re-alert is planned; see [firmware-guide.md §8](firmware-guide.md).)
