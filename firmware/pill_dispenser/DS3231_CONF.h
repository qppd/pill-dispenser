// DS3231_CONF.h — DS3231 RTC configuration
//
// The RTC talks to the Arduino over the I2C bus (SDA = 20, SCL = 21),
// so it needs NO extra pins — only this address for reference.
#pragma once

#define RTC_I2C_ADDRESS 0x68   // DS3231 default I2C address (for reference)
