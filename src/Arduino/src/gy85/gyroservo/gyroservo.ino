#include "Wire.h"
#include "Servo.h"

#include "I2Cdev.h"
#include "ADXL345.h"
#include "HMC5883L.h"
#include "ITG3200.h"

#define LED_PIN 13 // (Arduino is 13, Teensy is 6)
#define SENS 131072
#define MIN 100

#define GYRO_SENS 14375 // LSB per º/sec

HMC5883L mag;
ITG3200 gyro;
ADXL345 accel;

int16_t ax, ay, az;
int16_t mx, my, mz;
int16_t gx, gy, gz;

int16_t ref_ax, ref_ay, ref_az;
int16_t ref_mx, ref_my, ref_mz;
int16_t ref_gx, ref_gy, ref_gz;

float dif_ax, dif_ay, dif_az;
float dif_mx, dif_my, dif_mz;
float dif_gx, dif_gy, dif_gz;

Servo servo;
float angle = 90;


unsigned long time_1=0;
unsigned long time_2=0;

void setup(){
  Wire.begin();
  servo.attach(4);
  servo.write(angle);
    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);
    
    // configure LED for output
    pinMode(LED_PIN, OUTPUT);

    // initialize device
    Serial.println("Initializing I2C devices...");

    // verify connection
    Serial.println("Testing device connections...");
    accel.initialize();
    Serial.println(accel.testConnection() ? "ADXL345 connection successful" : "ADXL345 connection failed");

    
    mag.initialize();
    Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
    
    gyro.initialize();
    Serial.println(gyro.testConnection() ? "ITG3200 connection successful" : "ITG3200 connection failed");
  
}

void getDataGyro(){
    
    float v_gx, v_gy, v_gz;

    time_1 = time_2;
    gyro.getRotation(&gx, &gy, &gz);
    time_2 = micros();
    v_gx = (float)gx/GYRO_SENS;

    //angle = (float)angle+v_gx*180;
    angle=angle+v_gx*180*((float)time_2-(float)time_1)/100000;
    
    if(angle>=180) angle = 180;
    else if (angle<0) angle = 0;
      Serial.print("ActualX: ");
      Serial.print(gx); Serial.print(" ");
      
      Serial.print("VoltageX: ");
      Serial.print(v_gx); Serial.print(" ");
      
      Serial.print("AnguloServo: ");
      servo.write(angle);
      Serial.print(angle); Serial.println(" ");

      ref_gx = gx;
      ref_gy = gy;
      ref_gz = gz;
}

void loop() {

    // getDataAccel();
     getDataGyro();
    // getDataMag();

    //delay(500);
}
