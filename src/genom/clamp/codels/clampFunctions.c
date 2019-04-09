/*
 * clampFunctions.cc
 *
 *  Created on: 09/07/2014
 *      Author: goac
 */

#include "clampFunctions.h"

CLIENT* createClient(void){

	CLIENT* clnt;
	if ((clnt = clnt_create("localhost", CONTROLLER, CONTROLLERVERS, "tcp"))==NULL) {
		clnt_pcreateerror ("localhost");
		exit (1);
	}
	return clnt;
}

int setGrip(GRIP_STATE grip){
	CLIENT *client;
	int *result;

	double aperture = grip.open;


	client = createClient();
	result = setgripcontroller_1(&aperture, client);
	if (result == (int *) NULL) {
		clnt_perror (client, "call failed");
	}
	clnt_destroy (client);

	return *result;
}

int checkBarrier(GRIP_STATE grip){
	CLIENT *client;
	int *result;
	char *barrier;

	client = createClient();
	result = checkbarriercontroller_1((void*)&barrier, client);
	if (result == (int *) NULL) {
		clnt_perror (client, "call failed");
	}
	clnt_destroy (client);

	return *result;
}

COLORRGB getColor(void){
	CLIENT *client;
	COLORRGB *aux;
	int *result;
	char *arg;

	client = createClient();
	aux = getcolorcontroller_1((void*)&arg, client);
	if (aux == (COLORRGB *) NULL) {
		clnt_perror (client, "call failed");
	}
	clnt_destroy (client);

	return *aux;
}
