// WEIGHT_CONF.h — intake-detection constants
//
// Set these after calibration (implementation-guide.md Step 6).
// The formal tolerance rule: ±0.5 g or ±5% of pill weight, whichever is larger.
#pragma once

const float PILL_WEIGHT = 0.8;   // g — average weight of one pill
const float TOLERANCE = 0.5;     // g — how close to tare counts as "removed"

const unsigned long REMOVAL_TIMEOUT = 5UL * 60UL * 1000UL;   // 5 min
