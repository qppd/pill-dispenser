# Technology Stack

The complete stack of the automated pill dispenser, from the physical hardware up to the evaluation tools.

## Hardware stack

| Layer | Component | Spec / version | Role |
|---|---|---|---|
| Controller | Arduino Mega 2560 | ATmega2560, 5 V, 16 MHz, 54 DIO / 16 AIO | Runs the firmware, drives all modules |
| Clock | DS3231 RTC module | I2C addr 0x68, ±2 ppm, CR2032 backup | Keeps accurate time across power loss |
| Display | 16×2 LCD + I2C backpack | PCF8574, I2C addr 0x27 | Shows schedule, alerts, and status |
| Dispensing | SG90 servo ×2 | 4.8–6 V, ~1.8 kg·cm torque | Rotates the dispenser; clears the chute |
| Sensing | HX711 + load cell | 24-bit ADC, gain 128, ~0.1 g resolution | Detects dispensed pill and its removal |
| Alert | Active buzzer | 5 V, digital drive | Audible reminder |
| Alert | LEDs (red/green) | 5 mm, with 220 Ω resistors | Visual reminder / status |
| Input | Push buttons ×2 | Momentary | Snooze, manual dispense |
| Sterilization | UVC lamp | 254 nm (mercury) or 260–280 nm (LED) | Kills pathogens on the cup/dispenser |
| Switching | 5 V relay module | 1-channel, active-low or active-high | Switches the UVC lamp safely |
| Power | 9–12 V adapter | ≥ 1 A | Powers the Mega (VIN) |
| Power | 5 V adapter | ≥ 2 A, shared GND | Powers servos + relay (never from Mega regulator) |
| Enclosure | Acrylic / 3D-printed / PVC | Custom | Houses mechanics and electronics |

## Firmware stack

| Component | Library | Purpose |
|---|---|---|
| Language | Arduino C++ (C++11) | Firmware implementation |
| IDE | Arduino IDE 2.x (or PlatformIO) | Compile + upload to Mega |
| RTC | `RTClib` (Adafruit) | DS3231 read/set, alarm helpers |
| Display | `LiquidCrystal_I2C` (F. Malpartida / marcoschwartz fork) | I2C LCD control |
| Weight | `HX711` (bogde) | Read + calibrate the load cell |
| Servo | `Servo` (built-in) | PWM servo control |
| I2C | `Wire` (built-in) | I2C bus for RTC + LCD |

## Development & simulation tools

| Tool | Use |
|---|---|
| Fritzing / KiCad | Wiring diagrams and PCB layout |
| Tinkercad Circuits | Breadboard simulation before physical build |
| Multimeter | Continuity and voltage checks during assembly |
| Soldering station | Permanent joints for the power and relay circuits |
| Serial Monitor (115200 baud) | Debug logging and intake logs |

## Evaluation stack

| Tool | Use |
|---|---|
| Calibration weights (set) | Verify load cell accuracy across the trial range |
| Stopwatch / timestamp log | Measure dispensing time and reminder-response time |
| Trial log sheet (paper/CSV) | Record pass/fail per trial for every metric |
| Excel / Google Sheets | Descriptive stats: mean, SD, percentage accuracy |
| SPSS / JASP (free) | Inferential stats: t-tests, ANOVA, Cronbach's α |
| SUS questionnaire + Likert instrument | Usability/acceptance scoring by PWDs, caregivers, experts |
| Microbiological swabs + culture | Before/after CFU counts for UVC validation |

## Optional / upgrade path

| Component | Upgrade | Why |
|---|---|---|
| SG90 servo | MG995 / MG996R (metal gear) | Higher torque (~10–13 kg·cm) if jams occur |
| Load cell 1 kg | 5 kg cell | If dispenser mechanism adds significant tare mass |
| Wired logging | DS3231 + SD card module / RTC with EEPROM | Autonomous data logging without a PC |
| Local logging only | HC-05 Bluetooth / ESP-01 Wi-Fi module | Remote caregiver alerts and cloud logging (future work) |
