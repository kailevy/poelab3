#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
byte rightSensorPin = A1;
byte leftSensorPin = A0;
int error = 0; // cumulative error for integral control
int lastDiff = 0; // for derivative control
double deriv = 0; // derivative of error
float Pc = 0.166; // constant for proportional control
float Ic = 0.00166; // constant for integral control
float Dc = 25; // constant for derivative control
int baseSpeed = 20; //base speed for wheels
int sensorSkew = 0; // constant for how much the right sensor is skewed above the left
int lowestSpeed = 5; //fastest a wheel can get
int highestSpeed = 45; //slowest a wheel can get
unsigned long currMillis = 0; // for keeping track of time
unsigned long prevMillis = 0;
unsigned long timeInterval = 50; // time we should take derivative

int scalar = 10; // for scaling base speed, PID constants
int lastScalar = 10; // keeps track of previous one
int scaleB = 2; // scalar for base speed
float scaleP = 0.0167; // scalar for proportional constant
float scaleI = 0.000167; //scalar for integral constant
float scaleD = 2.5; //scalar for derivative constant

void setup() {
  Serial.begin(9600);
  AFMS.begin();
}


void loop() {
  // read serial and set values appropriately
  scalar = read_serial(scalar);
  
  if (scalar != lastScalar){
    // only compute new values of scalar has changed
    lastScalar = scalar;
    baseSpeed = scalar * scaleB;
    Pc = scalar * scaleP;
    Ic = scalar * scaleI;
    Dc = scalar * scaleD;
  }

  int diffSpeed = 0;
  
  // read sensors
  int rightSense = analogRead(rightSensorPin);
  int leftSense = analogRead(leftSensorPin) + sensorSkew;
  int diffSense = rightSense-leftSense;

  currMillis = millis();
  if (currMillis - prevMillis > timeInterval) {
    // if enough time has elapsed, calculate derivative
    deriv = ((float)diffSense - lastDiff)/(currMillis-prevMillis);
    prevMillis = currMillis;
    lastDiff = diffSense;
  }

  // add to integral
  error += diffSense;
  
  // compute speed difference
  diffSpeed = diffSense * Pc + Ic * error + Dc * deriv;
  
  // apply speed difference to left and right wheels
  int rightSpeed = baseSpeed - diffSpeed;
  int leftSpeed = baseSpeed + diffSpeed;
  
  //keep them within reasonable limits
  if (rightSpeed < lowestSpeed) {
    rightSpeed = lowestSpeed;
  }
  if (rightSpeed > highestSpeed) {
    rightSpeed = highestSpeed;
  }
  if (leftSpeed < lowestSpeed) {
    leftSpeed = lowestSpeed;
  }
  if (leftSpeed > highestSpeed) {
    leftSpeed = highestSpeed;
  }

  // send speed to motors
  rightMotor->setSpeed(rightSpeed);
  leftMotor->setSpeed(leftSpeed);

  // run motors
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);

  // log values
  Serial.print(rightSense); 
  Serial.print(",");
  Serial.print(leftSense);
  Serial.print(",");
  Serial.print(diffSense);
  Serial.print(",");
  Serial.print(deriv);
  Serial.print(",");
  Serial.print(error);
  Serial.print(",");
  Serial.print(rightSpeed);
  Serial.print(",");
  Serial.println(leftSpeed);
}

int read_serial(int prev_val) {
  //reads serial input
  if (Serial.available() > 0) {
    return Serial.parseInt();
  }
  return prev_val;
}

