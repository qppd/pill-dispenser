// SERVO_CONF.h — MG90S servo configuration
//
// Two servos: the dispenser drops one pill per sweep, the spoon presents
// the pill to the user. Both are powered from the LM2596S 5 V rail —
// never from the Mega's 5 V pin.
#pragma once

const byte PIN_SERVO = 9;    // dispenser servo (PWM)
const byte PIN_SPOON = 10;   // spoon servo (PWM)
