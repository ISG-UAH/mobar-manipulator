/*
 * communication.h
 *
 *  	Created on: 17/06/2014
 *      Author: Juan Alejandro Mora Prieto
 */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#define TIMEOUT 100000 //us

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include "Serial.h"
#include "controller.h"
#include "locomotion.h"
#include "clamp.h"
#include "proximitysensors.h"
// GenoM modules
//#include "locomotion/locomotionStruct.h"
//#include "clamp/clampStruct.h"

using namespace std;

class communication {

public:
		communication();
		~communication();

		int sendArduino(locomotion loc);
		char * sendArduino(SERVOS &servos);

		int sendArduinoGrip(GRIP_STATE grip);
		int sendArduinoBarrier(GRIP_STATE &grip);
		int sendArduinoColor(COLORRGB &color);
		int sendArduinoSensors(PROXIMITYSENSORS &sensors);

		int get_fd(void);
private:
		int serial_fd;
		log4cpp::Category *root, *sub1;

		void sendCommand(char command[100]);
		int readCommand(char * &data_rcv);
		int readCommand(char* &data_rcv, int multTimeout);
		SERVOS commandToServos(char * command);
		int checkCommandRcv(char * com_send, char * com_rcv);
		int stricmp (const char *p1, const char *p2);
		void log();

}; // End class communication

#endif
