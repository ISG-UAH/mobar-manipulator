// Ultrasonic - Library for HR-SC04 Ultrasonic Ranging Module.
// Rev.4 (06/2012)
// J.Rodrigo ( http://www.jra.so )
// more info at http://www.ardublog.com
// GitHub: https://github.com/elrodri/Ultrasonic-HC-SR04

#ifndef Ultrasonic_h
#define Ultrasonic_h

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define CM 1
#define INC 0
#define T_PULSE 2
#define T_TRIGGER 5

class Ultrasonic
{
  public:
    Ultrasonic(int TP, int EP);
	Ultrasonic(int TP, int EP, long TO);
    double Timing();
    double Timing_intPulsos(int num_rep);
    double Ranging(int sys);
    double Ranging(int sys, int num_rep);

  private:
    int Trig_pin;
    int Echo_pin;
	double Time_out;
    double distance_cm,distance_inc;
};

#endif
