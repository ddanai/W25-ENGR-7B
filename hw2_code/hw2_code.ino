// Include libraries needed for this project
#include <Pixy2.h>  // Allows communication with the Pixy2 camera
#include <Servo.h>  // Controls the servo motor for the claw
#include <CytronMotorDriver.h>  // Controls the left and right drive motors

// Configure motor drivers with meaningful names
CytronMD leftMotor(PWM_DIR, 3, 4);   // Left motor: PWM on Pin 3, DIR on Pin 4
CytronMD rightMotor(PWM_DIR, 9, 10); // Right motor: PWM on Pin 9, DIR on Pin 10

// Create objects for Pixy2 camera and claw servo
Pixy2 pixy;
Servo claw;

// Constants (DO NOT CHANGE)
const int CENTER_X = 150;     // The x-position that represents the center of the camera's view
const int TARGET_WIDTH = 200; // The object's width when it is close enough to grab
const float Kp = 1.5;         // Proportional gain for turning control (used for feedback)
const int MAX_SPEED = 255;    // Maximum motor speed
const int MIN_SPEED = 50;     // Minimum motor speed (ensures the robot moves instead of stalling)
const int FORWARD_SPEED = 150; // Speed for moving forward when approaching the object
const int TURN_THRESHOLD = 5; // How close the object must be to the center before moving forward
const int clawClosePosition = 90; // closed position of claw

void setup() {
    Serial.begin(115200);
    claw.attach(5);  // Attach the servo motor to digital pin 5
    claw.write(0);   // Start with the claw open
    pixy.init();     // Initialize Pixy2 camera
}

void loop() {
    trackObject(); // Call the function to detect and approach the object
}

/***************************************************
 * FUNCTION DEFINITIONS
 * ---------------------------------
 * This is where you will complete each function.
 * Follow the TODO instructions below.
 ***************************************************/

// Function to track and approach an object using Pixy2
void trackObject() {
  
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks > 0) {
    int xCoordinate = pixy.ccc.blocks[0].m_x;
    int width = pixy.ccc.blocks[0].m_width;
    int error = xCoordinate - CENTER_X;
    int turnSpeed =  Kp * abs(error);
    turnSpeed  = constrain(turnSpeed, MIN_SPEED, MAX_SPEED);


    if (error > TURN_THRESHOLD) {
      turnLeft(turnSpeed);

    } else if( error < -TURN_THRESHOLD ) {
      turnRight(turnSpeed);
    } else {
      if (width < TARGET_WIDTH) {
        moveForward(FORWARD_SPEED);
      } else {
        grabObject();
      }
    }
  
   } else {
    stopMotors();
    }
}

// Function to stop the robot and close the claw
void grabObject() {
    // TODO: Stop the motors before grabbing the object
    // TODO: Add a short delay before closing the claw
    // TODO: Close the claw by setting the correct servo angle
     stopMotors();
     delay(500);
     claw.write(clawClosePosition);

  
}

// Function to move forward
void moveForward(int speed) {
    // TODO: Set the correct motor speeds so that the rover moves forward
    // HINT: One motor will move forward, and the other will move backward (because they are facing outward)
    leftMotor.setSpeed(speed);
    rightMotor.setSpeed(-speed);
   
}

// Function to turn left
void turnLeft(int speed) {
    // TODO: Set the motor speeds so that the rover turns left
    // HINT: Both motors will move in the same direction for turning
    leftMotor.setSpeed(-speed);
    rightMotor.setSpeed(speed);
}

// Function to turn right
void turnRight(int speed) {
    // TODO: Set the motor speeds so that the rover turns right
    // HINT: Both motors will move in the same direction for turning
    leftMotor.setSpeed(speed);
    rightMotor.setSpeed(-speed);
}

// Function to stop motors
void stopMotors() {
    // TODO: Set both motor speeds to zero
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
}
