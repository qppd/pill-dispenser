// pill_dispenser.ino — main file: pins, setup(), and the state machine.
// Other modules in this folder: scheduler.ino, dispenser.ino, logger.ino.
// The Arduino IDE compiles every .ino in this folder as one program.

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <Servo.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 scale;
Servo dispenser, chute;

// Pins (see implementation-guide.md Step 4)
const byte PIN_DT = 3, PIN_SCK = 2;
const byte PIN_SERVO = 9, PIN_CHUTE = 10;
const byte PIN_BUZZER = 6;
const byte PIN_LED_RED = 7, PIN_LED_GREEN = 8;
const byte PIN_RELAY = 11;
const byte PIN_SNOOZE = 4, PIN_MANUAL = 5, PIN_INTERLOCK = 12;

// Tunable constants — set after calibration (Step 6) ←
const float PILL_WEIGHT = 0.8;   // g
const float TOLERANCE = 0.5;     // g
const unsigned long REMOVAL_TIMEOUT = 5UL * 60UL * 1000UL; // 5 min
const unsigned long UVC_DURATION = 60UL * 1000UL;          // 60 s

// Runtime state
enum State { IDLE, ALERT, DISPENSE, WAIT_REMOVAL, CONFIRMED, STERILIZE, ERROR_STATE };
State state = IDLE;
float tare = 0;
unsigned long alertStart = 0;    // millis() when ALERT began (timeout base)
unsigned long removalStart = 0;  // millis() when WAIT_REMOVAL began (timeout base)

void alert(bool on) { digitalWrite(PIN_BUZZER, on ? HIGH : LOW); }

void runSterilization() {
  if (digitalRead(PIN_INTERLOCK) == HIGH) return;       // cover open → refuse (closed = LOW)
  digitalWrite(PIN_RELAY, HIGH);
  delay(UVC_DURATION);
  digitalWrite(PIN_RELAY, LOW);
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
  pinMode(PIN_SNOOZE, INPUT_PULLUP); pinMode(PIN_MANUAL, INPUT_PULLUP); pinMode(PIN_INTERLOCK, INPUT_PULLUP);
  scale.set_scale(); scale.tare(); tare = scale.get_units();  // Step 6 calibration values
  lcd.setCursor(0, 0); lcd.print("Pill Dispenser OK");
}

void loop() {
  switch (state) {
    case IDLE:
      if (doseDue() || !digitalRead(PIN_MANUAL)) { alertStart = millis(); state = ALERT; }  // pressed = LOW
      break;
    case ALERT:
      alert(true); digitalWrite(PIN_LED_RED, HIGH);
      if (!digitalRead(PIN_SNOOZE) || millis() - alertStart > 30000) { alert(false); state = DISPENSE; }  // pressed = LOW
      break;
    case DISPENSE:
      dispensePill(); removalStart = millis(); state = WAIT_REMOVAL; tare = scale.get_units(); break;
    case WAIT_REMOVAL: {
      float w = scale.get_units();
      if (w <= tare + TOLERANCE) state = CONFIRMED;      // pill removed
      else if (millis() - removalStart > REMOVAL_TIMEOUT) { logEvent("MISSED"); state = IDLE; }
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
