# Tutorial — Building the Automated Pill Dispenser

A step-by-step guide from empty bench to deployed, tested device. Follows the study workflow: **materials gathering → hardware development → assembly → mechanical adjustment → coding → software development → deployment and evaluation**.

**Prerequisite reading:** [system-architecture.md](system-architecture.md) and [block-diagram.md](block-diagram.md) — know what you are building before you build it.

---

## Step 0 — Tools and workspace

- Soldering iron + solder, multimeter, wire strippers, screwdriver set
- Hot glue gun / epoxy, zip ties, standoffs
- A clean, static-safe workbench with the two power adapters nearby (unplugged until Step 3)

## Step 1 — Materials gathering

Buy everything in [bom.md](bom.md). Check each item on arrival:

- [ ] Mega 2560 boots and enumerates as a COM port
- [ ] DS3231 module includes the CR2032 battery holder and battery
- [ ] Both SG90 servos rotate freely with a tester or `Servo` sweep sketch
- [ ] HX711 + load cell respond on a known-good test sketch
- [ ] UVC lamp module has a documented wattage (needed for dose calculation)

## Step 2 — Mechanical assembly

1. **Enclosure** — cut/print the housing with compartments: dispenser hopper, dispensing cup, electronics bay, and a windowed UVC chamber.
2. **Dispenser mechanism** — mount the dispenser servo so its horn rotates a blocker plate that releases exactly one pill per sweep. Test with real pills: the plate geometry must prevent double-drops.
3. **Chute agitator** — mount the second servo to vibrate/tap the hopper when a pill jams.
4. **Load cell** — fix one end of the bar to the enclosure and mount the cup on the free end (a small platform). Nothing else may touch the free end — that is the #1 cause of noisy readings.
5. **UVC lamp** — place it so the beam hits the cup/dispenser area, behind the UV-blocking window. Add the **interlock switch** (microswitch on the cover) wired to a free digital pin.
6. **Cable routing** — keep servo and power wires away from the load cell and the HX711 leads (motor noise corrupts the readings). Twist the HX711 signal wires.

## Step 3 — Electrical wiring

Follow the [block-diagram.md](block-diagram.md) pin map. Summary:

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

> ⚠️ **Never** power the servos or relay from the Mega's 5 V pin. The regulator cannot supply their inrush current.

## Step 4 — Firmware

1. Install **Arduino IDE 2.x** (or PlatformIO).
2. Install libraries via the Library Manager: `RTClib` (Adafruit), `LiquidCrystal_I2C`, `HX711` (bogde). `Servo` and `Wire` are built in.
3. Create a sketch with the modules from [system-architecture.md](system-architecture.md). Minimal skeleton:

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

// Pins (see Step 3)
const byte PIN_DT = 3, PIN_SCK = 2;
const byte PIN_SERVO = 9, PIN_CHUTE = 10;
const byte PIN_BUZZER = 6;
const byte PIN_LED_RED = 7, PIN_LED_GREEN = 8;
const byte PIN_RELAY = 11;
const byte PIN_SNOOZE = 4, PIN_MANUAL = 5, PIN_INTERLOCK = 12;

// Schedule: {hour, minute}
struct Dose { uint8_t h, m; };
const Dose SCHEDULE[] = { {8, 0}, {13, 0}, {20, 0} };
const byte DOSES = sizeof(SCHEDULE) / sizeof(Dose);

const float PILL_WEIGHT = 0.8;   // g — set after calibration (Step 5)
const float TOLERANCE = 0.5;     // g — acceptance window
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
  scale.set_scale(); scale.tare(); tare = scale.get_units();  // Step 5 calibration values
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

4. Compile and upload. The Serial Monitor at 115200 prints the machine-readable trial log (`CONFIRMED,<timestamp>` lines) used by [testing-evaluation.md](testing-evaluation.md).

## Step 5 — Calibration

1. **RTC** — set once via a `rtc.adjust(DateTime(F(__DATE__), F(__TIME__)))` line, then remove it. Verify with the Serial Monitor.
2. **Load cell** — with the empty cup on the cell, run the HX711 calibration example: record `scale` for a known calibration weight, then `scale.set_scale(value); scale.tare();`. Confirm ~0.1 g resolution.
3. **Servo angles** — find the sweep angles that release exactly one pill with no double-drop; hard-code them in `dispensePill()`.
4. **Weight threshold** — weigh 20 pills of the target medication; set `PILL_WEIGHT` to the mean and `TOLERANCE` to the observed spread (see testing doc for the formal tolerance rule).

## Step 6 — Functional testing

Run the formal protocol in [testing-evaluation.md](testing-evaluation.md): pilot (5 trials), then ≥ 30 trials per condition, recording each metric on the trial log sheet.

## Step 7 — Deployment

- Place the unit where the PWD can reach the cup and see the LCD.
- Connect both adapters; confirm the RTC holds time across power cycles.
- Demonstrate one full cycle: alert → dispense → remove pill → green LED → UVC → idle.

## Safety notes

1. **UVC is hazardous** — the lamp must never run with the cover open (interlock). Use the UV-blocking window. Post a warning label.
2. **Servo power** — separate 5 V/2 A supply; shared GND only.
3. **Electronics** — keep mains-side wiring (if a 12 V lamp/ballast is used) insulated and away from user touch points.
4. **Medication errors** — the device is a reminder/dispensing aid for a supervised study; a caregiver must verify each dose during evaluation trials.
