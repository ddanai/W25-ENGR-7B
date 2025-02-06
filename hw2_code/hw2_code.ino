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
    // TODO: Retrieve detected objects from Pixy2
    // HINT: Use `pixy.ccc.getBlocks();` to get object data

    // TODO: Check if an object is detected
    // HINT: Use `pixy.ccc.numBlocks` to check if at least one block is found

    // TODO: If an object is detected:
    //   - Get the object's x-coordinate (m_x) and width (m_width)
    //   - Compute the error between object_x and CENTER_X
    //   - Calculate turn speed using proportional control: abs(error) * Kp
    //   - Constrain the turn speed between MIN_SPEED and MAX_SPEED

    // TODO: Control the robot based on the object's position:
    //   - If the object is to the left, call `turnRight(turn_speed);`
    //   - If the object is to the right, call `turnLeft(turn_speed);`
    //   - If the object is centered:
    //       * If it's far away, call `moveForward(FORWARD_SPEED);`
    //       * If it's close enough, call `grabObject();`
    
    // TODO: If no object is detected, stop the motors.
}

// Function to stop the robot and close the claw
void grabObject() {
    // TODO: Stop the motors before grabbing the object
    // TODO: Add a short delay before closing the claw
    // TODO: Close the claw by setting the correct servo angle
}

// Function to move forward
void moveForward(int speed) {
    // TODO: Set the correct motor speeds so that the rover moves forward
    // HINT: One motor will move forward, and the other will move backward (because they are facing outward)
}

// Function to turn left
void turnLeft(int speed) {
    // TODO: Set the motor speeds so that the rover turns left
    // HINT: Both motors will move in the same direction for turning
}

// Function to turn right
void turnRight(int speed) {
    // TODO: Set the motor speeds so that the rover turns right
    // HINT: Both motors will move in the same direction for turning
}

// Function to stop motors
void stopMotors() {
    // TODO: Set both motor speeds to zero
}
