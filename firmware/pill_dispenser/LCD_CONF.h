// LCD_CONF.h — 16x2 I2C LCD configuration
//
// The LCD also uses I2C (SDA = 20, SCL = 21). If the display is blank,
// your backpack may use address 0x3F instead of 0x27 — change it here.
#pragma once

#define LCD_I2C_ADDRESS 0x27   // PCF8574 backpack address (try 0x3F if blank)
#define LCD_COLS        16     // 16 columns
#define LCD_ROWS        2      // 2 rows
