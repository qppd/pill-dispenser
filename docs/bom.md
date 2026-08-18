# Bill of Materials (BOM)

Complete parts list for one working unit. Prices are **indicative Philippine pesos (PHP)**, 2025 street prices from local electronics shops (e-Gizmo, Robu, Alexan) and online (Shopee/Lazada). Actual prices vary by seller and batch. **Total ≈ ₱2,500 – ₱3,600.**

## Components

| # | Qty | Item | Specification | Role | Est. each | Est. total |
|---|---|---|---|---|---|---|
| 1 | 1 | Arduino Mega 2560 | ATmega2560 clone board (USB + barrel jack) | Main controller | ₱450 | ₱450 |
| 2 | 1 | DS3231 RTC module | I2C, ±2 ppm, CR2032 holder | Accurate timekeeping with backup | ₱120 | ₱120 |
| 3 | 1 | CR2032 battery | 3 V lithium coin cell | RTC backup power | ₱40 | ₱40 |
| 4 | 1 | 16×2 LCD + I2C backpack | PCF8574 backpack, 5 V | Status and reminder display | ₱180 | ₱180 |
| 5 | 2 | MG90S micro servo (metal gear) | 4.8–6 V, ~2.2 kg·cm, 13 g | Dispenser rotation + spoon presentation | ₱120 | ₱240 |
| 6 | 1 | HX711 module | 24-bit ADC, gain 128 | Load-cell amplifier | ₱90 | ₱90 |
| 7 | 1 | Load cell | 1 kg straight-bar (or 5 kg) | Measures pill presence/removal | ₱130 | ₱130 |
| 8 | 1 | Active buzzer | 5 V, digital | Audible reminder | ₱30 | ₱30 |
| 9 | 2 | LEDs | 5 mm red + green (buy spares) | Reminder/status indicators (D25 red, D23 green) | ₱10 | ₱20 |
| 10 | 4 | Resistors | 220 Ω (LEDs), 100 Ω (buzzer) | Current limiting (buttons use Mega's internal pull-ups) | ₱2 | ₱8 |
| 11 | 2 | Capacitors | 100 nF + 470 µF | Decoupling for servo/HX711 power | ₱10 | ₱20 |
| 12 | 1 | UVC LED module | **265–280 nm** UVC LED module (12 V DC, built-in constant-current driver; ~3 W input, ≥ 50–100 mW real UV-C output) | Sterilization of cup/dispenser | ₱450 | ₱450 |
| 13 | 1 | Relay module | 5 V, 1-channel, 10 A contacts | Switches the UVC LED module's 12 V input | ₱60 | ₱60 |
| 14 | 2 | Push buttons | Momentary, tactile | Snooze + manual dispense | ₱15 | ₱30 |
| 15 | 1 | Microswitch (interlock) | Momentary, normally-open | Cover-closed safety switch for the UVC LED module | ₱30 | ₱30 |
| 16 | 1 | Power adapter | 12 V DC, ≥ 2 A (3 A recommended) | Powers the Mega via VIN, feeds the LM2596S, and feeds the UVC LED module via the relay | ₱250 | ₱250 |
| 17 | 1 | LM2596S buck converter module | 12 V → 5 V, up to 3 A, 7-segment voltmeter | Steps 12 V down to 5 V for servos + relay coil | ₱120 | ₱120 |
| 18 | 1 | Breadboard + jumper wires | 830-point + M/M, M/F sets | Prototype wiring | ₱120 | ₱120 |
| 19 | 1 | Enclosure | Acrylic sheet / 3D print / PVC | Houses mechanics + electronics | ₱400 | ₱400 |
| 20 | 1 | UVC safety glass / shield | UV-blocking acrylic or tinted window | Lets user see the cup without UV exposure | ₱150 | ₱150 |
| 21 | 1 | Fasteners & misc | Screws, standoffs, hot glue, tape | Mounting servos, cell, LED module | ₱100 | ₱100 |
| — | — | **Total** | — | — | — | **≈ ₱3,038** |

> Range reflects seller/brand differences (e.g., original vs. clone Mega, 3D-printed vs. acrylic enclosure). Add **₱150–₱250 per servo** if upgrading to MG995/MG996R.

## Verified buying links (all items, Aug 2026)

Every item below has a **Shopee.ph** link (Lazada fallback noted where relevant). Prices/availability change — confirm in-app. For the UVC LED module (#12), message the seller for the real UV-C output in **mW** before ordering ([implementation-guide.md → Step 2](implementation-guide.md)). Ratings/orders as shown in search results on **Aug 18, 2026**.

| # | Item | Link (Shopee) | Rating / orders |
|---|---|---|---|
| 1 | Arduino Mega 2560 | [Geekcreit Mega 2560 R3 (ATmega2560-16AU)](https://shopee.ph/Mega-2560-R3-ATmega2560-16AU-Development-Board-Without-USB-Cable-Geekcreit-for-Arduino-products-that-work-with-official-Arduino-boards-i.44365765.52054520612) | 4.6★ · 20,341 |
| 2 | DS3231 RTC module | [DS3231 Precise RTC I2C AT24C32 (no battery — add #3)](https://shopee.ph/DS3231-RTC-Module-Precise-Real-Time-Clock-I2C-AT24C32-WITHOUT-BATTERY-i.542813777.25608684975) · [with-battery variant](https://shopee.ph/DS3231-RTC-Real-Time-Clock-DS1302-module-with-battery-for-Arduino-I2C-interface-i.64815518.4656026834) | 4.8★ · 2,740 |
| 3 | CR2032 battery | [NANFU CR2032 3 V](https://shopee.ph/NANFU-Battery-3V-Lithium-CR2032-i.27993050.28780915636) | 4.8★ · 25,349 |
| 4 | 16×2 LCD + I2C | [LCD1602 Blue/Green + IIC/I2C adapter (mega2560-compatible)](https://shopee.ph/LCD1602-5V-1602-module-Blue-Green-screen-16x2-Character-LCD-Display-Module-IIC-I2C-adapter-for-arduino-UNO-R3-mega2560-i.869092178.18978594134) | 5.0★ · 44 |
| 5 | MG90S servo ×2 | [ENGLAB SG90/MG90S metal-gear servo](https://shopee.ph/ENGLAB%E2%98%85Micro-Servo-Motor-SG90-MG90S-Servo-Metal-Gear-Servo-Motor-For-RC-Robot-Ship-Toy-i.1021339444.22480954645) | 4.9★ · 1,915 |
| 6–7 | HX711 + 1 kg load cell | [HX711 + straight-bar load cell combo (pick 1 kg)](https://shopee.ph/Load-Cell-Amplifier-HX711-Straight-Bar-Load-Cell-Weight-Sensor-1kg-3kg-5kg-10kg-20kg-i.18252381.314947306) | 4.9★ · 718 |
| 8 | Active buzzer 5 V | [5 V active piezo buzzer (continuous beep)](https://shopee.ph/active-buzzer-5V-piezo-buzzer-sounder-continuous-beep-good-for-arduino-and-DIY-projects-i.64815518.6686558287) | 5.0★ · 217 |
| 9 | LEDs red/green | [Primecircuits 10 pcs 5 mm assorted (red + green included)](https://shopee.ph/Primecircuits-10pcs-5mm-red-led-green-led-blue-led-yellow-led-white-led-Light-emitting-diode-i.114269794.7121504842) | 4.8★ · 507 |
| 10 | Resistors 220 Ω / 100 Ω | [600 pcs 30-value 1/4 W metal-film kit (incl. 220 Ω)](https://shopee.ph/600pcs-set-30-Kinds-1-4W-Resistance-1-Metal-Film-Resistor-Pack-Assorted-Kit-1K-10K-100K-200K-220-ohm-1M-Resistors-300pcs-set-i.981803564.23152846439) | — |
| 11 | Capacitors 100 nF + 470 µF | [Galvantronix store (470 µF 16 V + ceramic 100 nF)](https://shopee.ph/galvantronix) · or [components kit](https://shopee.ph/Electronic-components-Kits-Metal-film-Resistor-assortment-kit-diodes-electrolytic-Capacitor-Ceramic-set-transistor-Pack-i.1018859629.24637584160) | — |
| 12 | UVC LED module | [DC12–24 V UVC 270–280 nm water-purifier module (Mayige97k)](https://shopee.ph/%E3%80%90Mayige97k%E3%80%912Pcs-DC12-24V-270-280NM-Ultraviolet-Water-Purifier-Pet-Water-Dispenser-LED-Module-i.778742753.40266146390) · **Lazada fallback:** [KPOH 270–280 nm module](https://www.lazada.com.ph/tag/uv-led-water-purifier/) | 4.7★ · 1,526 |
| 13 | Relay module 5 V | [1-channel 5 V relay shield (for UNO/1280/2560)](https://shopee.ph/1-Channel-5V-Relay-Module-Shield-for-uno-1280-2560-ARM-PIC-AVR-DSP-i.116687067.44963157630) | 4.7★ · 16,564 |
| 14 | Push buttons ×2 | [100 pcs 6×6 mm tactile momentary](https://shopee.ph/100pcs-6x6mm-Panel-PCB-Momentary-Tactile-Tact-Mini-Push-Button-Switch-DIP-4pin-6x6x4.3-5-6-7.3-25-MM-6*6*4.3MM-5MM-6MM-7MM-8MM-25MM-i.331412195.9544491122) | 4.9★ · 180 |
| 15 | Microswitch (interlock) | [ONE-JANE 50 pcs micro limit switch (NC+NO)](https://shopee.ph/ONE-JANE-50Pcs-Micro-Limit-Switch-1NC-1NO-AC-1A-125V-Micro-Switch-Normal-Open-Close-Black-White-SPDT-3-Pin-Limit-Switch-for-Arduino-CYT1073-Toy-Mouse-i.50706257.46262657508) | 4.7★ · 30,826 |
| 16 | 12 V adapter ≥ 2 A | [12 V 3 A adapter, 5.5 mm barrel (5.5×2.1/2.5 mm)](https://shopee.ph/Power-Adapter-AC-DC-12V-3A-5.5mm-Adaptor-Supply-Us-Plug-for-Wifi-Router-CCTV-Camera-TV-Plus-PD1203-i.160107164.28200894204) | 5.0★ · 13 |
| 17 | LM2596S buck converter | [LM2596S 24 V/12 V → 5 V buck (HW-688 type)](https://shopee.ph/LM2596S-24V-12V-to-5V-Buck-Converter-USB-Mobile-Phone-DC-DC-Step-down-Module-LM2596S-HW-688-HCW-P715-i.682614836.22352461408) | 4.9★ · 827 |
| 18 | Breadboard + jumpers | [Solderless breadboard kit 2×830 + 2×400 + jumpers](https://shopee.ph/-UTnus9ddL-Solderless-Breadboard-Kit-2-Pcs-830-Tie-Points-2Pcs-400-Tie-Points-2-Set-65Pcs-Flexible-Breadboard-Jumper-Wires-i.371192636.49408196167) | 4.8★ · 22,171 |
| 19 | Enclosure (acrylic) | [Acrylic sheet pre-cut A4/A3 clear](https://shopee.ph/Acrylic-Sheet-Pre-Cut-Sizes-CLEAR-(A4-A3)-i.388725498.9816027905) | 4.9★ · 209 |
| 20 | UVC safety window | Use a **tinted/dark** sheet from the #19 store (choose a dark color — blocks UVC; no dedicated UV-blocking listing found, so verify by testing that UV-C doesn't pass). | — |
| 21 | Fasteners & misc | Generic — search Shopee for **"M3 standoff kit"** + **"screw assortment"**, or buy at a local hardware store. | — |

> Most links are **multi-packs** (600 pcs resistors, 100 pcs buttons, 50 pcs switches) — the spares are cheap and cover the BOM's "buy spares" advice.

## Consumables & tools (not counted in the unit cost)

| Item | Purpose |
|---|---|
| Soldering iron + solder | Relay/power joints |
| Multimeter | Continuity, voltage checks |
| Heat-shrink / electrical tape | Insulating splices |
| Calibration weights | Load-cell calibration and verification |
| Dupont connector housings | Removable wiring harness |

## Selection notes

- **Servo power is the #1 trap.** MG90S stall current can reach ~700 mA each. Two servos **must** run from the **LM2596S buck converter's 5 V output** (12 V in), sharing GND with the Mega. Running them off the Mega's 5 V regulator resets the board mid-dispense.
- **MG90S over SG90:** the MG90S uses **metal gears**, so it survives repeated dispenser sweeps and spoon movements far better than the plastic-gear SG90 (which strips teeth under load).
- **No pull-down resistors needed** — the snooze/manual buttons and the interlock switch use the Mega's built-in internal pull-ups (`INPUT_PULLUP`), so the 10 kΩ resistors are dropped from the BOM. Only the 220 Ω (LEDs) and 100 Ω (buzzer) remain.
- **LEDs:** exactly two are wired — red on D25 and green on D23. The 220 Ω resistors match that; buy a couple of spares since they are cheap.
- **LM2596S setup:** set the output to exactly **5.0 V** with its trimmer + multimeter *before* connecting any load. Rated 3 A max (add a heatsink for continuous servo use); the 7-segment display shows the output voltage for verification.
- **Load cell capacity** must exceed the tare of the cup + mechanism plus the pill weight. A 1 kg cell is right for a single-pill cup; choose 5 kg only if the mechanism is heavy.
- **UVC LED runs from the 12 V rail.** The module has a **built-in constant-current driver**, so it needs only **12 V DC** — the relay (item 13) switches that 12 V feed: 12 V → relay COM, relay NO → module **+**, module **−** → GND. No ballast, no 220 V mains, no mercury — the whole device is 12 V/5 V.
- **Buying the module:** search Lazada/Shopee for **"UVC LED module 12 V"**, **"265 nm/275 nm UV-C sterilizer module"**, or **"water purifier UVC LED"** (₱150–₱700). Verify the listing states **265–280 nm** (UV-C — a "365 nm/395 nm" listing is UV-A and will *not* sterilize) and quotes a **real UV-C output in mW** (a ~3 W module should list roughly 50–100 mW). The LED beam is directional — mount it close to the cup (2–5 cm) and aim it at the surface.
  - **Verified listings (Aug 2026 — prices/availability change, confirm in-app before ordering):**
    - **Shopee** — [12 V 275 nm 4 W UVC LED module](https://shopee.ph/1X-High-Quality-12V-275nm-4W-UVC-LED-Module-UV-Lamp-Sterilizer-UVC-Deep-Violet-Ultraviolet-Light-For-Sterilization-i.1858981735.45112537288) — ₱698
    - **Shopee** — [DC12–24 V UVC 270–280 nm water-purifier module (hottoppe.ph)](https://shopee.ph/DC12-24V-UVC-270-280NM-Ultraviolet-Water-Purifier-LED-Disinfection-Module-Water-Purifier-Module-hottoppe.ph-i.129874269.28512904317) — ~₱299
    - **Shopee** — [2 pcs DC12–24 V UVC 270–280 nm water-purifier module (Mayige97k)](https://shopee.ph/%E3%80%90Mayige97k%E3%80%912Pcs-DC12-24V-270-280NM-Ultraviolet-Water-Purifier-Pet-Water-Dispenser-LED-Module-i.778742753.40266146390) — 4.7★ (1,526 ratings)
    - **Shopee** — [DC12–24 V UVC 270–285 nm module (PH)](https://shopee.ph/DC12-24V-Uvc-uva-270-285NM-Ultraviolet-Water-Purifier-Sterilization-Pet-Water-Dispenser-Humidifier-LED-Disinfection-Module-PH-i.44808513.43711317861) — ₱279–₱456
    - **Lazada** — [(KPOH) DC12–24 V UVC 270–280 nm water-purifier module](https://www.lazada.com.ph/tag/uv-led-water-purifier/) — ₱153 (4.8★, 463 ratings); link is a search page — find the KPOH module there
    - **Lazada** — [Deep UV UVC 270/275/280 nm, DC 5/12/24 V](https://www.lazada.com.ph/tag/uv-led-12-volts/) — ₱393; search-page link, filter for the 12 V variant
  - **Preferred type:** the **DC12–24 V water-purifier module** — built-in constant-current driver, thousands of ratings, drop-in on the 12 V rail (no separate driver needed).
  - **Before ordering:** message the seller for the real UV-C output in **mW** (see [implementation-guide.md → Step 2](implementation-guide.md)). If a link expires, re-search the quoted terms.
- **UVC dose**: dose (mJ/cm²) = irradiance × time (see [testing-evaluation.md §10](testing-evaluation.md)); target **≥ 40 mJ/cm²** at the cup. With a ~100 mW module ~5 cm from the cup, the default 60 s cycle (`UVC_DURATION`) is a good starting point — measure and adjust. Always keep the module behind the UV-blocking window.
- **Mercury alternative (optional):** if much higher UV-C output is ever needed, the **Philips TUV 4W G4T5** (253.7 nm) tube + **T5 4 W electronic ballast** still works — use a **12 V DC ballast** (water-purifier type) to avoid 220 V mains; never a plain DC adapter.
- **RTC**: buy a module that includes the DS3231 (not the older DS1307) — the ±2 ppm accuracy is what makes daily scheduling reliable.
