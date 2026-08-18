// pill_dispenser.ino — main file: includes, setup(), and the state machine.
//
// This sketch is modular on purpose: every component's pins and constants
// live in ONE small header file (the *_CONF.h files below), so changing a
// pin or a timing value never means hunting through this file.
//
//   Configuration  →  *_CONF.h headers (one per component)
//   Logic          →  this file + scheduler.ino + dispenser.ino + logger.ino
//
// The Arduino IDE compiles every .ino AND every .h in this folder as one
// program — the headers are pulled in by the #include lines below.

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <Servo.h>

// ---- Component configuration (edit these, not the code) ----
#include "DS3231_CONF.h"     // RTC address
#include "LCD_CONF.h"        // LCD address + size
#include "HX711_CONF.h"      // load cell pins
#include "SERVO_CONF.h"      // servo pins
#include "BUZZER_CONF.h"     // buzzer pin
#include "LED_CONF.h"        // status LED pins
#include "RELAY_CONF.h"      // UVC relay pin + duration
#include "BUTTON_CONF.h"     // buttons + interlock pins
#include "SCHEDULE_CONF.h"   // dose schedule
#include "WEIGHT_CONF.h"     // pill weight, tolerance, timeout

// ---- Component objects (globals shared with scheduler/dispenser/logger) ----
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS);
HX711 scale;
Servo dispenser, spoon;

// ---- Runtime state ----
enum State { IDLE, ALERT, DISPENSE, WAIT_REMOVAL, CONFIRMED, STERILIZE, ERROR_STATE };
State state = IDLE;
float tare = 0;
unsigned long alertStart = 0;    // millis() when ALERT began (timeout base)
unsigned long removalStart = 0;  // millis() when WAIT_REMOVAL began (timeout base)

void alert(bool on) { digitalWrite(PIN_BUZZER, on ? HIGH : LOW); }

void runSterilization() {
  if (digitalRead(PIN_INTERLOCK) == HIGH) return;   // cover open → refuse (closed = LOW)
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
  dispenser.attach(PIN_SERVO); spoon.attach(PIN_SPOON);
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
