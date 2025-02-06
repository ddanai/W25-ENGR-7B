// Define pin assignments
#include "CytronMotorDriver.h"
const int sensor1Pin = 2; // Digital pin for Sensor 1
const int sensor2Pin = 3; // Digital pin for Sensor 2

// Configure the motor driver.
CytronMD motor1(PWM_DIR, 3, 4);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor2(PWM_DIR, 9, 10); // PWM 2 = Pin 9, DIR 2 = Pin 10.


void setup() {
  // Initialize sensor pins as inputs
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
}

void loop() {
  // Read sensor states
  int sensor1State = digitalRead(sensor1Pin);
  int sensor2State = digitalRead(sensor2Pin);

  // Control logic
  if (sensor1State == HIGH && sensor2State == HIGH) {
    // Stop both motors
	  motor1.setSpeed(0);  
    motor2.setSpeed(0);     

  } else if (sensor1State == HIGH) {
    // Move Motor 2 forward
    motor2.setSpeed(255);     
    motor1.setSpeed(0);     

  } else if (sensor2State == HIGH) {
    // Move Motor 1 forward
    motor1.setSpeed(255);     
    motor2.setSpeed(0);     

  } else if (sensor1State == LOW && sensor2State == LOW) {
    // Spin motors in opposite directions
    motor2.setSpeed(255);     
    motor1.setSpeed(-255);     
  }
}


