/*
 * Basic Motor + Encoder Example (DRV8833 + N20 Hall Encoder)
 * Library needed: AlashMotorControlLite
 *   (https://github.com/Alash-electronics/AlashMotorControlLite)
 *
 * Drives the N20 motor forward and backward while printing live encoder counts
 * so you can see the count increase in one direction and decrease in the other.
 *
 * Connections (Arduino Uno R3):
 *   DRV8833 VM / VCC  -> External 6V battery pack + (do NOT use Uno 5V)
 *   DRV8833 GND       -> Common GND (Battery -, DRV8833 GND, Uno GND)
 *   DRV8833 EEP / SLP -> Pin 4
 *   DRV8833 AIN1      -> Pin 5 (~PWM)
 *   DRV8833 AIN2      -> Pin 6 (~PWM)
 *   DRV8833 AOUT1/2   -> N20 Motor M1 / M2
 *   Encoder VCC       -> 5V (or 3.3V)
 *   Encoder GND       -> GND
 *   Encoder C1 / A    -> Pin 2 (Interrupt pin)
 *   Encoder C2 / B    -> Pin 3
 */

#include "AlashMotorControlLite.h"

const int motorIn1Pin = 5;
const int motorIn2Pin = 6;
const int drvEepPin   = 4;

const int encAPin = 2;
const int encBPin = 3;

AlashMotorControlLite motor(PWM_PWM, motorIn1Pin, motorIn2Pin);

volatile long encoderCount = 0;

void readEncoder() {
  if (digitalRead(encBPin) == HIGH) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}

void printEncoderCount(const char *label) {
  noInterrupts();
  long count = encoderCount;
  interrupts();

  Serial.print(label);
  Serial.print(" | Encoder Count: ");
  Serial.println(count);
}

void setup() {
  Serial.begin(115200);

  // Enable DRV8833 driver
  pinMode(drvEepPin, OUTPUT);
  digitalWrite(drvEepPin, HIGH);
  motor.stop();

  // Setup encoder interrupt
  pinMode(encAPin, INPUT_PULLUP);
  pinMode(encBPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encAPin), readEncoder, RISING);

  Serial.println("Basic Motor + Encoder Demo");
  delay(1000);
}

void loop() {
  // 1. Drive forward for 2 seconds and print encoder count every 200 ms
  motor.setSpeed(60);
  for (int i = 0; i < 10; i++) {
    printEncoderCount("Forward (60%) ");
    delay(200);
  }

  // 2. Brake for 1 second
  motor.brake();
  printEncoderCount("Braked        ");
  delay(1000);

  // 3. Drive reverse for 2 seconds and print encoder count every 200 ms
  motor.setSpeed(-60);
  for (int i = 0; i < 10; i++) {
    printEncoderCount("Reverse (-60%)");
    delay(200);
  }

  // 4. Stop for 2 seconds
  motor.stop();
  printEncoderCount("Stopped       ");
  delay(2000);
}
