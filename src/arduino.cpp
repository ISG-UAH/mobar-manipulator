#define M_PI 3.141592654

int count=0, i;
char aux;
char cadena[100];
double servos[6];
float radServo;

void setup(){
  Serial.begin(9600);
  Serial.println("Arduino ON");
}

void loop(){
  if (Serial.available()>0){
    // Leemos la instruccion
    while(aux=Serial.read()!=' '){
      cadena[count] = aux;
      Serial.print(count);
      Serial.println(cadena[count]);
      count++;
    }

    // Operamos con la instruccion
    // [...]

    for(i = 0; i < 6; i++){
      radServo = Serial.parseFloat();
      servos[i] = radServo * 360 / (2*M_PI);
      Serial.print(servos[i]);
      Serial.print(" ");
    }

/*
    for(count = 0; count < 6; count++){
      Serial.print(servos[count]);
      Serial.print(" ");
    }
*/
  Serial.flush();

  }
}
