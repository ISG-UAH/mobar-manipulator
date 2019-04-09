/*
 * proximityFunctions.cc
 *
 *  Created on: 14/07/2014
 *      Author: goac
 */
#include "proximityFunctions.h"


CLIENT* createClient(void){

	CLIENT* clnt;
	if ((clnt = clnt_create("localhost", CONTROLLER, CONTROLLERVERS, "tcp"))==NULL) {
		clnt_pcreateerror ("localhost");
		exit (1);
	}
	return clnt;
}

PROXIMITYSENSORS getAllSensors(void){
	CLIENT *client;
	PROXIMITYSENSORS  *result;
	char *getsens_arg;

	client = createClient();
	result = getsensorscontroller_1((void*)&getsens_arg, client);
	if (result == (PROXIMITYSENSORS *) NULL) {
		clnt_perror (client, "call failed");
	}
	clnt_destroy (client);

	return *result;
}

void UpdatePosters(PROXIMITYSENSORS sensors){

	SDI_F->sensors = sensors;
}
