# Testing & Evaluation Methodology

This document turns the device into a *research* deliverable. It defines **what** is measured, **how many** trials, **who** evaluates, and **how** results are analyzed — the pieces a proposal-defense panel will ask about.

## 1. Objectives

1. Verify the device dispenses the correct dose at the scheduled time.
2. Verify the load cell reliably confirms that the pill was actually taken.
3. Measure dispensing speed and reminder effectiveness.
4. Measure usability/acceptance from the perspective of PWDs, caregivers, and experts.
5. Validate that the UVC cycle meaningfully reduces microbial load.

## 2. Research variables

| Type | Variable | Operational definition |
|---|---|---|
| Independent | Dispenser operation | Scheduled/triggered dispensing runs under controlled conditions |
| Dependent | Dispensing accuracy | % correct doses out of total scheduled doses |
| Dependent | Dispensing time | Seconds from alert trigger to pill resting in cup |
| Dependent | Weight-detection error | |measured − reference mass| per trial (g) |
| Dependent | Intake-confirmation correctness | True-positive / false-negative rates vs. a visual log |
| Dependent | Reminder-response time | Seconds from alert to pill removal |
| Dependent | Usability score | SUS / Likert score from users and experts |
| Dependent | Microbial reduction | Log reduction in CFU after the UVC cycle |

## 3. Test categories

| Category | Who runs it | Method |
|---|---|---|
| Functional | Researchers | Scripted trials; each metric recorded per trial |
| Performance | Researchers | Stopwatch + load-cell log (CSV via serial) |
| Usability / acceptance | PWDs + caregivers + experts | SUS questionnaire + modified Likert + interview |
| Sterilization | Researchers (+ lab) | Dose calculation + before/after microbial sampling |

### 3.1 Firmware milestone tests (development phase, pre-trial gate)

Before any formal trials (Section 4), the firmware must pass the **milestone tests** below — one per subsystem, in build order. They are the software half of the build checks in [implementation-guide.md](implementation-guide.md); the full milestone workflow (including the test sketches for each step) is in [firmware-guide.md §6](firmware-guide.md). Each milestone is a **hard gate**: fix a failure before moving on — never "build it in" and hope.

| # | Milestone | Test | Pass criterion |
|---|---|---|---|
| M1 | Board + IDE upload | `Blink` on LED D25 | LED blinks; upload path works |
| M2 | RTC | `rtc.now()` read over Serial | Time matches wall clock; survives a power cycle (CR2032 backup) |
| M3 | LCD (I2C) | Hello message | `Pill Dispenser OK` renders; I2C address found (0x27 or 0x3F) |
| M4 | HX711 + load cell | Live weight read | Value tracks hand pressure; no `NaN`; noise below the 0.5 g tolerance |
| M5 | Servos ×2 | Sweep test (0–180°) | Both MG90S rotate smoothly; **no board resets** (servo power on the LM2596S rail) |
| M6 | Buzzer + LEDs + buttons | Manual toggle test | Buzzer beeps, LEDs light, buttons read LOW when pressed (pull-ups) |
| M7 | Relay + interlock | Relay + cover-open test | Relay energizes **only** when the cover interlock is closed |
| M8 | Full state machine | Complete end-to-end cycle | alert → dispense → remove → `CONFIRMED` → sterilize → idle, repeated ×5 clean |

**Gate rule:** M1–M8 must all pass before the pilot runs (Section 4). Record milestone results (date + pass/fail + notes) in the project log — reviewers will ask whether the device was verified before data collection.

## 4. Trial protocol

- **Pilot:** 5 trials to shake out procedure and hardware issues (results not included in the final analysis).
- **Formal runs:** **≥ 30 trials per test condition** (the minimum for the statistical tests below). Conditions are defined by dose type / pill size; run at least 3 conditions for comparison (e.g., small, medium, large pill).
- Each trial: record scheduled time, alert time, dispense time, weight delta, removal time, and outcome (success / wrong dose / missed / error).
- Alternate evaluator and test order to control for learning effects.

## 5. Metrics and acceptance criteria

| Metric | Formula / rule | Acceptance |
|---|---|---|
| Dispensing accuracy | correct doses ÷ scheduled doses × 100% | ≥ 95% |
| Wrong-dose rate | wrong doses ÷ total doses × 100% | ≤ 2% |
| Missed-dose rate (device-caused) | missed ÷ scheduled × 100% | ≤ 5% |
| Weight-detection tolerance | |measured − reference| ≤ **0.5 g** or **±5%** of pill weight, whichever is larger | ≥ 95% of trials |
| Intake confirmation | true positive rate (pill removed & detected) | ≥ 95% |
| Dispensing time | mean of per-trial times | ≤ 10 s (define per mechanism) |
| Reminder-response time | mean of per-trial times | ≤ 5 min |
| Usability (SUS) | standard SUS scoring | ≥ 68 (acceptable) |
| UVC reduction | log₁₀(CFU before ÷ CFU after) | ≥ 2-log reduction |

> **Tolerance justification (defense-ready):** with a 1 kg load cell at gain 128, HX711 resolution is ~0.1 g. A 0.5 g / 5% window is ≥ 5× the resolution, giving margin against vibration and cup residue while still rejecting partial-pill events.

## 6. Intake-confirmation validation

The load cell only *infers* intake; validate it against ground truth:

1. During every trial, a **visual observer** (researcher or camera log) records whether the pill was actually removed.
2. Compare observer records vs. sensor decisions → confusion matrix:

| | Pill removed (observer) | Pill not removed (observer) |
|---|---|---|
| **Sensor: confirmed** | True positive | False positive |
| **Sensor: missed** | False negative | True negative |

Report true-positive rate (sensitivity) and false-negative rate; acceptance ≥ 95% sensitivity.

## 7. Evaluators

| Group | n | Role |
|---|---|---|
| Researchers | 2–3 | Run functional/performance trials, record data |
| PWDs (target users) | 5–10 | Usability/acceptance; with guardian/caregiver consent |
| Caregivers | 5–10 | Usability + willingness to supervise use |
| Experts | 3–5 | Content validation of instruments + system validation (engineer + pharmacist/nurse) |

## 8. Instruments

- **SUS (System Usability Scale)** — 10 items, 5-point Likert; benchmark ≥ 68.
- **Modified Likert questionnaire** (expert-validated): appropriateness of reminders, ease of use, perceived safety of UVC, willingness to use.
- **Trial log sheet** (paper or CSV) — one row per trial.
- **Semi-structured interview** — short debrief for PWDs/caregivers (recorded, transcribed, thematically summarized).

## 9. Statistical treatment

| Analysis | Test | Purpose |
|---|---|---|
| Descriptive | Mean, SD, percentage | Summarize accuracy, times, errors |
| Accuracy vs. threshold | One-sample t-test | Is dispensing accuracy significantly ≥ 95%? |
| Across conditions | One-way ANOVA (or paired t-test if same unit) | Do pill sizes differ in accuracy/time? |
| Usability scores | Weighted mean, SD; group comparison (t-test) | PWD vs. caregiver vs. expert ratings |
| Instrument reliability | Cronbach's α | Internal consistency of questionnaire (α ≥ 0.70) |
| Sterilization | Paired t-test on log₁₀ CFU | Significant reduction before vs. after UVC |

All tests at α = 0.05. Use SPSS or JASP (free).

## 10. UVC sterilization validation (two legs)

1. **Dose calculation (design check):** dose (mJ/cm²) = irradiance (µW/cm²) × exposure time (s) ÷ 1000. With the module's rated UV-C output (mW) and the measured distance, compute the delivered dose at the cup surface; target **≥ 40 mJ/cm²** (2-log range for common vegetative bacteria).

   > **Worked example (directional UVC LED, 60° beam):** spot area A ≈ π·(d·tan 30°)² cm² at distance d (cm); irradiance E = (P × 1000)/A (µW/cm²) for real UV-C output P (mW); dose = E × t ÷ 1000. A **50 mW** module at **5 cm** (A ≈ 26 cm²) gives E ≈ 1,900 µW/cm² → **≈ 115 mJ/cm² in 60 s** (≈ 3× the target ✓); at 10 cm it drops to ≈ 29 mJ/cm², so raise `UVC_DURATION` to ≈ 85 s. A cheap single-chip module (~10 mW) needs ≤ 2 cm mounting or ≈ 105 s at 5 cm. Rule of thumb: aim for **≥ 2× the target in the calculation** (safety factor for reflections, aging, and beam misalignment), then confirm empirically with leg 2.
2. **Empirical (the leg that convinces panels):** swab the cup surface before and after the cycle (n ≥ 10 paired samples), culture, and count CFU. Compare log₁₀ CFU before vs. after with a paired t-test; report mean log reduction. Include a control surface not exposed to UVC.

## 11. Ethical considerations (if PWDs are involved)

- Ethics review/clearance from the institution.
- Informed consent from the PWD **and** guardian/caregiver; assent for minors or those unable to consent.
- UVC safety briefing; the interlock + UV-blocking window must be demonstrated during the ethics application.
- Right to withdraw; data anonymized.

## 12. What the panel will likely ask (prepare answers)

1. Why 30 trials? → Statistical minimum for t-tests; pilot excluded.
2. How do you know the pill was taken? → Visual-log ground truth + confusion matrix (Section 6).
3. Why ±0.5 g / 5%? → Resolution-based justification (Section 5).
4. Who evaluates? → Researchers (technical) + PWDs/caregivers/experts (acceptance).
5. How is the UVC claim proven? → Dose calculation + CFU reduction data (Section 10).
6. What statistics? → One-sample t-test, ANOVA, paired t-test, weighted mean, Cronbach's α (Section 9).
