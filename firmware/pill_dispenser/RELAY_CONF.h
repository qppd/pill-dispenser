// RELAY_CONF.h — UVC relay configuration
//
// The relay switches the 12 V feed to the UVC LED module. The module
// never runs unless the cover interlock (BUTTON_CONF.h) is closed.
#pragma once

const byte PIN_RELAY = 27;   // relay control pin

// How long the UVC cycle runs. Must deliver ≥ 40 mJ/cm² at the cup
// (see docs/testing-evaluation.md §10). 60 s is the default.
const unsigned long UVC_DURATION = 60UL * 1000UL;   // 60 s
