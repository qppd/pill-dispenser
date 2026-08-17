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
| SG90 servos | PWM | Rotates dispenser; agitates chute |
| 16×2 LCD | I2C | Renders schedule, alerts, status |
| Buzzer + LEDs | Digital | Audiovisual reminders |
| Relay + UVC lamp | Digital | Sterilization cycle |
| Power (9–12 V + 5 V) | VIN / 5 V | Supplies MCU and actuators separately |

## 2. Firmware layer (Arduino sketch)

| Module | Function |
|---|---|
| `Scheduler` | Reads RTC; matches current time against the dose table; fires due doses |
| `AlertSystem` | Drives LCD message, buzzer pattern, LED state; handles snooze |
| `Dispenser` | Commands the servo rotation and returns to rest; runs a retry on jam |
| `WeightMonitor` | Continuous tare tracking; detects *pill added* (weight ↑) and *pill removed* (weight ↓ toward tare) using the tolerance window |
| `UVCSterilizer` | Runs the lamp for the set duration; refuses to run unless the interlock is satisfied |
| `StateMachine` | Coordinates the above; owns the state transitions (below) |
| `LogStore` | Appends timestamped events (dispensed, confirmed, missed, error) to EEPROM/serial |

## 3. Data layer

| Data | Source | Stored in |
|---|---|---|
| Medication schedule (time, dose) | Defined at setup | EEPROM or program array |
| Tare weight | Calibrated at boot | RAM |
| Intake log (timestamp, event) | Runtime events | EEPROM / Serial Monitor |
| Trial records | Evaluation runs | CSV via serial (for analysis) |

## 4. Interface layer

- **LCD** — shows next dose time, current alert, and status messages.
- **Buttons** — snooze (silence the buzzer for N minutes), manual dispense (caregiver override).
- **Serial Monitor** — debug output and machine-readable log for evaluation trials.

## Firmware state machine

```
        ┌──────────► IDLE ◄────────────────────┐
        │              │  dose due              │
        │              ▼                        │
        │          ALERT ──snooze──► (back to IDLE)
        │              │  buzzer acknowledged / auto
        │              ▼
        │        DISPENSE ──no weight↑──► ERROR (retry)
        │              │  pill detected
        │              ▼
        │     WAIT_REMOVAL ──timeout──► MISSED (re-alert)
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
| `IDLE` | Boot / any state done | Dose time reached |
| `ALERT` | Dose due | Acknowledged or auto after N seconds |
| `DISPENSE` | Alert done | Weight increase ≥ pill weight − tolerance |
| `WAIT_REMOVAL` | Pill dispensed | Weight back near tare (taken) or timeout (missed) |
| `CONFIRMED` | Removal detected | Log entry written |
| `STERILIZE` | Intake confirmed | UVC duration elapsed, interlock safe |
| `ERROR` | No weight change / anomaly | Retry or manual reset |
| `MISSED` | Removal timeout | Re-alert or next schedule |

## Key design decisions

1. **Separate servo power rail** — actuators never draw from the Mega's 5 V regulator.
2. **Weight-delta logic, not absolute weight** — the system compares against the live tare, so cup residue and temperature drift are compensated.
3. **Interlock on UVC** — the lamp only energizes when the dispenser cover/window is closed (door switch or button state), preventing UV exposure to the user.
4. **Timeout-driven escalation** — a missed dose is not silently dropped; it re-alerts and is logged for the caregiver/study record.
