
#include "CytronMotorDriver.h"
#include <Servo.h>

// Configure the motor driver.
const int sensor1Pin = 2;   // Sensor 1 input pin
const int sensor2Pin = 3;   // Sensor 2 input pin
CytronMD motor1(PWM_DIR, 3, 4);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor2(PWM_DIR, 9, 10); // PWM 2 = Pin 9, DIR 2 = Pin 10.

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
  action1(
    linetracking()
  );
  break;
  Case 2: //color detecting 
  action2(
    colordetecting()
  );
  break;
  Case 3 //claw actuation
  action3(
    clawactuation()
  }
}



void linetracking(){ 
  // Read sensor inputs
  int sensorLeftStatus = digitalRead(sensor1Pin);
  int sensorRightStatus = digitalRead(sensor2Pin);
  if (sensorLeftStatus == LOW && sensorRightStatus == LOW) {
    motor1.setSpeed(150);  
    motor2.setSpeed(150)
  } else if (sensorLeftStatus == LOW && sensorRightStatus == HIGH) {
    motor1.setSpeed(0);
    motor2.setSpeed(150)
  } else if (sensorLeftStatus == HIGH && sensorRightStatus == LOW) {
    motor1.setSpeed(150);
    motor2.setSpeed(0)
  } else if (sensorLeftStatus == HIGH && sensorRightStatus == HIGH) {
    motor1.setSpeed(0);
    motor2.setSpeed(0)
    var=2;
  }
}

// Constants
const int CENTER_X = 150;     // The x-position that represents the center of the camera's view
const int TARGET_WIDTH = 200; // The object's width when it is close enough to grab
const float Kp = 1.5;         // Proportional gain for turning control (used for feedback)
const int MAX_SPEED = 255;    // Maximum motor speed
const int MIN_SPEED = 50;     // Minimum motor speed (ensures the robot moves instead of stalling)
const int FORWARD_SPEED = 150; // Speed for moving forward when approaching the object

  void colordetecting(){
    void loop(){
    trackObject();
    }
    void trackObject() {
      pixy.ccc.getBlocks();
      // TODO: Check if an object is detected
     if (pixy.ccc.numBlocks > 0) {
        int object_x = pixy.ccc.blocks[0].m_x;
        int object_width = pixy.ccc.blocks[0].m_width;
        int error = object_x - CENTER_X;
        int turn_speed = abs(error) * Kp;
        // Constrain the turn speed between max and min speed
        turn_speed = constrain(turn_speed, MIN_SPEED, MAX_SPEED);
        if (error > 5) {
          turnLeft(turn_speed);
        } 
        else if (error < -5) {
          turnRight(turn_speed);
        }
        else{
          if (object_width < TARGET_WIDTH) {
          moveForward(FORWARD_SPEED)
          } 
          else {
          var=3;
          } 
        } 
    }
    else {
      // No object detect; stop the motors
      stopMotors();
    }
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
}
void clawactuation(){
    // Close the claw 
    clawServo.write(clawClosePosition);
    // Lift arm
    liftServo.write(liftUpPosition);

}
