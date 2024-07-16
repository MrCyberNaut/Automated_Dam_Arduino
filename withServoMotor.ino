#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,16,2);
Servo myservo;

const int trigPin = 9;
const int echoPin = 10;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  lcd.init();                     
  lcd.backlight();
  
  myservo.attach(11);  // Attaches the servo on pin 11
}

void loop() {
  long duration, distance;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;   // Convert duration to distance

  lcd.setCursor(0,0);
  lcd.print("Distance (cm): ");
  lcd.setCursor(0,1);
  lcd.print(distance);
  lcd.print(" cm    ");   // Extra spaces to clear previous values
  
  // Rotate servo based on distance
  if (distance <= 10) {
    myservo.write(45);
  } else if (distance <= 15) {
    myservo.write(40);
  } else if (distance <= 20) {
    myservo.write(30);
  } else if (distance <= 30) {
    myservo.write(20);
  } else {
    myservo.write(0);
  }

  delay(500);
}
