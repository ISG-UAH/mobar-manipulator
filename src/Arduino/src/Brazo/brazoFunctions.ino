double readSensor(int rep){
  double readValue = 0;
  int count; 
  
  for(count = 0; count<rep; count++){
    readValue += analogRead(sensorPin);    
  }
  readValue = readValue/rep;
  
  return readValue;
}

void readColor()
{
  int i;
    for(i=0; i<3; i++)
    {
      digitalWrite(led[i], HIGH);
      delay(300);
      sensorValue[i] = readSensor(50);
      sensorValue[i] = map(sensorValue[i], blackValue[i], whiteValue[i], 0, 255);
      
      if(sensorValue[i] > 255) sensorValue[i] = 255;
      if(sensorValue[i] < 0) sensorValue[i] = 0;
      delay(100);
      digitalWrite(led[i], LOW);
      Serial.print(" ");
      Serial.print(sensorValue[i]);
    }
    Serial.println(";");
}


void Radar()
{
  int grad=0, num_targets=0, consec=0, ant=0;;
  double dist, targets[180], dist_targets[180], gradPrev;
  
  for(grad = 10; grad < 170; grad++)
  {
    gradPrev = s_radar.read();
    s_radar.write(grad);
    delay(abs(gradPrev-grad)*VEL_SERVO);
    dist = ultrasonic.Ranging(CM);
    
        
    if(dist < DIST_MAX){
      consec++;
      if(consec > 10){
        targets[num_targets] = grad;
        num_targets++;
        Serial.print(grad); Serial.print(", ");
        Serial.println(dist);
        consec=0;
      }
    }else{
        consec=0;
    }
  }
  s_radar.write(90);delay(90*VEL_SERVO);
  for(grad = 0; grad<=num_targets; grad++){
    dist_targets[grad] = MoveRadar(targets[grad]);
  }
  s_radar.write(90);
}
double MoveRadar(int grad){
  double dist, gradPrev;
  
  gradPrev = s_radar.read();
  s_radar.write(grad);
  delay(abs(gradPrev-grad)*VEL_SERVO);
  
  dist=ultrasonic.Ranging(CM,50);
  if(dist < DIST_MAX){
    Serial.print("Ang: ");
    Serial.print(grad);
    Serial.print("\t Rad: ");
    Serial.print(grad*M_PI/180);
    Serial.print("\t Distancia: ");
    Serial.print(dist); // CM or INC
    Serial.println(" cm" );
  }
  return dist;
}

void CheckCompass(){
   float nowXY, servo0, resp;
   
   nowXY = getDataMag();
          
   servo0 = miservo[0].read();
   
   Serial.print("refXY: ");
   Serial.print(refXY); Serial.print(" \t nowXY: ");
   Serial.print(nowXY); Serial.print(" \t Servo0: ");
   Serial.println(servo0);
}

float getDataMag(){

    //Get the reading from the HMC5883L and calculate the heading
    MagnetometerScaled scaled = mag.ReadScaledAxis(); //scaled values from compass.
    float heading = atan2(scaled.YAxis, scaled.XAxis);
    
    float declinationAngle = -0.019477874;
    heading += declinationAngle;
    
    // Correct for when signs are reversed.
    if(heading < 0) heading += 2*PI;
    if(heading > 2*PI) heading -= 2*PI;
    
    return heading * RAD_TO_DEG; //radians to degrees
}

void setupHMC5883L(){
    //Setup the HMC5883L, and check for errors
    int error;
    float gauss = 1.3;
    error = mag.SetScale(gauss); //Set the scale of the compass.
    error = mag.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
}

double getProximity(int pinAnalog){
   int i;
   double sensorVal = 0;
   
   for(i = 0; i<READ_REP; i++){
      sensorVal += analogRead(pinAnalog);
   } 
   sensorVal = sensorVal/READ_REP;
   
   return sensorVal;
}


// MoveTo: 1.5708 1.70 0.5048 1.2208 0.876 2.010;
void MoveTo(float servos[]){
  for(int i=0; i<5; i++){
    miservo[i].write(servos[i]);
  }
}




