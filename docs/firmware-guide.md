# Firmware Development Guide — Automated Pill Dispenser

*A developer-focused companion to the build guide. It explains **how the firmware works**, **how to develop and test it incrementally**, and **how to customize it**. The hardware build, wiring, and calibration procedures live in [implementation-guide.md](implementation-guide.md); this guide assumes you have followed Step 4 (wiring) and Step 5 (software) of that document.*

> **Canonical code:** the complete, up-to-date modular sketch lives in [implementation-guide.md → Step 5](implementation-guide.md), with ready-made files in this repo's `firmware/pill_dispenser/` folder. This guide quotes from it. If they ever disagree, implementation-guide.md wins — it is the single source of truth.

---

## 1. What the firmware does

The sketch runs on the Arduino Mega 2560 and coordinates five hardware responsibilities:

1. **Schedule** — watch the DS3231 RTC and fire a dose when the clock matches a scheduled time.
2. **Dispense** — rotate the MG90S dispenser servo to drop exactly one pill (the chute servo is wired for jam clearing but not yet commanded by the firmware).
3. **Confirm intake** — track the cup weight via the HX711 load cell: detect the pill landing, then detect it being removed.
4. **Alert** — drive the buzzer and red/green LEDs; handle the snooze and manual-dispense buttons. (LCD messages beyond the boot banner are planned.)
5. **Sterilize** — run the UVC lamp through the relay for a fixed duration, only while the cover interlock is closed.

All of this is one **state machine**: at any instant the device is in exactly one state and moves to another when a condition is met. See [system-architecture.md](system-architecture.md) for the layered view and the state diagram.

## 2. Codebase layout and libraries

| Piece | What it is |
|---|---|
| Language | Arduino C++ (C++11), one folder with several `.ino` modules |
| IDE | Arduino IDE 2.x (or PlatformIO) |
| `RTClib` (Adafruit) | DS3231 time reading |
| `LiquidCrystal_I2C` | 16×2 LCD via PCF8574 backpack |
| `HX711` (bogde) | Load-cell amplifier readout |
| `Servo`, `Wire` | Built-in; PWM servos and I2C bus |

**File layout** (the whole sketch lives in one folder, `firmware/pill_dispenser/` in this repo, or the `pill_dispenser/` folder you create in Step 5):

| File | Contents |
|---|---|
| `pill_dispenser.ino` | Main file — pins, constants, runtime state, `alert()`, `runSterilization()`, `setup()`, `loop()` (state machine) |
| `scheduler.ino` | `SCHEDULE[]` dose table + `doseDue()` |
| `dispenser.ino` | `dispensePill()` servo sweep |
| `logger.ino` | `logEvent()` CSV logging |

The Arduino IDE compiles every `.ino` in the folder as **one program** — the files share globals (`rtc`, `lcd`, `scale`, `dispenser`, `chute`, `state`, `tare`) defined in the main file. All libraries install via *Tools → Manage Libraries…* (see [implementation-guide.md → Step 5](implementation-guide.md)).

## 3. Reading the sketch, top to bottom

### 3.1 Globals and pins

```cpp
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 scale;
Servo dispenser, chute;

const byte PIN_DT = 3, PIN_SCK = 2;
const byte PIN_SERVO = 9, PIN_CHUTE = 10;
const byte PIN_BUZZER = 6;
const byte PIN_LED_RED = 7, PIN_LED_GREEN = 8;
const byte PIN_RELAY = 11;
const byte PIN_SNOOZE = 4, PIN_MANUAL = 5, PIN_INTERLOCK = 12;
```

- The pin numbers **must match the wiring table** in [implementation-guide.md → Step 4](implementation-guide.md).
- The LCD I2C address defaults to `0x27`; if the display is blank, run an I2C scanner and try `0x3F`.

### 3.2 Schedule and tunable constants

The tunables are split across two files: the **dose schedule** lives in `scheduler.ino`, the **weight/timeout constants** in `pill_dispenser.ino`.

```cpp
// scheduler.ino
struct Dose { uint8_t h, m; };
const Dose SCHEDULE[] = { {8, 0}, {13, 0}, {20, 0} };
const byte DOSES = sizeof(SCHEDULE) / sizeof(Dose);

// pill_dispenser.ino
const float PILL_WEIGHT = 0.8;   // g — set after calibration (Step 6) ←
const float TOLERANCE = 0.5;     // g — acceptance window (Step 6)   ←
const unsigned long REMOVAL_TIMEOUT = 5UL * 60UL * 1000UL; // 5 min
const unsigned long UVC_DURATION = 60UL * 1000UL;          // 60 s
```

- `SCHEDULE` is an array of `{hour, minute}` — add or remove doses here.
- `PILL_WEIGHT` / `TOLERANCE` are set in calibration (Step 6): the formal rule is **±0.5 g or ±5% of pill weight, whichever is larger**.
- `DOSES` is computed, never typed by hand — editing `SCHEDULE` is enough.

### 3.3 The state machine

```cpp
enum State { IDLE, ALERT, DISPENSE, WAIT_REMOVAL, CONFIRMED, STERILIZE, ERROR_STATE };
State state = IDLE;
```

The `loop()` is a `switch` on `state`; each `case` is a state's behavior. States map to the architecture's modules like this:

| State | Architecture module | What happens |
|---|---|---|
| `IDLE` | `Scheduler` | Watches the clock + manual button; does nothing else |
| `ALERT` | `AlertSystem` | Buzzer + red LED on; waits for snooze or auto-acknowledge |
| `DISPENSE` | `Dispenser` | Sweeps the dispenser servo, re-tares the scale |
| `WAIT_REMOVAL` | `WeightMonitor` | Watches weight drop back to tare (pill taken) or times out (missed) |
| `CONFIRMED` | `LogStore` + `AlertSystem` | Green LED, writes `CONFIRMED` log entry |
| `STERILIZE` | `UVCSterilizer` | Runs UVC for `UVC_DURATION`, checks interlock first |
| `ERROR_STATE` | `StateMachine` | Reserved for a future retry/error path (see §8) |

### 3.4 State-by-state walkthrough

**`IDLE`** — the resting state. Every loop iteration it asks the RTC whether a dose is due, and also watches the manual-dispense button (caregiver override):

```cpp
case IDLE:
  if (doseDue() || !digitalRead(PIN_MANUAL)) { alertStart = millis(); state = ALERT; }  // pressed = LOW
  break;
```

`doseDue()` reads `rtc.now()` and compares hour/minute against every entry in `SCHEDULE`:

```cpp
bool doseDue() {
  DateTime now = rtc.now();
  for (byte i = 0; i < DOSES; i++)
    if (now.hour() == SCHEDULE[i].h && now.minute() == SCHEDULE[i].m) return true;
  return false;
}
```

**`ALERT`** — the reminder state. Turns the buzzer and red LED on, then exits when the user snoozes or after a 30 s auto-acknowledge:

```cpp
case ALERT:
  alert(true); digitalWrite(PIN_LED_RED, HIGH);
  if (!digitalRead(PIN_SNOOZE) || millis() - alertStart > 30000) { alert(false); state = DISPENSE; }  // pressed = LOW
  break;
```

**`DISPENSE`** — sweeps the servo to drop a pill, then re-tares the scale so the next state measures the weight delta:

```cpp
void dispensePill() {
  dispenser.write(90); delay(600); dispenser.write(0);  // sweep → release pill
  delay(800);                                           // let it settle on the cup
}
```

The two angles (`90` then `0`) are the calibration values from Step 6.3 — tune them until exactly one pill drops per sweep, ten times in a row.

**`WAIT_REMOVAL`** — the core intake-detection logic. The scale was tared right after dispensing (pill present = weight above tare). When the weight falls back to (or below) `tare + TOLERANCE`, the pill was removed:

```cpp
case WAIT_REMOVAL: {
  float w = scale.get_units();
  if (w <= tare + TOLERANCE) state = CONFIRMED;      // pill removed
  else if (millis() - removalStart > REMOVAL_TIMEOUT) { logEvent("MISSED"); state = IDLE; }
  break;
}
```

This is **weight-delta logic, not absolute weight** — cup residue and temperature drift are compensated because every comparison is against the live tare.

**`CONFIRMED`** — green LED + a timestamped `CONFIRMED` log entry, then on to sterilization:

```cpp
case CONFIRMED:
  digitalWrite(PIN_LED_RED, LOW); digitalWrite(PIN_LED_GREEN, HIGH);
  logEvent("CONFIRMED"); delay(2000); digitalWrite(PIN_LED_GREEN, LOW);
  state = STERILIZE; break;
```

**`STERILIZE`** — runs the UVC lamp for `UVC_DURATION`, but **refuses if the interlock is open**:

```cpp
void runSterilization() {
  if (digitalRead(PIN_INTERLOCK) == HIGH) return;       // cover open → refuse (closed = LOW)
  digitalWrite(PIN_RELAY, HIGH);
  delay(UVC_DURATION);
  digitalWrite(PIN_RELAY, LOW);
}
```

## 4. Input logic: internal pull-ups (important)

The three inputs — snooze (D4), manual (D5), interlock (D12) — use the Mega's **internal pull-ups**, not external resistors:

```cpp
pinMode(PIN_SNOOZE, INPUT_PULLUP); pinMode(PIN_MANUAL, INPUT_PULLUP); pinMode(PIN_INTERLOCK, INPUT_PULLUP);
```

With `INPUT_PULLUP`, the pin reads `HIGH` normally and the button/switch **shorts the pin to GND when pressed**, so:

- **pressed = LOW** → button reads are inverted: `!digitalRead(PIN_SNOOZE)`, `!digitalRead(PIN_MANUAL)`
- **interlock closed = LOW** → the UVC guard is `digitalRead(PIN_INTERLOCK) == HIGH` (open cover refuses)

Wire each button one leg to its pin, other leg to GND — no 10 kΩ resistors needed (see [bom.md](bom.md) selection notes).

## 5. Logging and the serial format

```cpp
void logEvent(const char* tag) {
  DateTime now = rtc.now();
  Serial.print(tag); Serial.print(",");
  Serial.print(now.timestamp()); Serial.println();
}
```

Every log line is one CSV row: `tag,timestamp` — e.g. `CONFIRMED,2026-08-17T08:05:00`. Three tags are emitted: `MISSED`, `CONFIRMED`, `STERILIZED`. The timestamp is ISO 8601 from the RTC, so the Serial Monitor output can be saved directly as `.csv` and opened in Excel/Google Sheets. `setup()` opens the link at 115200 baud.

## 6. Development workflow (build it up in milestones)

Do **not** upload the whole sketch to a fresh build. Develop in order — each milestone is independently testable and isolates wiring/software mistakes:

| # | Milestone | Test |
|---|---|---|
| M1 | `Blink` on LED D7 | LED blinks — board + IDE + upload path work |
| M2 | RTC read via Serial | `rtc.now()` prints correct time; survives power cycle (battery) |
| M3 | LCD hello | `Pill Dispenser OK` shows; fix I2C address if blank |
| M4 | HX711 read | Value changes when you press the load cell; no `NaN` |
| M5 | Servo sweep ×2 | Both MG90S rotate 0–180°; **servos powered from LM2596S rail** |
| M6 | Buzzer + LEDs + buttons | Buzzer beeps, LEDs light, buttons read LOW when pressed (pull-ups) |
| M7 | Relay + interlock | Relay clicks; UVC refuses to run with cover open |
| M8 | Full state machine | The complete cycle: alert → dispense → remove → confirmed → sterilize → idle |

Each milestone is a small sketch; only M8 is the full code from Step 5. This order matches the wiring order in [implementation-guide.md → Step 4](implementation-guide.md), so by M8 every subsystem is already known-good.

## 7. Testing the firmware

Beyond the build checks in Step 7 of the implementation guide, test these firmware-specific cases:

- **Missed dose:** dispense, then *don't* remove the pill → expect `MISSED` after the timeout and a return to `IDLE`.
- **Manual override:** press the manual button while idle → expect the full alert→dispense cycle.
- **Snooze:** during `ALERT`, press snooze → alert stops, dispense proceeds.
- **Interlock safety:** open the cover during `STERILIZE` (or before it) → the relay must not energize. Verify the relay is truly off, not just "intended to be".
- **Power-cycle:** unplug and replug mid-cycle → the RTC keeps time; the device returns to `IDLE` (state is not persisted).
- **Log format:** capture ≥ 3 events and open the CSV in a spreadsheet — one column (tag) + one column (timestamp), no stray characters.

For evaluation trials, the `CONFIRMED`/`MISSED`/`STERILIZED` lines are the machine-readable log — see [testing-evaluation.md](testing-evaluation.md).

## 8. Customization

| What | Where | How |
|---|---|---|
| Dose schedule | `SCHEDULE[]` | Edit the `{hour, minute}` entries; `DOSES` updates itself |
| Pill weight / tolerance | `PILL_WEIGHT`, `TOLERANCE` | Set from calibration (Step 6) |
| Servo angles | `dispensePill()` | Change `90` / `0` to your calibrated sweep angles |
| UVC duration | `UVC_DURATION` | Seconds × 1000; must deliver ≥ 40 mJ/cm² (see testing doc §8) |
| Removal timeout | `REMOVAL_TIMEOUT` | How long to wait before logging `MISSED` |
| Alert auto-duration | `millis() - alertStart > 30000` in `ALERT` | The 30 s auto-acknowledge |
| LCD address | `LiquidCrystal_I2C lcd(0x27, ...)` | `0x3F` for common clones |
| Log tags | `logEvent("...")` calls | Add tags for new events (keep the `tag,timestamp` shape) |

### Adding a feature (worked example: a real snooze timer)

The current sketch treats snooze as "acknowledge now, dispense immediately." To snooze for N minutes instead:

1. Add a constant: `const unsigned long SNOOZE_MS = 10UL * 60UL * 1000UL;`
2. In `ALERT`, on snooze: record `unsigned long snoozeUntil = millis() + SNOOZE_MS;` and return to `IDLE`.
3. In `IDLE`, treat "dose due OR snooze expired" as the trigger — e.g. keep a `bool snoozed` flag and a target timestamp.

> **Tip — timeouts:** always capture `millis()` at the start of a timed state and compare `millis() - start >= duration`. See the known issue below.

### Timeout pattern (why the sketch captures `millis()` at state entry)

`millis()` counts up from **boot**, never from "when I entered this state." Comparing it against an absolute duration — `millis() > 30000` — would fire immediately once the board has been on longer than that, so the sketch records a start timestamp when each timed state begins and compares the delta instead:

```cpp
unsigned long alertStart = 0;    // set when ALERT begins
unsigned long removalStart = 0;  // set when WAIT_REMOVAL begins
...
case IDLE:
  if (doseDue() || !digitalRead(PIN_MANUAL)) { alertStart = millis(); state = ALERT; }
  break;
case ALERT:
  alert(true); digitalWrite(PIN_LED_RED, HIGH);
  if (!digitalRead(PIN_SNOOZE) || millis() - alertStart > 30000) { alert(false); state = DISPENSE; }
  break;
```

Follow the same capture-on-entry pattern for any new timed behavior (snooze timer, retry delay, etc.).

## 9. Firmware troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| Won't compile | Missing library | Install `RTClib`, `LiquidCrystal_I2C`, `HX711` via Library Manager |
| LCD blank / blocks | Wrong I2C address | Run an I2C scanner; try `0x3F` |
| HX711 reads `NaN` | Loose DT/SCK wires; servo noise | Re-seat wires; keep servo wires away from HX711 leads |
| Buttons do nothing | Wired to 5 V (old pull-down style) | Re-wire: one leg to pin, other leg to GND (pull-up style) |
| UVC runs with cover open | Interlock logic inverted | Closed switch must read LOW; check wiring + `INPUT_PULLUP` |
| Serial garbage | Wrong baud rate | Set Serial Monitor to 115200 |
| Board resets when servo moves | Servo powered from Mega 5 V | Move servo power to the LM2596S rail |
| Alert/removal timeout fires immediately | Timeout compared against absolute `millis()` | Capture a start timestamp at state entry and compare deltas (§8) |

## 10. Where to go next

- **Why the layers are arranged this way** → [system-architecture.md](system-architecture.md)
- **Libraries and upgrade path** (e.g., SD-card logging, Bluetooth alerts) → [stack.md](stack.md)
- **Formal evaluation protocol** (trials, stats, UVC validation) → [testing-evaluation.md](testing-evaluation.md)
