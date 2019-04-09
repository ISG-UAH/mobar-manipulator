/*
 * communication.cpp
 *
 *  	Created on: 17/06/2014
 *      Author: Juan Alejandro Mora Prieto
 */
#include "communication.h"

communication::communication(){
	char port[100];
	FILE *fp;

	log();
	system("sudo service portmap stop; sudo rpcbind -wi; sudo service portmap start");
	fp = popen("/bin/ls /dev/ttyACM*", "r");
	if(fp == NULL){
		sub1->error("COM: POPEN: Error executing command");
		exit;
	}

	fscanf(fp, "%s", port);
	sub1->debug("COM: Port %s.", port);
	serial_fd = serial_open(port, B57600);

	if (serial_fd==-1) {
		sub1->error("COM: OPEN: Error connecting with Arduino.");
		exit(1);
	}else{
		sub1->debug("COM: Communication starts. Port %d open.", serial_fd);
	}

}

communication::~communication(){
	int ret;

	ret = serial_close(serial_fd);

	sub1->debug("COM: Communication ends. Port %d closed.", serial_fd);
	log4cpp::Category::shutdown();
}

void communication::log(){

    string initFileName = "log4cpp.properties";
    log4cpp::PropertyConfigurator::configure(initFileName);

    root = &log4cpp::Category::getRoot();
    sub1 = &log4cpp::Category::getInstance(std::string("sub1"));

    /*
    root->warn("Storm is coming");
    sub1->debug("Received storm warning");
    sub1->info("Closing all hatches");
    sub2->error("Solar panels are blocked");
    */
}

int communication::sendArduino(locomotion loc){

	char command[50], *com_rec;
	int i, count, ret, read;
	SERVOS servos;

	if((ret = loc.GetServos(servos))==0){

		count = sprintf(command, "MoveTo:");

		for(i = 0; i<servos.num_servos; i++){
			count += sprintf(command, "%s %1.3f",command,  servos.q[i]);
		}

		count += sprintf(command, "%s;", command);

		do{
			sendCommand(command);
			read = readCommand(com_rec);
			if(checkCommandRcv(command, com_rec) != 0){
				read = -1;
			}
		}while(read == -1);
	}

	return ret;
}

char * communication::sendArduino(SERVOS &servos){

	char command[50], *com_rec;
	int i, count, ret, read;

	count = sprintf(command, "GetPos:;");

	do{
		sendCommand(command);
		read = readCommand(com_rec);
		if(checkCommandRcv(command, com_rec) != 0){
			read = -1;
		}
	}while(read == -1);

	servos = commandToServos(com_rec);
	return com_rec;
}


int communication::sendArduinoGrip(GRIP_STATE grip){
	char command[50], *com_rec;
	int count, read;

	count  = sprintf(command, "MoveServo: 5");
	count += sprintf(command, "%s %1.3f;",command,  grip.open);

	do{
		sendCommand(command);
		read = readCommand(com_rec);
		if(checkCommandRcv(command, com_rec) != 0){
			read = -1;
		}
	}while(read == -1);

	return read;
}

int communication::sendArduinoBarrier(GRIP_STATE &grip){
	char command[50], *com_rec, *param;
	int count, read;

	count  = sprintf(command, "GetBarrier:;");

	do{
		sendCommand(command);
		read = readCommand(com_rec);
		if(checkCommandRcv(command, com_rec) != 0){
			read = -1;
		}
	}while(read == -1);

	com_rec = strtok(com_rec, ": ");
	if (stricmp(com_rec, "GetBarrier") == 0)
	{
		param = strtok (NULL, ";");
		grip.barrier = atoi(param);
	}

	return read;
}

int communication::sendArduinoColor(COLORRGB &color){

	char command[50], *com_rec, *param, *com_aux;
	int count, read, i=0;
	//int *rgb;
	int rgb[3];


	count  = sprintf(command, "GetColor:;");

	do{
		sendCommand(command);
		read = readCommand(com_rec, 5);
		if(checkCommandRcv(command, com_rec) != 0){
			read = -1;
		}
	}while(read == -1);

	com_aux = strtok(com_rec, ";");
	com_aux = strtok(com_aux, ": ");
	if (stricmp(com_aux, "GetColor") == 0)
	{
		for(i = 0; i<3; i++){
			param = strtok(NULL, " ");
			rgb[i]=atoi(param);
		}
		getColor(rgb, color);
	}


	return read;
}
int communication::sendArduinoSensors(PROXIMITYSENSORS &sensors){
	char command[50], *com_rec, *com_aux, *param;
	int count, read, i=0, ret;
	double data[100];

	count  = sprintf(command, "GetProximity:;");

	do{
		sendCommand(command);
		read = readCommand(com_rec);
		if(checkCommandRcv(command, com_rec) != 0){
			read = -1;
		}
	}while(read == -1);

	com_aux = strtok(com_rec, ";");
	com_aux = strtok(com_aux, ": ");
	if (stricmp(com_aux, "GetProximity") == 0)
	{
		while ((param = strtok (NULL, " ")) != NULL)
		  {
			data[i]=atof(param);
			i++;
		  }
	}

	ret = getProximitySensors(data, sensors);
	return read;
}



void communication::sendCommand(char* command){
	sub1->debug("COM: Send Command: %s", command);
	serial_send(serial_fd, command, strlen(command));
}

int communication::readCommand(char* &data_rcv){
	int ret;
	ret = readCommand(data_rcv,1);
	return ret;
}

int communication::readCommand(char* &data_rcv, int multTimeout){
	int ret;
	timeval tim;
	double t_init, t_end;
	char data[1024];

	do
	{
		gettimeofday(&tim, NULL);
		t_init =tim.tv_sec+(tim.tv_usec/1000000.0);

		ret = serial_read(serial_fd, data, 1024, multTimeout*TIMEOUT);
		gettimeofday(&tim, NULL);
		t_end =tim.tv_sec+(tim.tv_usec/1000000.0);

		if(t_end - t_init > multTimeout*TIMEOUT/100000.0){
		 sub1->error("COM: Closing port %d : exceeded TIMEOUT.", serial_fd);
		 return -1;
		}
	}while(ret == 0);

	sub1->debug("COM: Received Command [%1.3f s]: %s", (t_end - t_init), data);

	data_rcv = data;
	return ret;
}

int communication::get_fd(void){
	return serial_fd;
}

SERVOS communication::commandToServos(char * command){
	SERVOS serv;
	char *comando, *param;
	int i=0;

	comando = strtok(command, ": ");
	if (stricmp(comando, "GetPos") == 0)
	{
		while ((param = strtok (NULL, " ")) != NULL)
		  {
			serv.q[i]=atof(param);
			i++;
		  }
	}

	serv.num_servos = i;
	return serv;
}

int communication::checkCommandRcv(char * com_send, char * com_rcv){
	int ret;
	char com_send_aux[1000], com_rcv_aux[1000];
	char *com1, *com2;

	strcpy(com_rcv_aux, com_rcv);
	strcpy(com_send_aux, com_send);

	com1 = strtok(com_send_aux, ":");
	com2 = strtok(com_rcv_aux, ":");
	ret = stricmp(com_send_aux, com_rcv_aux);

	return ret;
}



// STRCMP from standard C++ Library, but case insensitive
int communication::stricmp (const char *p1, const char *p2)
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
