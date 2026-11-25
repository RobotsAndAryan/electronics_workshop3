/*
 * Basic Quadrature Encoder Test (N20 2-Channel Hall Sensor)
 * No extra library needed (uses built-in Arduino interrupts)
 *
 * Connections (Arduino Uno R3):
 *   Check the labels printed on the small encoder PCB (wire colors vary!):
 *     VCC     -> 5V (or 3.3V)
 *     GND     -> GND
 *     C1 / A  -> Pin 2 (Hardware interrupt pin)
 *     C2 / B  -> Pin 3 (Direction pin)
 *     M1 / M2 -> Leave disconnected while spinning the shaft by hand
 */

const int encAPin = 2;  // Encoder Channel A (must be interrupt pin 2 or 3 on Uno)
const int encBPin = 3;  // Encoder Channel B

volatile long encoderCount = 0;  // Updated inside interrupt

// Runs automatically whenever Channel A goes from LOW to HIGH
void readEncoder() {
  // Check Channel B to determine which direction the shaft is turning
  if (digitalRead(encBPin) == HIGH) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(encAPin, INPUT_PULLUP);
  pinMode(encBPin, INPUT_PULLUP);

  // Trigger readEncoder() on every rising edge of Channel A
  attachInterrupt(digitalPinToInterrupt(encAPin), readEncoder, RISING);

  Serial.println("N20 Quadrature Encoder Test");
  Serial.println("Spin the motor shaft by hand to see counts go up and down.");
}

void loop() {
  // Briefly pause interrupts while copying the count
  noInterrupts();
  long count = encoderCount;
  interrupts();

  Serial.print("Encoder Count: ");
  Serial.println(count);

  delay(200);
}
