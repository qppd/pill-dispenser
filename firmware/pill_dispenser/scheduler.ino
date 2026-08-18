// scheduler.ino — the "is a dose due?" check.
//
// The dose list itself lives in SCHEDULE_CONF.h — edit the times there.
// Uses the global RTC object `rtc` and the schedule constants `SCHEDULE`
// / `DOSES` defined via the headers included in pill_dispenser.ino.

bool doseDue() {
  DateTime now = rtc.now();
  for (byte i = 0; i < DOSES; i++)
    if (now.hour() == SCHEDULE[i].h && now.minute() == SCHEDULE[i].m) return true;
  return false;
}
