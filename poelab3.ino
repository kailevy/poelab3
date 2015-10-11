#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
byte rightSensorPin = A1;
byte leftSensorPin = A0;
int senseThreshold = 20; //threshold for if one is too much
int error = 0; // cumulative error for integral control
int lastDiff = 0; // for derivative control
double deriv = 0; // derivative of error
float Pc = 0.15; // constant for proportional control
float Ic = 0.0015; // constant for integral control
float Dc = 20; // constant for derivative control
int baseSpeed = 20; //base speed for wheels
int sensorSkew = 0; // constant for how much the right sensor is skewed above the left
int lowestSpeed = 5; //fastest a wheel can get
int highestSpeed = 45; //slowest a wheel can get
unsigned long currMillis = 0; // for keeping track of time
unsigned long prevMillis = 0;
unsigned long timeInterval = 100; // time we should take derivative

void setup() {
  Serial.begin(9600);
  AFMS.begin();
}


void loop() {
  int rightSense = analogRead(rightSensorPin);
  int leftSense = analogRead(leftSensorPin) + sensorSkew;
  Serial.print("Right: ");
  Serial.println(rightSense); 
  Serial.print("Left: ");
  Serial.println(leftSense);
  int diffSense = rightSense-leftSense;
  int diffSpeed = 0;
  
  currMillis = millis();
  if (currMillis - prevMillis > timeInterval) {
    
    deriv = ((float)diffSense - lastDiff)/(currMillis-prevMillis);
    Serial.println(diffSense);
    Serial.println(lastDiff);
    Serial.println(currMillis);
    Serial.println(prevMillis);
    Serial.print("Derivative: ");
    Serial.println(deriv);
    prevMillis = currMillis;
    lastDiff = diffSense;
  }
  
  error += diffSense;
  
  if (diffSense > senseThreshold){
    diffSpeed = diffSense * Pc;
    Serial.println('1');
    diffSpeed = diffSpeed + Ic * error + Dc * deriv;
  }
  else if (diffSense < -senseThreshold){
    diffSpeed = diffSense * Pc;
    Serial.println('2');
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
  Serial.print("Right Speed: ");
  Serial.println(rightSpeed);
  Serial.print("Left Speed: ");
  Serial.println(leftSpeed);
  rightMotor->setSpeed(rightSpeed);
  leftMotor->setSpeed(leftSpeed);
  
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
}
