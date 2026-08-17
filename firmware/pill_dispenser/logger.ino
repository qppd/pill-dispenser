// logger.ino — timestamped CSV events over Serial (one row per event).
// Uses the global RTC object `rtc` defined in pill_dispenser.ino.

void logEvent(const char* tag) {
  DateTime now = rtc.now();
  Serial.print(tag); Serial.print(",");
  Serial.print(now.timestamp()); Serial.println();
}
