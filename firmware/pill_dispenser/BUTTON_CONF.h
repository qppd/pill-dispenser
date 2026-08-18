// BUTTON_CONF.h — buttons + interlock configuration
//
// All three use the Mega's internal pull-ups (INPUT_PULLUP), so each
// connects one leg to its pin and the other leg to GND — no resistors.
// Pressed = LOW, interlock closed = LOW.
#pragma once

const byte PIN_SNOOZE    = 4;    // snooze button (pressed = LOW)
const byte PIN_MANUAL    = 5;    // manual-dispense button (pressed = LOW)
const byte PIN_INTERLOCK = 12;   // cover interlock microswitch (closed = LOW)
