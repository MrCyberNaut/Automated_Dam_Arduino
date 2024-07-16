#include <Servo.h>

Servo servo1;
Servo servo2;

const int trigPin = 9;
const int echoPin = 10;

void setup() {
  servo1.attach(6); // Attach servo 1 to pin 6
  servo2.attach(7); // Attach servo 2 to pin 7

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  long duration, distance;

  // Trigger the ultrasonic sensor to get a distance measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration / 29.1; // Calculate distance in centimeters

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Control servo motors based on distance
  if (distance < 15) {
    servo1.write(180); // Turn servo 1 to 90 degrees
    servo2.write(0); // Turn servo 2 to 90 degrees
  } else if (distance >= 15 && distance <= 30) {
    servo1.write(0);   // Turn servo 1 to 0 degrees
    servo2.write(180); // Turn servo 2 to 180 degrees
  } else {
    servo1.write(0);   // Turn servo 1 to 0 degrees
    servo2.write(0);   // Turn servo 2 to 0 degrees
  }

  // Add a delay to control the update rate
  delay(100);
}
