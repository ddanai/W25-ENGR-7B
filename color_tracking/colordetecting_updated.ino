// Include libraries needed for this project
#include <Pixy2.h>  // Allows communication with the Pixy2 camera
#include <Servo.h>  // Controls the servo motor for the claw
#include <CytronMotorDriver.h>  // Controls the left and right drive motors

// Configure motor drivers with meaningful names
CytronMD leftMotor(PWM_DIR, 3, 4);   // Left motor: PWM on Pin 3, DIR on Pin 4
CytronMD rightMotor(PWM_DIR, 6, 7); // Right motor: PWM on Pin 6, DIR on Pin 7

// Create objects for Pixy2 camera and claw servo
Pixy2 pixy;
Servo claw;
// Constants
const int CENTER_X = 150;     // The x-position that represents the center of the camera's view
const int TARGET_WIDTH = 200; // The object's width when it is close enough to grab
const float Kp = 1.5;         // Proportional gain for turning control (used for feedback)
const int MAX_SPEED = 255;    // Maximum motor speed
const int MIN_SPEED = 50;     // Minimum motor speed (ensures the robot moves instead of stalling)
const int FORWARD_SPEED = 150; // Speed for moving forward when approaching the object

void setup() {
    Serial.begin(9600);  // Start serial communication for debugging
    pixy.init();         // Initialize Pixy2 camera
    claw.attach(5);      // Attach servo to pin 5
}

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
          moveForward(FORWARD_SPEED);
          } 
          else {
          int var=3;
          } 
        } 
    }
    else {
      // No object detect; stop the motors
      stopMotors();
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