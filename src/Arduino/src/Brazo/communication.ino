void decodificaCadena(char* cadenaIN){
 int i=0, num_servo;
 char *comando, *param;
 float servos[6], radServo;
 
// delay(200);
 
 comando = strtok (cadenaIN,": ");
 if (stricmp(comando, "MoveTo") == 0)
 {
     while ((param = strtok (NULL, " ")) != NULL)
      {
        radServo=atof(param);
        servos[i] = radServo * 360 / (2*M_PI);
        i++;
      }

      MoveTo(servos);
      Serial.println("MoveTo: Done;");

 }else if (stricmp(comando, "MoveServo") == 0)
 {
     param = strtok(NULL, " ");
     num_servo = atoi(param);    

     param = strtok(NULL, " ");
     radServo=atof(param);

     miservo[num_servo].write(radServo*180/M_PI);
     Serial.println("MoveServo: Done;");
     

 }else if (stricmp(comando, "GetServo") == 0)
 {
     param = strtok(NULL, " ");
     num_servo = atoi(param);
     
     Serial.print("GetServo: ");
     Serial.println(miservo[num_servo].read()*180/M_PI);
     Serial.print(";");

 }else if (stricmp(comando, "SetVel") == 0)
 {
     param = strtok(NULL, " ");
     tam_pasos = atoi(param);
     Serial.println("SetVel: Done;");

 }else if (stricmp(comando, "GetVel") == 0){
     Serial.print("GetVel: ");
     Serial.println(tam_pasos);

 }else if (stricmp(comando, "GetPos") == 0){
     Serial.print("GetPos:");
     for(i=0; i<5; i++){
        servos[i] = miservo[i].read()*(2*M_PI)/360;
        Serial.print(" ");
        Serial.print(servos[i]);
       }
     Serial.println(";");

 }else if (stricmp(comando, "Radar") == 0){
     
     Serial.println("Radar START:");
     Radar();
     Serial.println("Radar END;");

 }else if (stricmp(comando, "MoveRadar") == 0){
     param = strtok(NULL, " ");
     MoveRadar(atoi(param));
     Serial.println("MoveRadar Done;");

 }else if (stricmp(comando, "GetColor") == 0){
       Serial.print("GetColor:");
       readColor();
       
 }else if (stricmp(comando, "GetBarrier") == 0){
       Serial.print("GetBarrier: ");
       Serial.print(analogRead(BarrierIn));
       Serial.println(";");  
 
 }else if (stricmp(comando, "GetCompass") == 0){
       CheckCompass();

 }else if (stricmp(comando, "GetProximity") == 0){
   Serial.print("GetProximity:");
    for(i=initIR; i<initIR+NUM_SENSORS; i++){
        sensorValue[i] = getProximity(i);
        
        Serial.print(" ");
        Serial.print(sensorValue[i]);
    }
    Serial.println(";");
 } else {
    Serial.println("Error: Command not found."); 
 }
}

// STRCMP from standard C++ Library, but case insensitive
int stricmp (const char *p1, const char *p2)
{
  register unsigned char *s1 = (unsigned char *) p1;
  register unsigned char *s2 = (unsigned char *) p2;
  unsigned char c1, c2;
 
  do
  {
      c1 = (unsigned char) toupper((int)*s1++);
      c2 = (unsigned char) toupper((int)*s2++);
      if (c1 == '\0')
      {
            return 0;
      }
  }
  while (c1 == c2);
 
  return c1 - c2;
}
