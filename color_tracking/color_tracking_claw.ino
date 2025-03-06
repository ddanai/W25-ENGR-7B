// Include libraries needed for this project
#include <Pixy2.h>  // Allows communication with the Pixy2 camera
#include <Servo.h>  // Controls the servo motor for the claw
#include <CytronMotorDriver.h>  // Controls the left and right drive motors

// Configure motor drivers with meaningful names
CytronMD leftMotor(PWM_DIR, 3, 4);   // Left motor: PWM on Pin 3, DIR on Pin 4
CytronMD rightMotor(PWM_DIR, 6, 7); // Right motor: PWM on Pin 6, DIR on Pin 7

// Create objects for Pixy2 camera and claw servo
Pixy2 pixy;

// =========== SERVOS ===========
// 1) Claw servo: open/close
// 2) Lift servo: raise/lower the claw
Servo clawServo;
Servo liftServo;

const int clawServoPin = 10; // claw servo input pin     
const int liftServoPin = 9; // lift servo input pin         

// Claw positions (in degrees)
int clawOpenPosition   = 145;   
int clawClosedPosition = 35;  

// Lift positions (in degrees)
int liftDownPosition   = 0;    
int liftUpPosition     = 45; 
//sweep position
int pos;

// Constants
const int CENTER_X = 150;     // The x-position that represents the center of the camera's view
const int TARGET_WIDTH = 105; // The object's width when it is close enough to grab
const float Kp = 1.2;         // Proportional gain for turning control (used for feedback)
const int MAX_SPEED = 255;    // Maximum motor speed
const int MIN_SPEED = 50;     // Minimum motor speed (ensures the robot moves instead of stalling)
const int FORWARD_SPEED = 255; // Speed for moving forward when approaching the object

void setup() {
    Serial.begin(9600);  // Start serial communication for debugging
    pixy.init();         // Initialize Pixy2 camera
    clawServo.attach(10);      // Attach servo to pin 9
    liftServo.attach(9);
}

void loop(){
   //claw testing
  /* clawLower();
   clawOpen();
   clawClose();
   clawRaise();
   */
   


    trackObject();
  }
    
void trackObject() {
    pixy.ccc.getBlocks();
      // TODO: Check if an object is detected
    if (pixy.ccc.numBlocks > 0) {
      int object_x = pixy.ccc.blocks[0].m_x;
      int object_width = pixy.ccc.blocks[0].m_width;
      int error = object_x - CENTER_X;
      int turn_speed = 200- abs(error) * Kp;
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
          else if (error <= -75){
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
    rightMotor.setSpeed(-speed/2);
}

// Function to turn right
void turnRight(int speed) {
    // Negative to the left motor = backward 
    // Negative to the right motor = forward
    leftMotor.setSpeed(speed/2);
    rightMotor.setSpeed(-speed);
}

// Function to stop motors
void stopMotors() {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
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
