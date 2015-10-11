#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
byte rightSensorPin = A1;
byte leftSensorPin = A0;
int senseThreshold = 0; //threshold for if one is too much
int error = 0; // cumulative error for integral control
int lastDiff = 0; // for derivative control
double deriv = 0; // derivative of error
float Pc = 0.17; // constant for proportional control
float Ic = 0.0017; // constant for integral control
float Dc = 25; // constant for derivative control
int baseSpeed = 20; //base speed for wheels
int sensorSkew = 0; // constant for how much the right sensor is skewed above the left
int lowestSpeed = 5; //fastest a wheel can get
int highestSpeed = 45; //slowest a wheel can get
unsigned long currMillis = 0; // for keeping track of time
unsigned long prevMillis = 0;
unsigned long timeInterval = 50; // time we should take derivative

void setup() {
  Serial.begin(9600);
  AFMS.begin();
}


void loop() {
  int rightSense = analogRead(rightSensorPin);
  int leftSense = analogRead(leftSensorPin) + sensorSkew;
  int diffSense = rightSense-leftSense;
  int diffSpeed = 0;
  
  currMillis = millis();
  if (currMillis - prevMillis > timeInterval) {
    
    deriv = ((float)diffSense - lastDiff)/(currMillis-prevMillis);
    prevMillis = currMillis;
    lastDiff = diffSense;
  }
  
  error += diffSense;
  
  if (diffSense > senseThreshold){
    diffSpeed = diffSense * Pc;
    diffSpeed = diffSpeed + Ic * error + Dc * deriv;
  }
  else if (diffSense < -senseThreshold){
    diffSpeed = diffSense * Pc;
    diffSpeed = diffSpeed + Ic * error + Dc * deriv;
  }
  int rightSpeed = baseSpeed - diffSpeed;
  int leftSpeed = baseSpeed + diffSpeed;
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
  
  rightMotor->setSpeed(rightSpeed);
  leftMotor->setSpeed(leftSpeed);
  
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  
  
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
