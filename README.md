# Motor, Encoder & Servo Testing Guide (Arduino Uno)

Notes, pinouts, and basic example sketches for testing the workshop actuators and encoders:

| # | Component | Description | Datasheet / Technical Reference |
| :--- | :--- | :--- | :--- |
| 1 | **DRV8833 Motor Driver** | Dual H-bridge DC motor driver (`2.7V–10.8V`) | [TI DRV8833 Datasheet (PDF)](https://www.ti.com/lit/ds/symlink/drv8833.pdf) |
| 2 | **N20 DC Gearmotor** | Miniature `6V` metal-geared DC motor | [Pololu Micro Metal Gearmotors](https://www.pololu.com/category/60/micro-metal-gearmotors) |
| 3 | **N20 Hall Quadrature Encoder** | 2-channel magnetic encoder (`C1`/`A`, `C2`/`B`) | [Pololu Magnetic Encoder Kit (12 CPR)](https://www.pololu.com/product/3081) |
| 4 | **SG90 / MG90S Micro Servo** | 9g `180°` micro servo (`4.8V–6.0V`) | [TowerPro SG90 Datasheet (PDF)](https://www.friendlywire.com/projects/ne555-servo-safe/SG90-datasheet.pdf) / [MG90S Datasheet (PDF)](https://www.electronicoscaldas.com/datasheet/MG90S_Tower-Pro.pdf) |

---

## Setup Notes

- **Microcontroller:** All sketches are written for the **Arduino Uno R3** ([Arduino Uno R3 Datasheet (PDF)](https://docs.arduino.cc/resources/datasheets/A000066-datasheet.pdf)).
- **Installing libraries:**
  - **`AlashMotorControlLite`** (needed for sketches `1` and `3`): In the Arduino IDE, go to `Sketch` -> `Include Library` -> `Manage Libraries...`, search for `AlashMotorControlLite`, and click Install. If it does not appear in Library Manager, download the ZIP from [github.com/Alash-electronics/AlashMotorControlLite](https://github.com/Alash-electronics/AlashMotorControlLite) and install via `Sketch` -> `Include Library` -> `Add .ZIP Library...`.
  - **`Servo`** (needed for sketch `4`): Built into the Arduino IDE ([Arduino Servo Reference](https://docs.arduino.cc/libraries/servo/)).
- **Serial Monitor:** All sketches use **`115200` baud**. Open `Tools` -> `Serial Monitor` (`Ctrl+Shift+M`) and set the baud rate dropdown in the bottom-right corner to `115200` (if left on `9600`, the text will look scrambled).
- **IMPORTANT — Power & Common Ground:**
  - **Do not power DC motors from the Arduino `5V` pin.** Power the DRV8833 `VM` / `VCC` pin from an external `6V` battery pack (or bench supply).
  - Connect **Battery `-`**, **DRV8833 `GND`**, **Encoder `GND`**, **Servo `GND`**, and **Arduino `GND`** together so everything shares a common ground.
- **No Pin Conflicts:** All four sketches use the same pin layout (`2, 3` = Encoder, `4, 5, 6` = DRV8833 Channel A, `9` = Micro Servo), so you can leave everything wired on one breadboard while trying each example. *(Note: On Arduino Uno, `<Servo.h>` uses Timer1 and disables `analogWrite()` PWM on Pins `9` and `10`. Keeping Motor A on Pins `5` and `6` avoids this conflict.)*

---

## 1. DC Motor Control (DRV8833 + N20 Gearmotor)

Drives a DC motor forward and backward at different speeds (`-100` to `100`) using the DRV8833 H-bridge in `PWM_PWM` mode, and demonstrates both **active brake** (`motor.brake()`) and **coast stop** (`motor.stop()`).

**Datasheets:** [TI DRV8833 Datasheet (PDF)](https://www.ti.com/lit/ds/symlink/drv8833.pdf) | [Pololu Micro Metal Gearmotors](https://www.pololu.com/category/60/micro-metal-gearmotors)

**Library needed:** `AlashMotorControlLite`

**Wiring:**

| DRV8833 Pin | Arduino Uno / Power | Notes |
| :--- | :--- | :--- |
| `VM` / `VCC` | **Battery `+` (`6V`)** | Motor power (`2.7V–10.8V`); **do NOT use Uno `5V`** |
| `GND` | Common `GND` | Share ground with Battery `-` and Arduino `GND` |
| `EEP` / `SLP` | Pin `4` (or `5V`) | Pulled `HIGH` in code to enable the driver *(may be labelled `nSLEEP` or `STBY`)* |
| `AIN1` | Pin `~5` | Motor A PWM input 1 |
| `AIN2` | Pin `~6` | Motor A PWM input 2 |
| `AOUT1`, `AOUT2` | N20 Motor `M1`, `M2` | Motor wires (swap to reverse direction) |

**Sketch:** [`Motor_Control_test/Motor_Control_test.ino`](Motor_Control_test/Motor_Control_test.ino)
Upload and open Serial Monitor at `115200` baud. The motor cycles through forward (`50%`, `100%`), active brake, reverse (`-50%`, `-100%`), and coast stop.

---

## 2. Quadrature Magnetic Encoder (N20 Hall Sensor)

Reads a 2-channel Hall-effect encoder mounted on the back of the N20 motor. Channel A (`C1`) triggers an Arduino hardware interrupt on each rising edge, and checking Channel B (`C2`) tells you whether the shaft is spinning forward (`count++`) or backward (`count--`).

**Datasheet / Guide:** [Pololu Magnetic Encoder Kit (12 CPR)](https://www.pololu.com/product/3081)

**Library needed:** None (uses built-in Arduino `attachInterrupt()`)

**Wiring:**
*(Note: Wire colors on 6-pin N20 encoder cables vary between suppliers—always check the labels printed on the small encoder PCB: `M1`, `M2`, `GND`, `VCC`, `C1`/`A`, `C2`/`B`.)*

| Encoder PCB Pin | Arduino Uno | Notes |
| :--- | :--- | :--- |
| `VCC` | `5V` (or `3.3V`) | Power for Hall sensors |
| `GND` | `GND` | Ground |
| `C1` (`OUT A`) | **Pin `2`** | Hardware interrupt pin (uses `INPUT_PULLUP` in code) |
| `C2` (`OUT B`) | **Pin `3`** | Direction pin (uses `INPUT_PULLUP` in code) |
| `M1`, `M2` | Not connected *(or DRV8833 `AOUT1`/`2`)* | Leave unconnected when spinning by hand |

**Sketch:** [`Encoder_test/Encoder_test.ino`](Encoder_test/Encoder_test.ino)
Upload, open Serial Monitor at `115200` baud, and spin the motor shaft by hand. Watch `Encoder Count` count up in one direction and down in the opposite direction.

---

## 3. Basic Motor + Encoder Example

Combines **Section 1** and **Section 2**: spins the N20 motor forward for 2 seconds, brakes, then spins in reverse for 2 seconds while printing the live `Encoder Count` every 200 ms.

**Library needed:** `AlashMotorControlLite`

**Wiring:** Uses the exact combined wiring from **Section 1** and **Section 2** (no pin changes needed).

**Sketch:** [`Motor_Encoder_demo/Motor_Encoder_demo.ino`](Motor_Encoder_demo/Motor_Encoder_demo.ino)
Upload and open Serial Monitor at `115200` baud to verify that your motor and encoder work together and that counts change cleanly in both directions.

---

## 4. Micro Servo Motor (SG90 / MG90S)

Controls a standard 9g `180°` micro servo using the built-in `<Servo.h>` library (`myServo.write(angle)` sends a `50 Hz` control pulse corresponding to `0°–180°`).

**Datasheets:** [TowerPro SG90 Datasheet (PDF)](https://www.friendlywire.com/projects/ne555-servo-safe/SG90-datasheet.pdf) | [TowerPro MG90S Datasheet (PDF)](https://www.electronicoscaldas.com/datasheet/MG90S_Tower-Pro.pdf)

**Library needed:** `Servo` (built into Arduino IDE)

**Wiring:**

| Servo Wire | Signal | Arduino Uno | Notes |
| :--- | :--- | :--- | :--- |
| **Orange** *(or Yellow)* | Signal | **Pin `9`** | Servo control pin |
| **Red** | `VCC` (`4.8V–6.0V`) | `5V` *(or external `5V`)* | One unloaded SG90 can test on Uno `5V`; use external `5V` under load |
| **Brown** *(or Black)* | `GND` | `GND` | Common ground |

**Sketch:** [`Micro_Servo_test/Micro_Servo_test.ino`](Micro_Servo_test/Micro_Servo_test.ino)
Upload and open Serial Monitor at `115200` baud. Centers the servo at `90°` on startup (handy for aligning the horn), steps through `0°`, `90°`, and `180°`, then smoothly sweeps back and forth.

---

## Common Issues

- **Garbled text in Serial Monitor:** Change the Serial Monitor baud rate dropdown to **`115200`**.
- **DC motor does not spin:** Check that `EEP` / `SLP` is connected to **Pin `4`** (or `5V`), that `VM` is powered from the battery pack (not Uno `5V`), and that Battery `-`, DRV8833 `GND`, and Arduino `GND` are connected together.
- **DC motor spins the wrong way:** Swap the `M1` and `M2` motor wires on `AOUT1` and `AOUT2`.
- **Encoder count stays `0` or does not change direction:** Check that `C1` (`A`) is on **Pin `2`** and `C2` (`B`) is on **Pin `3`**, and verify the silkscreen labels on the encoder PCB rather than trusting wire colors.
- **Arduino Uno resets when the motor or servo moves:** Too much current is being drawn from USB / Uno `5V`. Power the DRV8833 `VM` (and any loaded servos) from an external `5V–6V` supply with a shared `GND`.
- **Servo buzzes or gets warm at `0°` or `180°`:** Some micro servos hit their mechanical stop right near the limits—try changing `0` and `180` to `10` and `170` in `Micro_Servo_test.ino`.
- **Second motor on Pins `9` and `10` stops working with `<Servo.h>`:** `<Servo.h>` uses Timer1 on the Uno, which disables `analogWrite()` on Pins `9` and `10`. Keep Motor A on Pins `5` and `6`.
