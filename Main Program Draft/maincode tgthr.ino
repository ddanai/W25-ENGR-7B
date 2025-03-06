#include "CytronMotorDriver.h"
#include <Servo.h>

// Configure the motor driver.
const int sensor1Pin = 2;   // Sensor 1 input pin (left)
const int sensor2Pin = 5;   // Sensor 2 input pin (right)
CytronMD motor1(PWM_DIR, 3, 4);  // PWM 1 = Pin 3, DIR 1 = Pin 4
CytronMD motor2(PWM_DIR, 6, 7); // PWM 2 = Pin 6, DIR 2 = Pin 7

// =========== SERVOS ===========
// 1) Claw servo: open/close
// 2) Lift servo: raise/lower the claw
Servo clawServo;
Servo liftServo;

const int clawServoPin = 5; // claw servo input pin     
const int liftServoPin = 6; // lift servo input pin         

// Claw positions (in degrees)
int clawOpenPosition   = 60;   
int clawClosedPosition = 120;  

// Lift positions (in degrees)
int liftDownPosition   = 0;    
int liftUpPosition     = 90;  

int var=1;

void setup() {
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
}
void loop() {
  Switch(var){
  Case 1: //line tracking 
    linetracking()
  ;
  break;
  Case 2: //color detecting 
    colordetecting()
  ;
  break;
  Case 3 //claw actuation
    clawactuation()
  ;
}

//motor 1 (left) negative is forwards
//motor 2 (right) positive is forwards
void linetracking(){ 
  // Read sensor inputs
  int sensorLeftStatus = digitalRead(sensor1Pin);
  int sensorRightStatus = digitalRead(sensor2Pin);
  if (sensorLeftStatus == LOW && sensorRightStatus == LOW) {
    motor1.setSpeed(-200);  
    motor2.setSpeed(200);
  } else if (sensorLeftStatus == LOW && sensorRightStatus == HIGH) {
    motor1.setSpeed(-200);
    motor2.setSpeed(-200);
  } else if (sensorLeftStatus == HIGH && sensorRightStatus == LOW) {
    motor1.setSpeed(200);
    motor2.setSpeed(200);
  } else if (sensorLeftStatus == HIGH && sensorRightStatus == HIGH) {
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    var=2;
  }
}
/*
void moveForward(int speed) {
    // Because the right motor is physically reversed on the rover:
    //   - Left motor: positive = forward
    //   - Right motor: positive = *reverse* from the robot's perspective
    // To move forward, the right motor must be given a negative speed
    leftMotor.setSpeed(speed);
    rightMotor.setSpeed(-speed);
}

// Function to turn left
void turnLeft(int speed) {
    // Hint says both motors move in the same direction to turn in place.
    // Positive to the left motor = forward
    // Positive to the right motor = backward
    leftMotor.setSpeed(speed);
    rightMotor.setSpeed(speed);
}

// Function to turn right
void turnRight(int speed) {
    // Negative to the left motor = backward 
    // Negative to the right motor = forward
    leftMotor.setSpeed(-speed);
    rightMotor.setSpeed(-speed);
}

// Function to stop motors
void stopMotors() {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
}*/

