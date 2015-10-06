
const int analogInPin = A0;  /
int sensorValue = 0;        // value read from the pot


void setup() {

  Serial.begin(9600); 
}

void loop() {

  sensorValue = analogRead(analogInPin);            
  Serial.print(sensorValue);      

  delay(2);                     
}

