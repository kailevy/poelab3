
const int analogInPin1 = A0;  
const int analogInPin2 = A1;
int sensorValue1 = 0;
int sensorValue2 = 0;


void setup() {

  Serial.begin(9600); 
}

void loop() {

  sensorValue1 = analogRead(analogInPin1);  
  sensorValue2 = analogRead(analogInPin2);
  //float voltage= sensorValue * (5.0 / 1023.0);
  Serial.print("Sensor 1: ");
  Serial.println(sensorValue1); 
  Serial.print("Sensor 2: ");
  Serial.println(sensorValue2);
                     
}

