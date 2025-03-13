#include "CytronMotorDriver.h"
#include <Servo.h>
#include <Pixy2.h>  // Allows communication with the Pixy2 camera
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
Pixy2 pixy;

const int clawServoPin = 10; // claw servo input pin     
const int liftServoPin = 9; // lift servo input pin         


int clawOpenPosition   = 145;   
int clawClosedPosition = 35;  
// Lift positions (in degrees)
int liftDownPosition   = 160;    
int liftUpPosition     = 130; 
//sweep position
int pos;
// Constants
const int CENTER_X = 150;     // The x-position that represents the center of the camera's view
const int TARGET_WIDTH = 140; // The object's width when it is close enough to grab
const float Kp = 1.4;         // Proportional gain for turning control (used for feedback)
const int MAX_SPEED = 255;    // Maximum motor speed
const int MIN_SPEED = 50;     // Minimum motor speed (ensures the robot moves instead of stalling)
const int FORWARD_SPEED = 255; // Speed for moving forward when approaching the object


//begin 
int var=1;

void setup() {
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  Serial.begin(9600);  // Start serial communication for debugging
  pixy.init();         // Initialize Pixy2 camera
  clawServo.attach(10);      // Attach servo to pin 9
  liftServo.attach(9);
  // Set initial positions to avoid random movement
  liftServo.write(liftDownPosition);  
  clawServo.write(clawOpenPosition);
}
void loop() {
  switch(var){
  case 1: //line tracking 
    linetracking();
  break;
  case 2: //color detecting 
    colordetecting();
    break;
  case 3://lift/close claw
    Serial.println("Executing case 3...");
    clawClose();
    delay(200);
    clawRaise();
    break;
  
  }
}

//motor 1 (left) negative is forwards
//motor 2 (right) positive is forwards
void linetracking(){ 
  // Read sensor inputs
  int sensorLeftStatus = digitalRead(sensor1Pin);
  int sensorRightStatus = digitalRead(sensor2Pin);
  if (sensorLeftStatus == LOW && sensorRightStatus == LOW) {
    leftMotor.setSpeed(-170);  
    rightMotor.setSpeed(170);
  } else if (sensorLeftStatus == LOW && sensorRightStatus == HIGH) {
    // right turn
    leftMotor.setSpeed(-255);
    rightMotor.setSpeed(10);
  } else if (sensorLeftStatus == HIGH && sensorRightStatus == LOW) {
    // left turn
    leftMotor.setSpeed(-10);
    rightMotor.setSpeed(255);
  } else if (sensorLeftStatus == HIGH && sensorRightStatus == HIGH) {
    // stop
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
    // go to case 2 
    var=2;
  }
}

void colordetecting() {
    pixy.ccc.getBlocks();
      // Check if an object is detected
    if (pixy.ccc.numBlocks > 0) {
      int object_x = pixy.ccc.blocks[0].m_x;
      int object_width = pixy.ccc.blocks[0].m_width;
      int error = object_x - CENTER_X;
      int turn_speed = 200-abs(error) * Kp;
       // Constrain the turn speed between max and min speed
        turn_speed = constrain(turn_speed, MIN_SPEED, MAX_SPEED);
        if (error >= 75){
        leftMotor.setSpeed(200);
    rightMotor.setSpeed(-50);
       }
        else if (error > 20) {
          turnLeft(turn_speed);
        } 
        else if (error < -20 && error>-75) {
          turnRight(turn_speed);
         }
          else if (error <=- 75){
            leftMotor.setSpeed(50);
    rightMotor.setSpeed(-200);
        }
        else{
          if (object_width < TARGET_WIDTH) {
          moveForward(FORWARD_SPEED);
          } 
          else {
          //if object closer than target width stop motors 
          stopMotors();
          // go to case 3
          var=3;
          } 
        
        } 
    }
    else {
      // No object detect; stop the motors
      rotateMotors();
    }
}
void moveForward(int speed) {
//reversed for color tracking so 
//motor 1 (left) positive is forwards
//motor 2 (right) negative is forwards
    leftMotor.setSpeed(speed);
    rightMotor.setSpeed(-speed);
}

// Function to turn left
void turnLeft(int speed) {
    leftMotor.setSpeed(speed);
    rightMotor.setSpeed(-25);
}

// Function to turn right
void turnRight(int speed) {
    leftMotor.setSpeed(25);
    rightMotor.setSpeed(-speed);
}

// Function to stop motors
void stopMotors() {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
}
// Function to rotate motors
void rotateMotors() {
    leftMotor.setSpeed(170);
    rightMotor.setSpeed(170);
}
  
// Function to close the claw
void clawClose(){
      clawServo.write(clawClosedPosition);                   
}

// Function to lift the claw
void clawRaise(){
      liftServo.write(liftUpPosition);              
}

// Function to open the clae
void clawOpen(){
      clawServo.write(clawOpenPosition);     
}         
// Function to lower down the claw
void clawLower(){
      liftServo.write(liftDownPosition);             
}
