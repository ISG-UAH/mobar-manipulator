// Ultrasonic - Library for HR-SC04 Ultrasonic Ranging Module.
// Rev.4 (06/2012)
// J.Rodrigo ( http://www.jra.so )
// more info at http://www.ardublog.com
// GitHub: https://github.com/elrodri/Ultrasonic-HC-SR04

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int TP, int EP)
{
   pinMode(TP,OUTPUT);
   pinMode(EP,INPUT);
   Trig_pin=TP;
   Echo_pin=EP;
   Time_out=3000;  // 30ms 5 metros // 3ms 50cm
}

Ultrasonic::Ultrasonic(int TP, int EP, long TO)
{
   pinMode(TP,OUTPUT);
   pinMode(EP,INPUT);
   Trig_pin=TP;
   Echo_pin=EP;
   Time_out=TO;
}

double Ultrasonic::Timing()
{
  double duration=0;
  digitalWrite(Trig_pin, LOW);
  delayMicroseconds(T_PULSE);

  digitalWrite(Trig_pin, HIGH);
  delayMicroseconds(T_TRIGGER);
  digitalWrite(Trig_pin, LOW);

  duration = pulseIn(Echo_pin,HIGH,Time_out);
  
  digitalWrite(Trig_pin, HIGH);
  
  if ( duration == 0 ) {duration = Time_out;}
  return duration;
}

double Ultrasonic::Timing_intPulsos(int num_rep)
{
  int i;
  double medida[num_rep], suma_medidas = 0;

  for(i = 0; i<num_rep; i++)
  {
	  medida[i] = Timing();
  	  suma_medidas += medida[i];
	  delayMicroseconds(Time_out);
  }
  suma_medidas = suma_medidas / num_rep;
  return suma_medidas;
}

double Ultrasonic::Ranging(int sys){
  return Ranging(sys, 1);
}

double Ultrasonic::Ranging(int sys, int num_rep)
{
  double result=0;
  result = Timing_intPulsos(num_rep);

  /*	Vel_sonido = 345 m/s
  *	distance = (t (us) * 1e-6 / 2) * Vel_sonido (m/s) * 100 (cm)
  *	distance = tiempo (us) * 0.01715 = tiempo (us) / 58.3090379
  */
  distance_cm = result * 0.01715;

  if (sys) {
	  return distance_cm;
  } else {
	/*	Conversion cm en pulgadas
	*	1 cm = 0.3937007787 inch
	*/
	  distance_inc = distance_cm * 0.393700787;
  return distance_inc; 
  }

}
