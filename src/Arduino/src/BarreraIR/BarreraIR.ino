/*
 Leds infrarrojos - Barrera para pinza
 
 */

int sensorPin = A3;    // select the input pin for the potentiometer
int emisor = 30;
int sensorValue = 0;  // variable to store the value coming from the sensor
float Tension=0;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  pinMode(emisor, OUTPUT); 
  Serial.begin(9600); 
}

void loop() {
  // read the value from the sensor:
  digitalWrite(emisor, HIGH);
  sensorValue = analogRead(sensorPin);    
  Tension = 5*sensorValue/1023;
  
  Serial.print("Valor: "); Serial.print(sensorValue);
  Serial.print("   Tension: "); Serial.println(Tension);
  
  if(sensorValue>1000){ // Objeto en barrera
    digitalWrite(ledPin, HIGH);
  }else{
    digitalWrite(ledPin, LOW);
  }
  
  delay(500);  
}
