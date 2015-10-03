#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
byte rightSensorPin = A0;
byte leftSensorPin = A1;
int senseThreshold = 1000; //threshold for if it's over tape


void setup() {
  Serial.begin(9600);
  AFMS.begin();
  rightMotor->setSpeed(100);
  leftMotor->setSpeed(100);
}


void loop() {
  int rightSense = analogRead(rightSensorPin);
  int leftSense = analogRead(leftSensorPin);
  
  
  if (leftSense > senseThreshold) {
    leftMotor->run(RELEASE);
  }
  else if (rightSense > senseThreshold) {
    rightMotor->run(RELEASE);
  }  
  else {
    leftMotor->run(FORWARD);
    rightMotor->run(FORWARD);
  }
}
