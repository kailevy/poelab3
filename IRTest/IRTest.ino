
const int analogInPin = A0;  
int sensorValue = 0;        // value read from the pot


void setup() {

  Serial.begin(9600); 
}

void loop() {

  sensorValue = analogRead(analogInPin);  
  //float voltage= sensorValue * (5.0 / 1023.0);
  Serial.println(sensorValue);      

  delay(2);                     
}

