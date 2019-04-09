#include <locomotion.h>
#include <communication.h>
#include <controller.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(){
	locomotion loc;
	communication com;
	POSITION pos;
	SERVOS serv;
	ANGLEROTATE rot;
	char *command;
	int i;
	double p[3];

	pos = loc.GetPosition();

	serv = loc.GetServos();

	p[0] = 0; p[1] = -2; p[2]=0;
	loc.Move(p);
	command = com.sendArduino(loc);
/*
	p[0] = -4; p[1] = 0; p[2]=0;
	loc.Move(p);
	command = com.sendArduino(loc);
*/
	rot.angle = 0.9; rot.axis = 0;
	loc.Rotate(rot);
	command = com.sendArduino(loc);

	rot.angle = -0.6; rot.axis = 0;
	loc.Rotate(rot);
	command = com.sendArduino(loc);

	rot.angle = 0.2; rot.axis = 2;
	loc.Rotate(rot);
	command = com.sendArduino(loc);

	command = com.sendArduino(serv);
	loc.SetServos(serv);

	return 0;

}
