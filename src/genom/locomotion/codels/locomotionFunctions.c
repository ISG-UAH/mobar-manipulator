/*
 * locomotionFunctions.cc
 *
 *  Created on: 30/06/2014
 *      Author: goac
 */
#include "locomotionFunctions.h"

CLIENT* createClient(void){

	CLIENT* clnt;
	if ((clnt = clnt_create("localhost", CONTROLLER, CONTROLLERVERS, "tcp"))==NULL) {
		clnt_pcreateerror ("localhost");
		exit (1);
	}
	return clnt;
}

POSITION initPosition(){
	POSITION pos;
	int i;

	for (i=0; i<3; i++){
		pos.n[i] = 0; pos.o[i] = 0; pos.a[i] = 0; pos.p[i] = 0;
	}
	pos.n[0] = 1; pos.o[1] = 1; pos.a[2] = 1;
	return pos;
}

STATUS movePosition(POSITION pos){
	CLIENT *client;
	STATUS status;
	int *result;

	client = createClient();
	result = movecontroller_1(&pos, client);
	if (result == (int *) NULL) {
		clnt_perror (client, "call failed");
		status = ERROR;
	}else{
		status = OK;
	}

	clnt_destroy (client);

	return status;
}

STATUS moveToPosition(POSITION pos){
	CLIENT *client;
	STATUS status;
	int *result;

	client = createClient();
	result = movetocontroller_1(&pos, client);
	if (result == (int *) NULL) {
		clnt_perror (client, "call failed");
		status = ERROR;
	}else if (*result == -1){
		status = ERROR;
	}else{
		status = OK;
	}

	clnt_destroy (client);

	return status;
}

STATUS rotatePosition(ANGLEROTATE ang){
	CLIENT *client;
	STATUS status;
	int  *result;

	client = createClient();
	result = rotatecontroller_1(&ang, client);
	if (result == (int *) NULL) {
		clnt_perror (client, "call failed");
		status = ERROR;
	}else if (*result == -1){
			status = ERROR;
	}else{
		status = OK;
	}

	clnt_destroy (client);

	return status;
}

POSITION* getPosition(void){
	CLIENT *client;
	POSITION *result;
	char *getpos_arg;

	client = createClient();
	result = getposcontroller_1((void*)&getpos_arg, client);
	if (result == (POSITION *) NULL) {
		clnt_perror (client, "call failed");
	}
	clnt_destroy (client);
	return result;
}

SERVOS* getServos(void){
	CLIENT *client;
	SERVOS  *result;
	char *getservoscontroller_1_arg;

	client = createClient();
	result = getservoscontroller_1((void*)&getservoscontroller_1_arg, client);
	if (result == (SERVOS *) NULL) {
		clnt_perror (client, "call failed");
	}
	clnt_destroy (client);

	return result;
}

STATUS updatePosters(void){
	// Update posters
	SDI_F->position = *getPosition();
	SDI_F->servos = *getServos();

	return OK;
}

STATUS checkError(void){
	CLIENT *client;
	STATUS status;
	int *result;
	char *error;

	client = createClient();
	result = checkerror_1((void*)&error, client);
	if (result == (int *) NULL) {
		clnt_perror (client, "call failed");
		status = ERROR;
	}else{
		status = OK;
	}
	clnt_destroy (client);

	return status;

}
