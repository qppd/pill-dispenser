// SCHEDULE_CONF.h — medication dose schedule
//
// Edit the times here — the firmware reads this list every loop.
// DOSES is computed from the list, so you never type it by hand.
#pragma once

struct Dose { uint8_t h, m; };   // one dose time: {hour, minute}

const Dose SCHEDULE[] = {
  {8, 0},    // 8:00 AM
  {13, 0},   // 1:00 PM
  {20, 0}    // 8:00 PM
};

const byte DOSES = sizeof(SCHEDULE) / sizeof(Dose);
