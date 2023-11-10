#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo doorServo;

const int trigPin1 = 9; // First ultrasonic sensor trigger pin
const int echoPin1 = 8; // First ultrasonic sensor echo pin
const int trigPin2 = 10; // Second ultrasonic sensor trigger pin
const int echoPin2 = 11; // Second ultrasonic sensor echo pin
const int servoPin = 7;

int carCount = 0;

// Function declarations
void openDoor();
void closeDoor();
float checkDistance(int trigPin, int echoPin);
void updateDisplay();

void setup() {
   lcd.init();
  lcd.backlight();
  lcd.print("Car count: ");
  lcd.setCursor(0, 1);

  doorServo.attach(servoPin);
  closeDoor(); // Ensure the door is closed initially

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

void loop() {
  // Check if a car is approaching the entrance
  if (carCount < 5 && checkDistance(trigPin1, echoPin1) <= 10) {
    openDoor();
    delay(3000); // Adjust this delay as needed to allow time for the car to pass the door
    closeDoor();
    carCount++;
    updateDisplay();
  }

  // Check if a car is leaving the parking
  if (checkDistance(trigPin2, echoPin2) <= 10 && carCount > 0) {
    openDoor();
    delay(3000); // Adjust this delay as needed to allow time for the car to pass the door
    closeDoor();
    carCount--;
    updateDisplay();
  }
}

// Function to check the distance using an ultrasonic sensor
float checkDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

// Function to open the door using the servo motor
void openDoor() {
  doorServo.write(0); // Assuming 0 degrees is the open position
}

// Function to close the door using the servo motor
void closeDoor() {
  doorServo.write(90); // Assuming 90 degrees is the closed position
}

// Function to update the car count on the display
void updateDisplay() {
  lcd.setCursor(12, 1);
  lcd.print(carCount);
}
