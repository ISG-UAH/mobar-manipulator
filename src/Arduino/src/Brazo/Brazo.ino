#include <Servo.h> 
#include <Ultrasonic.h>
#include <Wire.h>

#include <I2Cdev.h>
#include <ADXL345.h>
#include <HMC5883L.h>
#include <ITG3200.h>

#define VEL_SERVO 4.333333
#define MARGEN_ERROR 0.5

#define GYRO_SENS 14375 // LSB per º/sec
#define GYRO_RANGE 2000 // +- 2000 º/sec
#define GYRO_VOLTAGE 3.6 // V
#define GYRO_ZEROVOLTAGE 2.1 // V (offset)

#define DIST_MAX 22

// Proximity sensors
#define READ_REP 10
#define NUM_SENSORS 8
double sensorValue[16];
int initIR = 4;

HMC5883L mag;
ITG3200 gyro;
ADXL345 accel;

int count=0;
char cadena[200];
Servo miservo[6];
float qPrev[6], qAct[6];
float tam_pasos;
unsigned long duration;
float refXY;

int16_t ax, ay, az;
int16_t mx, my, mz;
int16_t gx, gy, gz;


// Radar
Ultrasonic ultrasonic(44 , 45); // (Trig PIN,Echo PIN)
Servo s_radar;

// Color
double blackValue[3] = {102.82, 65.90, 87.52};
double whiteValue[3] = {195.90, 133.54, 143.68};
//double blackValue[3] = {80.82, 74.96, 84.52};
//double whiteValue[3] = {215.82, 156.70, 146.52};
int sensorPin = A12; 
int led[3] = {38, 39, 40};

int BarrierIn = A13;

void setup(){
  
  int numservo[] = {11, 10, 9, 8, 7, 6};
  float valservo[] = {90, 130, 90, 130, 90, 60};
   
  for(int i=0; i<6; i++){
     qPrev[i] = valservo[i];
     miservo[i].attach(numservo[i]);
  }
  
  tam_pasos = 5;
  
  MoveTo(qPrev);
  Serial.begin(57600);
  // Puerto I2C
  Wire.begin();
//  Serial.println("Initializing I2C devices...");
//  Serial.println("Testing device connections...");
  accel.initialize();
//  Serial.println(accel.testConnection() ? "ADXL345 connection successful" : "ADXL345 connection failed");
    
  mag = HMC5883L();
  setupHMC5883L();
  delay(500);
  refXY = getDataMag();
 
    
  gyro.initialize();
//  Serial.println(gyro.testConnection() ? "ITG3200 connection successful" : "ITG3200 connection failed");
  
  
  // Radar
  s_radar.attach(3);
  s_radar.write(90);

  // COLOR Sensor
  pinMode(led[0], OUTPUT);
  pinMode(led[1], OUTPUT);
  pinMode(led[2], OUTPUT);
  
   // Puerto Serie

//  Serial.println("Arduino ON");
 
}

void loop(){
  while(Serial.available()>0){
     cadena[count] = Serial.read();
     if(cadena[count] == ';'){
       count=0;
       decodificaCadena(cadena);
       Serial.flush();
     }else{
       count++; 
     }
  }
  
}






