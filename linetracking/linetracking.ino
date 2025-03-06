// Include libraries needed for this project
#include <Pixy2.h>  // Allows communication with the Pixy2 camera
#include <Servo.h>  // Controls the servo motor for the claw
#include <CytronMotorDriver.h>  // Controls the left and right drive motors

// Configure the motor driver.
const int sensor1Pin = 2;   // Sensor 1 input pin (left)
const int sensor2Pin = 5;   // Sensor 2 input pin (right)
CytronMD leftMotor(PWM_DIR, 3, 4);  // PWM 1 = Pin 3, DIR 1 = Pin 4
CytronMD rightMotor(PWM_DIR, 6, 7); // PWM 2 = Pin 6, DIR 2 = Pin 7

// =========== SERVOS ===========
// 1) Claw servo: open/close
// 2) Lift servo: raise/lower the claw
Servo clawServo;
Servo liftServo;

//Constants for line tracking/lifting
const int clawServoPin = 10; // claw servo input pin     
const int liftServoPin = 9; // lift servo input pin

// Constants for color tracking
const int CENTER_X = 150;     // The x-position that represents the center of the camera's view
const int TARGET_WIDTH = 115; // The object's width when it is close enough to grab
const float Kp = 1;         // Proportional gain for turning control (used for feedback)
const int MAX_SPEED = 255;    // Maximum motor speed
const int MIN_SPEED = 50;     // Minimum motor speed (ensures the robot moves instead of stalling)
const int FORWARD_SPEED = 255; // Speed for moving forward when approaching the object

// Claw positions (in degrees)
int clawOpenPosition   = 145;   
int clawClosedPosition = 35;  

// Lift positions (in degrees)
int liftDownPosition   = 0;    
int liftUpPosition     = 45; 

//sweep position
int pos;

int var=1;

void setup() {
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
}

void loop() {
linetracking();
}

//motor 1 (left) negative is forwards
//motor 2 (right) positive is forwards
void linetracking(){ 
  // Read sensor inputs
  int sensorLeftStatus = digitalRead(sensor1Pin);
  int sensorRightStatus = digitalRead(sensor2Pin);
  if (sensorLeftStatus == LOW && sensorRightStatus == LOW) {
    leftMotor.setSpeed(-200);  
    rightMotor.setSpeed(200);
  } else if (sensorLeftStatus == LOW && sensorRightStatus == HIGH) {
    leftMotor.setSpeed(-200);
    rightMotor.setSpeed(-200);
  } else if (sensorLeftStatus == HIGH && sensorRightStatus == LOW) {
    leftMotor.setSpeed(200);
    rightMotor.setSpeed(200);
  } else if (sensorLeftStatus == HIGH && sensorRightStatus == HIGH) {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
    var=2;
  }
}


