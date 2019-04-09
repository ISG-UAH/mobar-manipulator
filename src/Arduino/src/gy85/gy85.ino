#include "Wire.h"

#include "I2Cdev.h"
#include "ADXL345.h"
#include "HMC5883L.h"
#include "ITG3200.h"

#define LED_PIN 13 // (Arduino is 13, Teensy is 6)
#define SENS 131072
#define MIN 100

#define GYRO_SENS 14375 // LSB per º/sec
#define GYRO_RANGE 2000 // +- 2000 º/sec
#define GYRO_VOLTAGE 3.6 // V
#define GYRO_ZEROVOLTAGE 2.1 // V (offset)

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


// class default I2C address is 0x53
// specific I2C addresses may be passed as a parameter here
// ALT low = 0x53 (default for SparkFun 6DOF board)
// ALT high = 0x1D

bool blinkState = false;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

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
    
    
    // Init vars
    ref_ax = 0, ref_ay = 0, ref_az = 0;
    ref_mx = 0, ref_my = 0, ref_mz = 0;
    ref_gx = 0, ref_gy = 0, ref_gz = 0;
}

void getDataAccel(){
    accel.getAcceleration(&ax, &ay, &az);

    dif_ax = ref_ax - ax;
    dif_ay = ref_ay - ay;
    dif_az = ref_az - az;
    
    if(abs(dif_ax) > MIN  || abs(dif_ay) > MIN || abs(dif_az) > MIN){
      
      Serial.print("Referencia: ");
      Serial.print(ref_ax); Serial.print(" ");
      Serial.print(ref_ay); Serial.print(" ");
      Serial.print(ref_az); Serial.print(" ");
      
      Serial.print("Actual: ");
      Serial.print(ax); Serial.print(" ");
      Serial.print(ay); Serial.print(" ");
      Serial.print(az); Serial.print(" ");
      
      Serial.print("Diferencia: ");
      Serial.print(dif_ax); Serial.print(" ");
      Serial.print(dif_ay); Serial.print(" ");
      Serial.print(dif_az); Serial.println(" ");
      
      ref_ax = ax;
      ref_ay = ay;
      ref_az = az;
    }
}

void getDataGyro(){
  
    float v_gx, v_gy, v_gz;
    
    gyro.getRotation(&gx, &gy, &gz);
    v_gx = (float)gx*2*100/GYRO_SENS;
    v_gy = (float)gy;
    v_gz = (float)gz;

/*    v_gx = (float)(gx*2*100/GYRO_SENS);
    v_gy = (float)(gy*2*100/GYRO_SENS);
    v_gz = (float)(gz*2*100/GYRO_SENS);
*/
   /* dif_gx = (v_gx - GYRO_ZEROVOLTAGE)/GYRO_SCALE;
    dif_gy = (v_gy - GYRO_ZEROVOLTAGE)/GYRO_SCALE;
    dif_gz = (v_gz - GYRO_ZEROVOLTAGE)/GYRO_SCALE;
    */
    dif_gx = (float)(ref_gx - gx);
    dif_gy = (float)(ref_gy - gy);
    dif_gz = (float)(ref_gz - gz);
    
    //if(abs(dif_gx) > 10  || abs(dif_gy) > 10 || abs(dif_gz) > 10){
      /*
      Serial.print("Referencia: ");
      Serial.print(ref_gx); Serial.print(" ");
      Serial.print(ref_gy); Serial.print(" ");
      Serial.print(ref_gz); Serial.print(" ");
      */
      Serial.print("Actual: ");
      Serial.print(gx); Serial.print(" ");
      Serial.print(gy); Serial.print(" ");
      Serial.print(gz); Serial.print(" ");
      
      Serial.print("Voltage: ");
      Serial.print(v_gx); Serial.print(" ");
      Serial.print(v_gy); Serial.print(" ");
      Serial.print(v_gz); Serial.println(" ");
      /*
      Serial.print("Diferencia: ");
      Serial.print(dif_gx); Serial.print(" ");
      Serial.print(dif_gy); Serial.print(" ");
      Serial.print(dif_gz); Serial.println(" ");
*/
      ref_gx = gx;
      ref_gy = gy;
      ref_gz = gz;
    //}
}

void getDataMag(){
    mag.getHeading(&mx, &my, &mz);

    dif_mx = ref_mx - mx;
    dif_my = ref_my - my;
    dif_mz = ref_mz - mz;
    
    if(abs(dif_mx) > MIN  || abs(dif_my) > MIN || abs(dif_mz) > MIN){
      
      Serial.print("Referencia: ");
      Serial.print(ref_mx); Serial.print(" ");
      Serial.print(ref_my); Serial.print(" ");
      Serial.print(ref_mz); Serial.print(" ");
      
      Serial.print("Actual: ");
      Serial.print(mx); Serial.print(" ");
      Serial.print(my); Serial.print(" ");
      Serial.print(mz); Serial.print(" ");
      
      Serial.print("Diferencia: ");
      Serial.print(dif_mx); Serial.print(" ");
      Serial.print(dif_my); Serial.print(" ");
      Serial.print(dif_mz); Serial.println(" ");
      
      ref_mx = mx;
      ref_my = my;
      ref_mz = mz;
    }
}

void loop() {

    // getDataAccel();
     getDataGyro();
    // getDataMag();

    delay(500);
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
}
