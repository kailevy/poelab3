#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
byte rightSensorPin = A0;
byte leftSensorPin = A1;
int senseThreshold = 30; //threshold for if one is too much


void setup() {
  Serial.begin(9600);
  AFMS.begin();
}


void loop() {
  int rightSense = analogRead(rightSensorPin);
  int leftSense = analogRead(leftSensorPin);
  int diffSense = rightSense-leftSense;
  int diffSpeed = 0;
  
  if (diffSense > senseThreshold){
    diffSpeed = diffSense/2;
  }
  else if (diffSense < -senseThreshold){
    diffSpeed = -diffSense/2;
  }
  
  rightMotor->setSpeed(100 + diffSpeed);
  leftMotor->setSpeed(100 - diffSpeed);
  
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
}
