// dispenser.ino — the one-pill servo sweep.
// Uses the global Servo objects `dispenser` and `chute` defined in pill_dispenser.ino.

void dispensePill() {
  dispenser.write(90); delay(600); dispenser.write(0);  // sweep → release pill
  delay(800);                                           // let it settle on the cup
}
