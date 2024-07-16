#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
Servo myservo2;

const int trigPin = 9;
const int echoPin = 10;
const int irPin = 3;
const int waterLevelPin = A0;
volatile int rotations = 0;
float volume = 0.0;
int waterLevelPercentage = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irPin, INPUT);
  pinMode(waterLevelPin, INPUT);

  lcd.init();
  lcd.backlight();

  myservo.attach(6);  // Attach servo to pin 6 (or any other available PWM pin)
  myservo2.attach(12); // Attach the second servo to another PWM pin

  attachInterrupt(digitalPinToInterrupt(irPin), countRotation, FALLING);
  Serial.begin(9600);
}

void loop() {
  long duration, distance;

  // Measure water level from the analog pin
  int waterLevel = analogRead(waterLevelPin);

  // Ultrasonic sensor distance measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  // Calculate water level percentage (100% at 2cm, 0% at 16cm)
  waterLevelPercentage = map(distance, 16, 2, 0, 100);


  // Control the servo based on distance
  if (distance <= 3) {
    myservo.write(180); // Angle for closer distance
  } else if (distance <= 4) {
    myservo.write(150); // Angle for a moderate distance
  }else if (distance <= 6) {
    myservo.write(90);
    myservo2.write(135); // Angle for a moderate distance
  }
   else {
    myservo.write(90);
    myservo2.write(0); // Angle for further distance
  }

  // Control the second servo based on water level
  
  // Calculate volume based on rotations (10 rotations = 0.5 liters)
  volume = 0.5 * rotations / 10;

  
  // Print the data to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.print("Water Level Percentage: ");
  Serial.print(waterLevelPercentage);
  Serial.println("%");
  Serial.print("Volume: ");
  Serial.print(volume, 1);
  Serial.println(" liters");
  Serial.print("Rotations: ");
  Serial.println(rotations);

  // Send water level and volume data to the ESP32
  Serial.print("WaterLevel:");
  Serial.print(waterLevelPercentage);
  Serial.print(",Volume:");
  Serial.println(volume, 1);

  // Display data on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Water Level: ");
  lcd.print(waterLevelPercentage);
  lcd.print("%  ");

  lcd.setCursor(0, 1);
  lcd.print("Volume: ");
  lcd.print(volume, 1);
  lcd.print(" liters   ");

  delay(1500);
}

void countRotation() {
  rotations++;
}
