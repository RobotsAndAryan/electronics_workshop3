/*
 * Basic Micro Servo Test (SG90 / MG90S)
 * Library needed: Servo (built into the Arduino IDE)
 *
 * Connections (Arduino Uno R3):
 *   Signal (Orange / Yellow) -> Pin 9
 *   VCC    (Red)             -> 5V (single unloaded SG90 OK on Uno 5V;
 *                               use external 5V + common GND under load)
 *   GND    (Brown / Black)   -> GND
 *
 * Note: On Arduino Uno, <Servo.h> uses Timer1, which disables analogWrite()
 * PWM on pins 9 and 10. Motor A uses pins 5 & 6 so there is no conflict.
 */

#include <Servo.h>

const int servoPin = 9;

Servo myServo;

void setup() {
  Serial.begin(115200);
  myServo.attach(servoPin);

  Serial.println("SG90 / MG90S Micro Servo Test");
  Serial.println("Centering at 90 deg...");
  myServo.write(90);
  delay(1000);
}

void loop() {
  // 1. Move to fixed angles: 0 -> 90 -> 180 degrees
  Serial.println("Angle: 0 deg");
  myServo.write(0);
  delay(1000);

  Serial.println("Angle: 90 deg");
  myServo.write(90);
  delay(1000);

  Serial.println("Angle: 180 deg");
  myServo.write(180);
  delay(1000);

  // 2. Smoothly sweep back from 180 to 0 degrees
  Serial.println("Sweeping 180 -> 0 deg...");
  for (int angle = 180; angle >= 0; angle -= 2) {
    myServo.write(angle);
    delay(20);
  }
  delay(500);

  // 3. Smoothly sweep from 0 to 180 degrees
  Serial.println("Sweeping 0 -> 180 deg...");
  for (int angle = 0; angle <= 180; angle += 2) {
    myServo.write(angle);
    delay(20);
  }
  delay(500);
}
