// scheduler.ino — dose schedule and the "is a dose due?" check.
// Uses the global RTC object `rtc` defined in pill_dispenser.ino.

// Schedule: {hour, minute} — change to the study's dose times
struct Dose { uint8_t h, m; };
const Dose SCHEDULE[] = { {8, 0}, {13, 0}, {20, 0} };
const byte DOSES = sizeof(SCHEDULE) / sizeof(Dose);

bool doseDue() {
  DateTime now = rtc.now();
  for (byte i = 0; i < DOSES; i++)
    if (now.hour() == SCHEDULE[i].h && now.minute() == SCHEDULE[i].m) return true;
  return false;
}
