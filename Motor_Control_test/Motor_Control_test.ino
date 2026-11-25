/*
 * Basic DC Motor Control Test (DRV8833 + N20 Gearmotor)
 * Library needed: AlashMotorControlLite
 *   (https://github.com/Alash-electronics/AlashMotorControlLite)
 *
 * Connections (Arduino Uno R3):
 *   DRV8833 VM / VCC  -> External 6V battery pack + (do NOT use Uno 5V)
 *   DRV8833 GND       -> Common GND (Battery -, DRV8833 GND, Uno GND)
 *   DRV8833 EEP / SLP -> Pin 4 (pulled HIGH to enable driver, or tie to 5V)
 *   DRV8833 AIN1      -> Pin 5 (~PWM)
 *   DRV8833 AIN2      -> Pin 6 (~PWM)
 *   DRV8833 AOUT1/2   -> N20 Motor M1 / M2
 */

#include "AlashMotorControlLite.h"

const int motorIn1Pin = 5;  // DRV8833 AIN1 (PWM)
const int motorIn2Pin = 6;  // DRV8833 AIN2 (PWM)
const int drvEepPin   = 4;  // DRV8833 EEP / SLP enable pin

// Create motor object in PWM_PWM mode for the DRV8833 driver
// Speed range is -100 (full reverse) to +100 (full forward)
AlashMotorControlLite motor(PWM_PWM, motorIn1Pin, motorIn2Pin);

void setup() {
  Serial.begin(115200);

  // Enable the DRV8833 motor driver
  pinMode(drvEepPin, OUTPUT);
  digitalWrite(drvEepPin, HIGH);

  motor.stop();
  Serial.println("DRV8833 Basic Motor Control Test");
  delay(1000);
}

void loop() {
  // 1. Spin forward at half speed, then full speed
  Serial.println("Forward: 50%");
  motor.setSpeed(50);
  delay(1500);

  Serial.println("Forward: 100%");
  motor.setSpeed(100);
  delay(1500);

  // 2. Active brake (stops the shaft quickly)
  Serial.println("Brake");
  motor.brake();
  delay(1000);

  // 3. Spin reverse at half speed, then full speed
  Serial.println("Reverse: -50%");
  motor.setSpeed(-50);
  delay(1500);

  Serial.println("Reverse: -100%");
  motor.setSpeed(-100);
  delay(1500);

  // 4. Coast stop (motor spins down freely)
  Serial.println("Coast stop");
  motor.stop();
  delay(2000);
}
