//Limulidae 
//Members : Isaac Lagunas, Renee Limonadi, Diego Martin, Darius Danai, Isabelle Yin, Victoria Chen, Jason Juarez
//Last Edit: 3/18/25
// This code controls our autonomous rover to follow a black line, detect an object, and then lift it up with our claw.
// It operates in three different modes: line tracking, color detecting, and grabbing/lifting the object
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
const int CENTER_X = 130;     // The x-position that represents the center of the camera's view
const int TARGET_WIDTH = 145; // The object's width when it is close enough to grab
const float Kp = 1.8;         // Proportional gain for turning control (used for feedback)
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
  // Set initial servo positions to avoid random movement
  liftServo.write(liftDownPosition);  
  clawServo.write(clawClosedPosition);
  
}
void loop() {
  switch(var){
  case 1: //line tracking 
    linetracking();
  break;
  case 2: //color detecting 
    clawOpen();
    colordetecting();
    break;
  case 3://lift/close claw
    Serial.println("Executing case 3...");
    clawClose();
    delay(100);
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
  // Value to adjust speed of turn
  int turn_adj = 5;
  int turnSpeed = 255;
  int speed=255;
  int KP=2.8;  
  if (sensorLeftStatus == LOW && sensorRightStatus == LOW) { // Both on white go straight
    leftMotor.setSpeed(-speed);  
    rightMotor.setSpeed(speed);
  } else if (sensorLeftStatus == LOW && sensorRightStatus == HIGH) { //Right sensor on the line start slowing down the right motor
    leftMotor.setSpeed(-speed);
    rightMotor.setSpeed(turnSpeed);
     while (sensorLeftStatus == LOW){
      turn_adj=turn_adj*KP;
     turnSpeed=turnSpeed-turn_adj;
     turnSpeed=constrain(turnSpeed,20,255);
     leftMotor.setSpeed(-speed);
     rightMotor.setSpeed(turnSpeed);
     delay(15);
       
       if (digitalRead(sensor1Pin) == HIGH){ // Stop turning when the left sensor detects the line
        turnSpeed=255;
        turn_adj=3;
        break;
       }
     }

  } else if (sensorLeftStatus == HIGH && sensorRightStatus == LOW) { // Left sensor on the line start slowing down the left motor
    leftMotor.setSpeed(-turnSpeed);
    rightMotor.setSpeed(speed);
     while (sensorRightStatus == LOW){
      turn_adj=turn_adj*KP;
     turnSpeed=turnSpeed-turn_adj;
     turnSpeed=constrain(turnSpeed,20,255);
     leftMotor.setSpeed(-turnSpeed);
     rightMotor.setSpeed(speed);
     delay(15);
       if (digitalRead(sensor2Pin) == HIGH){ // Stop turning when the right sensor detects the line
        turnSpeed=255;
        turn_adj=3;
        break;
       }
     }
  } else if (sensorLeftStatus == HIGH && sensorRightStatus == HIGH) { // Stop motors when both sensors detect black. Change cases.
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
    var=2;
      
  }}

void colordetecting() {
    pixy.ccc.getBlocks();
      // Check if an object is detected
    if (pixy.ccc.numBlocks > 0) {
      int object_x = pixy.ccc.blocks[0].m_x;
      int object_width = pixy.ccc.blocks[0].m_width;
      int error = object_x - CENTER_X;
      int turn_speed = 245-abs(error) * Kp;
       // Constrain the turn speed between max and min speed
        turn_speed = constrain(turn_speed, MIN_SPEED, MAX_SPEED);
        if (error >= 75){
        leftMotor.setSpeed(255);
    rightMotor.setSpeed(-70);
       }
        else if (error > 10) {
          turnLeft(turn_speed);
        } 
        else if (error < -10 && error>-75) {
          turnRight(turn_speed);
         }
          else if (error <=- 75){
            leftMotor.setSpeed(70);
    rightMotor.setSpeed(-255);
        }
        else{
          if (object_width < TARGET_WIDTH) {
          moveForward(FORWARD_SPEED);
          } 
          else {
          //if object closer than target width stop motors and change cases 
          stopMotors();
          var=3;
          } 
        
        } 
    }
    else {
      // No object detect; spin rover
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
    leftMotor.setSpeed(255);
    rightMotor.setSpeed(-speed);
}

// Function to turn right
void turnRight(int speed) {
    // Negative to the left motor = backward 
    // Negative to the right motor = forward
    leftMotor.setSpeed(speed);
    rightMotor.setSpeed(-255);
}

// Function to stop motors
void stopMotors() {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
}

// Function to spin rover
void rotateMotors() { 
    leftMotor.setSpeed(-255);
    rightMotor.setSpeed(-255);
}
  
// Function to close the claw
void clawClose(){
      clawServo.write(clawClosedPosition);                   
}

// Function to lift the claw
void clawRaise(){
      liftServo.write(liftUpPosition);              
}

// Function to open the claw
void clawOpen(){
      clawServo.write(clawOpenPosition);     
}         

// Function to lower the claw
void clawLower(){
      liftServo.write(liftDownPosition);             
}
