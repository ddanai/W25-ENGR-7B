#include "CytronMotorDriver.h"
#include <Servo.h>
#include <Pixy2.h>  // Allows communication with the Pixy2 camera

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

// Create objects for Pixy2 camera 
Pixy2 pixy;


const int clawServoPin = 5; // claw servo input pin     
const int liftServoPin = 6; // lift servo input pin         

int clawOpenPosition   = 145;   
int clawClosedPosition = 35;  
// Lift positions (in degrees)
int liftDownPosition   = 0;    
int liftUpPosition     = 45; 
//sweep position
int pos;
// Constants
const int CENTER_X = 150;     // The x-position that represents the center of the camera's view
const int TARGET_WIDTH = 115; // The object's width when it is close enough to grab
const float Kp = 1;         // Proportional gain for turning control (used for feedback)
const int MAX_SPEED = 255;    // Maximum motor speed
const int MIN_SPEED = 50;     // Minimum motor speed (ensures the robot moves instead of stalling)
const int FORWARD_SPEED = 255; // Speed for moving forward when approaching the object



int var=1;

void setup() {
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  Serial.begin(9600);  // Start serial communication for debugging
  pixy.init();         // Initialize Pixy2 camera
  clawServo.attach(10);      // Attach servo to pin 9
  liftServo.attach(9);
}
void loop() {
  Switch(var){
  Case 1: //line tracking 
    linetracking();
  break;
  Case 2: //color detecting 
    colordetecting();
  break;
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

void colordetecing() {
    pixy.ccc.getBlocks();
      // TODO: Check if an object is detected
    if (pixy.ccc.numBlocks > 0) {
      int object_x = pixy.ccc.blocks[0].m_x;
      int object_width = pixy.ccc.blocks[0].m_width;
      int error = object_x - CENTER_X;
      int turn_speed = abs(error) * Kp;
       // Constrain the turn speed between max and min speed
        turn_speed = constrain(turn_speed, MIN_SPEED, MAX_SPEED);
        if (error > 15) {
          turnLeft(turn_speed);
        } 
        else if (error < -15) {
          turnRight(turn_speed);
        }
        else{
          if (object_width < TARGET_WIDTH) {
          moveForward(FORWARD_SPEED);
          } 
          else {
          //if object closer than target width stop motors 
          stopMotors();
          clawClose();
          clawRaise();
          } 
        
        } 
    }
    else {
      // No object detect; stop the motors
      rotateMotors();
    }
}
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
    rightMotor.setSpeed(-25);
}

// Function to turn right
void turnRight(int speed) {
    // Negative to the left motor = backward 
    // Negative to the right motor = forward
    leftMotor.setSpeed(25);
    rightMotor.setSpeed(-speed);
}

// Function to stop motors
void stopMotors() {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
}

void rotateMotors() {
    leftMotor.setSpeed(255);
    rightMotor.setSpeed(-255);
}
  

void clawClose(){
    // Close the claw 
    for (pos = 140; pos >= 35; pos -= 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      clawServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
  }
}
void clawRaise(){
    // Lift arm
     for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      liftServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(30);                       // waits 15 ms for the servo to reach the position
    }

}

void clawOpen(){
  //Open Claw
    for (pos = 35; pos <= 140; pos += 1) { // goes from 180 degrees to 0 degrees
      clawServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
    // Lower arm
}

void clawLower(){
     for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      liftServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(30);                       // waits 15 ms for the servo to reach the position
     }
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

