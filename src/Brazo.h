#ifndef BRAZO_H
#define BRAZO_H

#define TIMEOUT 1000000
#define MAX_SPEED 12

/* Cinematica Directa e Inversa - Orocos KDL */
#include <kdl/tree.hpp>
#include <kdl/chain.hpp>
#include <kdl/chainfksolver.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/frames_io.hpp>
#include <kdl/frames.hpp>
#include <kdl/chainiksolver.hpp>
#include <kdl/chainiksolvervel_pinv.hpp>
#include <kdl/chainiksolvervel_wdls.hpp>
#include <kdl/chainiksolverpos_nr.hpp>
#include <kdl/chainiksolverpos_nr_jl.hpp>

#include "chainiksolverpos_nr_jl_we.hpp"

#include "../../brazoStruct.h"
#include "RobotDH.h"
#include "Serial.h"

#include <stdio.h>
#include <iostream>
#include <strings.h>
#include <sys/time.h>

/*
#ifdef use_namespace
namespace ROBOOP {
  using namespace NEWMAT;
#endif
*/

using namespace KDL;
using namespace std;

class Brazo {

public:
	  Brazo();
	  Brazo(char* filename);
	  ~Brazo();

	  int Config(char* filename);

	  /* Desplazamientos y Rotacion */
	  int Move(double posicion[3]);
	  int Clamp(double apertura);
	  int Rotate(int eje, double angulo); // Eje 0 (alpha), 1 (theta), 2 (psi)
	  BRAZO_SERVOS GetPosition(void);

	  /* Velocidad */
	  int SetVel(BRAZO_VELOCITY speed);

	  /* Consulta de variables */
	  BRAZO_SERVOS GetServos(void);
	  BRAZO_COORD GetCoord(void);
	  BRAZO_VELOCITY GetVel(void);

	  /* Impresion de parametros por pantalla */
	  void print();

private:
	  Chain chain;
	  Frame coordBrazo;
	  JntArray qServos, qMin, qMax;
	  double qPinza, speed;
	  int serial_fd;

	  /* Inicializar Arduino por puerto Serial */
	  int initArduino();
	  int initArduino(char *portArduino);

	  /* Desplazamientos y Rotacion */
	  int InvKine();
	  double ConvertSpeed(BRAZO_VELOCITY vel);
	  BRAZO_VELOCITY ConvertSpeed(double vel);

	  /* Comunicacion con placa Arduino */
	  int sendCommand(char command[100]);

	  /* Conversion de coordenadas */
	  JntArray ServosToJnt(BRAZO_SERVOS qInt);
	  BRAZO_SERVOS JntToServos(void);
	  JntArray QLimites(double limites[6]);
	  BRAZO_COORD FrameToCoord(void);

	  /* Impresion de parametros por pantalla */
	  void print(Frame coordBrazo);
	  void print(JntArray servos);



}; // End class brazo


/*
#ifdef use_namespace
}
#endif
*/

#endif
