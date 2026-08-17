# Implementation Guide — Automated Pill Dispenser

*A student-friendly, step-by-step build guide. Follow the steps in order — each step depends on the one before it.*

## What you will end up with

A working pill dispenser that:

- Shows the time on the LCD and keeps it accurate even after power loss
- Alerts (LCD + buzzer + red LED) at the scheduled medication times
- Dispenses **exactly one pill** per dose
- Detects when the pill is taken (green LED + log entry)
- Runs a UVC sterilization cycle after each confirmed intake

## Who this is for

2nd–4th year engineering/IT students. You should already have done one Arduino project (like the Blink sketch). You do **not** need to know I2C, PWM, or load cells yet — those are explained in the [Glossary](#glossary-plain-words) and in each step.

## How long it takes

| Step | What | Time |
|---|---|---|
| 0 | Know your parts | 20 min |
| 1 | Tools | 15 min |
| 2 | Buy + check parts | 1 hr |
| 3 | Mechanics | 2–3 hrs |
| 4 | Wiring | 1–2 hrs |
| 5 | Software | 1–2 hrs |
| 6 | Calibration | 1–1.5 hrs |
| 7 | System testing | 2–3 hrs |
| 8 | Deployment | 30 min |

**Total: about 8–12 hours** of part-time work.

## How to use this guide

Every step has the same format:

1. **Why** — one line explaining the point of the step (so you understand, not just copy).
2. **What to do** — the numbered instructions.
3. **Check** — *verify your work before moving on.* If a check fails, fix it first. Skipping a check is how broken devices happen.

> **Read the [Safety notes](#safety-notes) at the end of this guide before you start Step 3.** Especially the UVC and servo-power rules.

---

## Step 0 — Know your parts (20 min)

Open [block-diagram.md](block-diagram.md) and keep it beside you. Quick reference:

| Part | What it does | Its "language" |
|---|---|---|
| Arduino Mega 2560 | The brain. Runs the program, reads sensors, drives outputs | — |
| DS3231 RTC | Keeps the correct time, even without power (battery backup) | I2C |
| 16×2 LCD + backpack | Shows messages (time, alerts) | I2C |
| HX711 + load cell | Acts like a tiny weighing scale for the cup | DT/SCK pins |
| SG90 servo ×2 | Small motors that rotate to a set angle — one dispenses, one clears jams | PWM |
| Buzzer + LEDs | Make noise and light for reminders | Digital pins |
| Relay + UVC lamp | Relay is a remote-controlled switch that turns the lamp on/off | Digital pin |
| 9–12 V and 5 V adapters | Power. The 9–12 V feeds the Mega; the 5 V feeds the servos and relay | — |

Don't memorize this — just know *where to look it up*.

## Step 1 — Get your tools ready (15 min)

| Tool | Used for |
|---|---|
| Soldering iron + solder | Permanent joints (relay, power wires) |
| Multimeter | Checking connections (Step 4) |
| Wire strippers + screwdriver | Preparing wires, mounting |
| Hot glue gun / epoxy + zip ties | Mounting parts in the enclosure |
| A clean workbench | So small parts don't get lost |

## Step 2 — Buy and check all parts (1 hr)

**Why:** A missing or defective part stops the build — and some parts (like a dead RTC battery) are only easy to fix now.

Buy everything in [bom.md](bom.md), then check each item **the moment it arrives**:

- [ ] Mega 2560 plugs into the PC and shows up as a COM port (Arduino IDE → Tools → Port)
- [ ] DS3231 module includes the CR2032 battery **and** the battery is inside its holder
- [ ] Both SG90 servos rotate: upload the built-in example *File → Examples → Servo → Sweep* and watch the horn move
- [ ] HX711 + load cell respond: upload *File → Examples → HX711 → Read_1x_load_cell* and see a changing value when you press the load cell
- [ ] UVC lamp module has a printed wattage (you need it later for the dose calculation)

**Check:** every box above is ticked. If a part fails its test, replace it now — do not build it in.

## Step 3 — Build the mechanics (2–3 hrs)

**Why:** the electronics only work if the physical parts move freely and nothing vibrates the scale.

### 3.1 Enclosure

Cut/print the housing with **four compartments**: pill hopper (top), dispensing cup (front), electronics bay (back), and the UVC chamber behind a UV-blocking window.

### 3.2 Dispenser mechanism (the one-pill rule)

Mount the dispenser servo so its horn turns a **blocker plate** in front of the hopper opening. One sweep = one pill falls into the cup.

**Test now, by hand:** rotate the horn slowly and confirm exactly one pill drops per pass. If two drop, the opening is too wide — adjust the plate before wiring anything.

### 3.3 Chute agitator

Mount the second servo so it can tap/vibrate the hopper when a pill jams (pills sometimes stick together or wedge).

### 3.4 Load cell (the most sensitive part)

Fix one end of the load-cell bar to the enclosure. Mount the cup platform on the **free end**. **Nothing else may touch the free end** — not a wire, not a servo, not the enclosure wall. Contact here is the #1 cause of noisy readings.

### 3.5 UVC lamp + interlock

Place the lamp so its beam hits the cup/dispenser area, behind the UV-blocking window. Mount a **microswitch (interlock)** on the cover — the lamp must only run when the cover is closed.

### 3.6 Cable routing

Keep servo and power wires **away from** the load cell and HX711 leads — motor noise corrupts scale readings. Twist the two HX711 signal wires together along their length.

**Check:** (a) one pill per sweep, by hand, ×10 tries; (b) the cup platform moves freely and nothing touches it; (c) the interlock switch clicks when the cover opens/closes; (d) the lamp is fully behind the UV-blocking window.

## Step 4 — Wire the electronics (1–2 hrs)

**Why:** every part needs power and a signal wire to the Mega. A single wrong wire is the most common cause of "it doesn't work."

**Golden rules (read twice):**

> **Rule 1 — Servo power.** The servos and relay are powered by the **separate 5 V/2 A adapter**, *not* the Mega's 5 V pin. The Mega's regulator cannot supply the current the servos draw when they move — the board will reset mid-dispense.
>
> **Rule 2 — Common ground.** The 5 V adapter's GND must connect to the Mega's GND. Without a shared ground, signals don't work.

**Wire in this order** (easier to find mistakes):

1. Power rails: 9–12 V adapter → Mega VIN; 5 V adapter → breadboard rail
2. I2C devices: DS3231 and LCD (both to SDA=20, SCL=21)
3. Sensor: HX711 (DT=3, SCK=2)
4. Actuators: both servos (signal to 9 and 10, power from the 5 V rail) and the relay (IN=11)
5. Indicators + inputs: buzzer, LEDs, buttons, interlock

| Module | Mega pin | Also connect |
|---|---|---|
| DS3231 | SDA → 20, SCL → 21 | VCC → 5 V, GND |
| LCD (I2C) | SDA → 20, SCL → 21 | VCC → 5 V, GND |
| HX711 | DT → 3, SCK → 2 | VCC → 5 V, GND |
| Servo (dispenser) | signal → 9 | VCC/GND → **5 V adapter** |
| Servo (chute) | signal → 10 | VCC/GND → **5 V adapter** |
| Buzzer | + → 6 | GND; 100 Ω in series |
| Red LED | + → 7 via 220 Ω | GND |
| Green LED | + → 8 via 220 Ω | GND |
| Relay | IN → 11 | VCC → 5 V, GND; UVC lamp on the relay contacts |
| Snooze button | → 4 (10 kΩ pulldown) | GND |
| Manual button | → 5 (10 kΩ pulldown) | GND |
| Interlock switch | → 12 (10 kΩ pulldown) | GND |
| Power | 9–12 V → VIN | 5 V/2 A adapter GND joined to Mega GND |

**Before applying power**, use the multimeter in continuity mode to confirm: no short between 5 V and GND on the breadboard rail, and each module's VCC pin only connects where it should.

**Check:** (a) no shorts; (b) adapter voltages measure correctly (9–12 V and 5 V); (c) every signal pin matches the table. Only then plug in the power.

## Step 5 — Install the software (1–2 hrs)

**Why:** the Mega does nothing until it has a program ("sketch").

1. Install **Arduino IDE 2.x**.
2. Install libraries: *Tools → Manage Libraries…* → search and install `RTClib` (Adafruit), `LiquidCrystal_I2C`, and `HX711` (by bogde). (`Servo` and `Wire` are already built in.)
3. Create a new sketch and paste the code below. The parts you must change after calibration are marked with `// ←`.

```cpp
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <Servo.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 scale;
Servo dispenser, chute;

// Pins (see Step 4)
const byte PIN_DT = 3, PIN_SCK = 2;
const byte PIN_SERVO = 9, PIN_CHUTE = 10;
const byte PIN_BUZZER = 6;
const byte PIN_LED_RED = 7, PIN_LED_GREEN = 8;
const byte PIN_RELAY = 11;
const byte PIN_SNOOZE = 4, PIN_MANUAL = 5, PIN_INTERLOCK = 12;

// Schedule: {hour, minute} — change to the study's dose times
struct Dose { uint8_t h, m; };
const Dose SCHEDULE[] = { {8, 0}, {13, 0}, {20, 0} };
const byte DOSES = sizeof(SCHEDULE) / sizeof(Dose);

const float PILL_WEIGHT = 0.8;   // g — set after calibration (Step 6) ←
const float TOLERANCE = 0.5;     // g — acceptance window (Step 6)   ←
const unsigned long REMOVAL_TIMEOUT = 5UL * 60UL * 1000UL; // 5 min
const unsigned long UVC_DURATION = 60UL * 1000UL;          // 60 s

enum State { IDLE, ALERT, DISPENSE, WAIT_REMOVAL, CONFIRMED, STERILIZE, ERROR_STATE };
State state = IDLE;
float tare = 0;

bool doseDue() {
  DateTime now = rtc.now();
  for (byte i = 0; i < DOSES; i++)
    if (now.hour() == SCHEDULE[i].h && now.minute() == SCHEDULE[i].m) return true;
  return false;
}

void alert(bool on) { digitalWrite(PIN_BUZZER, on ? HIGH : LOW); }

void dispensePill() {
  dispenser.write(90); delay(600); dispenser.write(0);  // sweep → release pill
  delay(800);                                           // let it settle on the cup
}

void runSterilization() {
  if (digitalRead(PIN_INTERLOCK) == LOW) return;        // cover open → refuse
  digitalWrite(PIN_RELAY, HIGH);
  delay(UVC_DURATION);
  digitalWrite(PIN_RELAY, LOW);
}

void logEvent(const char* tag) {
  DateTime now = rtc.now();
  Serial.print(tag); Serial.print(",");
  Serial.print(now.timestamp()); Serial.println();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  lcd.init(); lcd.backlight();
  scale.begin(PIN_DT, PIN_SCK);
  dispenser.attach(PIN_SERVO); chute.attach(PIN_CHUTE);
  pinMode(PIN_BUZZER, OUTPUT); pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT); pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_SNOOZE, INPUT); pinMode(PIN_MANUAL, INPUT); pinMode(PIN_INTERLOCK, INPUT);
  scale.set_scale(); scale.tare(); tare = scale.get_units();  // Step 6 calibration values
  lcd.setCursor(0, 0); lcd.print("Pill Dispenser OK");
}

void loop() {
  switch (state) {
    case IDLE:
      if (doseDue() || digitalRead(PIN_MANUAL)) state = ALERT;
      break;
    case ALERT:
      alert(true); digitalWrite(PIN_LED_RED, HIGH);
      if (digitalRead(PIN_SNOOZE) || millis() > 30000) { alert(false); state = DISPENSE; }
      break;
    case DISPENSE:
      dispensePill(); state = WAIT_REMOVAL; tare = scale.get_units(); break;
    case WAIT_REMOVAL: {
      float w = scale.get_units();
      if (w <= tare + TOLERANCE) state = CONFIRMED;      // pill removed
      else if (millis() > REMOVAL_TIMEOUT) { logEvent("MISSED"); state = IDLE; }
      break;
    }
    case CONFIRMED:
      digitalWrite(PIN_LED_RED, LOW); digitalWrite(PIN_LED_GREEN, HIGH);
      logEvent("CONFIRMED"); delay(2000); digitalWrite(PIN_LED_GREEN, LOW);
      state = STERILIZE; break;
    case STERILIZE:
      runSterilization(); logEvent("STERILIZED"); state = IDLE; break;
    case ERROR_STATE:
      lcd.setCursor(0, 0); lcd.print("ERROR - RESET"); break;
  }
}
```

**What the code does (plain words):** the `loop()` is a *state machine* — the device is always in exactly one state and moves to the next when a condition is met: `IDLE` (watching the clock) → `ALERT` (buzzer + red LED) → `DISPENSE` (servo drops the pill) → `WAIT_REMOVAL` (scale watches for the pill to be picked up) → `CONFIRMED` (green LED + log) → `STERILIZE` (UVC) → back to `IDLE`. See [system-architecture.md](system-architecture.md) for the full diagram.

4. Upload: *Sketch → Upload* (or Ctrl+U). Open *Tools → Serial Monitor* and set the baud rate to **115200**.

**Check:** (a) upload finishes with no errors; (b) the LCD shows `Pill Dispenser OK`; (c) the Serial Monitor prints a line. If the LCD is blank, your I2C address may be 0x3F instead of 0x27 — see [Troubleshooting](#troubleshooting).

## Step 6 — Calibrate (1–1.5 hrs)

**Why:** the code uses numbers it doesn't know yet — the correct time, the scale's conversion factor, the servo's exact angles, and the pill's weight. This step supplies them.

### 6.1 Set the RTC time

Add this line once inside `setup()` (right after `rtc.begin();`), upload, then **delete it** and upload again:

```cpp
rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // sets time from your PC
```

**Check:** the Serial Monitor prints the correct current time. Power the Mega off and on — the time must still be correct (battery backup works).

### 6.2 Calibrate the load cell

1. Upload *File → Examples → HX711 → calibration* (the example included with the library).
2. Follow its Serial Monitor prompts: place a **known weight** (e.g., a 100 g calibration weight) on the cup, type its value, and it computes the scale factor.
3. Put that factor into the sketch: `scale.set_scale(<your factor>);` then `scale.tare();` (with the empty cup in place).

**Check:** put the known weight back on the cup — the Serial Monitor should read it within **0.5 g**.

### 6.3 Find the servo angles

Change the two angles in `dispensePill()` (currently `90` and `0`) until a full cycle releases **exactly one pill, ten times in a row**. Mark the working angles down — you'll report these in the methodology.

**Check:** 10/10 single-pill drops, no jams, no double-drops.

### 6.4 Set the weight threshold

Weigh **20 pills** of the target medication on the calibrated scale. Set `PILL_WEIGHT` to the average, and `TOLERANCE` to a little above the spread you observed (see [testing-evaluation.md](testing-evaluation.md) for the formal rule: **±0.5 g or ±5% of pill weight, whichever is larger**).

**Check:** a dispensed pill makes the reading rise past `PILL_WEIGHT − TOLERANCE`, and removing it drops the reading back to tare.

## Step 7 — Test the whole system (2–3 hrs)

**Why:** this is where the device becomes *research data*, not just a working gadget.

Follow the full protocol in [testing-evaluation.md](testing-evaluation.md):

1. **Pilot:** 5 trial runs to practice the procedure (results are not counted).
2. **Formal runs:** ≥ 30 trials per condition (e.g., per pill size). Each trial records scheduled time, alert time, dispense time, weight delta, removal time, and outcome.
3. **Data:** the Serial Monitor's `CONFIRMED,<timestamp>` lines are your machine-readable log — save them as CSV for analysis.
4. **Users:** once the technical metrics pass, run the usability questionnaires with PWDs, caregivers, and experts (Section 7 of the testing doc).

**Check:** at least 95% of trials meet the acceptance criteria in the testing doc before you call the device "working."

## Step 8 — Deploy and use (30 min)

- Place the unit where the PWD can reach the cup and see the LCD.
- Connect both adapters; confirm the RTC still holds time after a power cycle.
- Run one complete demo cycle in front of a witness: alert → dispense → remove pill → green LED → UVC → back to idle.

**Check:** the full cycle works with the witness present — this is also your documentation evidence (photo/video for the manuscript).

---

## Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| Board resets when the servo moves | Servo powered from the Mega's 5 V pin | Move servo power to the separate 5 V adapter (Rule 1) |
| HX711 reads `NaN` or jumps wildly | Loose DT/SCK wires; servo noise; missing shared GND | Re-seat wires, twist signal wires, keep servo wires away, check common ground |
| LCD is blank / shows blocks | Wrong I2C address | Run an I2C scanner sketch; change `0x27` to `0x3F` |
| Wrong time after power loss | CR2032 battery missing/dead | Replace battery, re-run Step 6.1 |
| Two pills drop at once | Hopper opening too wide / sweep too long | Narrow the opening; shorten the servo sweep |
| No pill drops (jam) | Hopper angle; agitator not triggering | Tilt the hopper; increase the chute servo tap |
| Buzzer too quiet / always on | Wrong pin; resistor too large | Check pin 6 wiring; reduce series resistor |
| Interlock ignored — UVC runs with cover open | Switch wired inverted or no pulldown | Check the 10 kΩ pulldown and switch logic |
| Device "works" but intake never confirms | TOLERANCE too tight; cup not fully on the free end | Loosen tolerance per Step 6.4; remount cup |

## Glossary (plain words)

| Term | Meaning |
|---|---|
| **Arduino Mega 2560** | A microcontroller board — the "brain" that runs the program |
| **Breadboard** | A plastic board with holes for prototyping circuits without soldering |
| **Jumper wire** | A short wire used to connect breadboard holes |
| **GND** | Ground — the 0 V reference; every circuit shares it |
| **VIN** | The Mega's power input pin (9–12 V) |
| **Resistor** | Limits current; the 220 Ω protects LEDs, the 10 kΩ pulls button pins low |
| **Pull-down resistor** | Holds a pin at 0 V until a button connects it to 5 V |
| **LED** | Light-emitting diode — lights up when current flows through it |
| **Buzzer** | A small speaker that beeps when given power |
| **Servo** | A motor that rotates to a commanded angle and holds it |
| **PWM** | Pulse-width modulation — the signal that tells a servo its angle |
| **I2C** | A two-wire bus (SDA/SCL) that lets the Mega talk to the RTC and LCD with only 2 wires |
| **LCD backpack** | A small board (PCF8574) that converts the LCD to I2C |
| **Load cell** | A metal bar that bends slightly under weight; the bend changes its electrical resistance |
| **HX711** | A chip that reads the tiny load-cell signal and sends it to the Mega (24-bit) |
| **Tare** | Zeroing the scale — "the empty cup is now 0" (pagtata-zero ng timbangan) |
| **Relay** | A switch controlled by electricity — lets a small signal turn the lamp on/off |
| **Interlock** | A safety switch that prevents the lamp from running when the cover is open |
| **UVC** | Ultraviolet-C light (254–280 nm) that kills germs |
| **RTC** | Real-time clock — keeps track of date/time, even unpowered |
| **COM port** | The virtual port the Mega appears as when plugged into a PC |
| **Serial Monitor** | The Arduino IDE window that shows text the Mega prints |
| **State machine** | A program pattern where the device is always in one defined "state" and moves between them |
| **Tolerance** | The allowed difference (e.g., ±0.5 g) before a reading counts as "changed" |
| **Calibration weight** | A weight of known mass used to tune the scale |
| **CFU** | Colony-forming unit — how bacteria are counted on a culture plate |

## Final checklist (printable)

- [ ] All parts arrived and passed their arrival tests (Step 2)
- [ ] One pill per sweep ×10 by hand (Step 3)
- [ ] Nothing touches the load-cell free end (Step 3)
- [ ] Servos + relay on the separate 5 V supply; grounds joined (Step 4)
- [ ] No shorts before first power-on (Step 4)
- [ ] Sketch uploads; LCD shows `Pill Dispenser OK` (Step 5)
- [ ] RTC keeps time across a power cycle (Step 6)
- [ ] Scale reads a known weight within 0.5 g (Step 6)
- [ ] 10/10 single-pill drops by the servo (Step 6)
- [ ] Pilot (5) + ≥ 30 formal trials pass the acceptance criteria (Step 7)
- [ ] Full demo cycle witnessed (Step 8)

## Safety notes

1. **UVC is hazardous** — the lamp must never run with the cover open (interlock). Use the UV-blocking window. Post a warning label.
2. **Servo power** — separate 5 V/2 A supply; shared GND only.
3. **Electronics** — keep mains-side wiring (if a 12 V lamp/ballast is used) insulated and away from user touch points.
4. **Medication errors** — the device is a reminder/dispensing aid for a supervised study; a caregiver must verify each dose during evaluation trials.
