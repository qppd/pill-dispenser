# Hardware Block Diagram

Shows every hardware module of the pill dispenser and the interface that connects it to the Arduino Mega 2560. The I2C bus (SDA/SCL) is shared by the RTC and the LCD; everything else uses dedicated pins.

## Mermaid diagram

```mermaid
flowchart TB
    subgraph PWR["Power Supply"]
        P1["9–12 V adapter<br/>→ Mega VIN"]
        P2["5 V / 2 A adapter<br/>→ servos + relay"]
    end

    subgraph SENS["Sensing"]
        S1["DS3231 RTC<br/>(I2C addr 0x68)"]
        S2["HX711 + load cell<br/>(24-bit ADC)"]
    end

    subgraph UI["User Interface"]
        U1["16×2 I2C LCD"]
        U2["Active buzzer"]
        U3["Red / green LEDs"]
        U4["Push buttons<br/>(snooze, manual)"]
    end

    subgraph ACT["Actuation"]
        A1["SG90 servo — dispenser"]
        A2["SG90 servo — chute/agitator"]
        A3["5 V relay → UVC lamp"]
    end

    MCU["Arduino Mega 2560<br/>(ATmega2560)"]

    P1 -- "VIN / GND" --> MCU
    P2 -- "5 V / GND" --> A1
    P2 -- "5 V / GND" --> A2
    P2 -- "5 V / GND" --> A3

    S1 -- "I2C: SDA(20) / SCL(21)" --> MCU
    S2 -- "DT(3) / SCK(2)" --> MCU
    U1 -- "I2C: SDA(20) / SCL(21)" --> MCU
    U2 -- "D6" --> MCU
    U3 -- "D7 (red) / D8 (green)" --> MCU
    U4 -- "D4 / D5" --> MCU
    A1 -- "PWM D9" --> MCU
    A2 -- "PWM D10" --> MCU
    A3 -- "D11" --> MCU
```

## ASCII fallback

```
                     +---------------------------+
  +----------+       |     ARDUINO MEGA 2560      |       +------------------+
  | DS3231   |--I2C--| 20 SDA   21 SCL            |--I2C--| 16x2 I2C LCD     |
  | RTC      |       |                           |       +------------------+
  +----------+       |                           |       +------------------+
  +----------+       |   2 SCK    3 DT            |       | Active buzzer    |
  | HX711 +  |--DT/--|                           |--D6---+------------------+
  | load cell|  SCK  |   4 btn   5 btn            |       +------------------+
  +----------+       |                           |       | LEDs D7/D8       |
  +----------+       |   6 bz    7/8 LED 9/10 PWM |       +------------------+
  | 9-12 V   |--VIN--|  11 relay                  |       +------------------+
  | adapter  |       |                           |--D11--| Relay -> UVC lamp|
  +----------+       +----+----------------+-----+       +------------------+
                          | 5 V / GND (from separate 2 A supply)
                          |                 |
                     +----+-----+     +-----+----+
                     | SG90 D9  |     | SG90 D10 |
                     | dispenser|     | chute    |
                     +----------+     +----------+
```

## Interface summary

| Interface | Pins on Mega | Modules |
|---|---|---|
| I2C | SDA = 20, SCL = 21 | DS3231 (0x68), LCD backpack (0x27) |
| HX711 two-wire | DT = 3, SCK = 2 | Load cell amplifier |
| PWM | 9, 10 | Dispenser servo, chute servo |
| Digital out | 6, 7, 8, 11 | Buzzer, red LED, green LED, relay |
| Digital in | 4, 5 | Snooze button, manual-dispense button |
| Power | VIN, 5 V, GND | 9–12 V adapter; separate 5 V/2 A for servos |
