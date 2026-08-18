// HX711_CONF.h — HX711 + load cell configuration
//
// The HX711 amplifier uses two wires: DT (data) and SCK (clock).
// Keep these wires short and away from the servo wires.
#pragma once

const byte PIN_DT  = 31;   // HX711 data pin
const byte PIN_SCK = 29;   // HX711 clock pin
