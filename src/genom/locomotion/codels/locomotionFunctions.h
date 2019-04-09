/*
 * locomotionFunctions.h
 *
 *  Created on: 30/06/2014
 *      Author: goac
 */

#ifndef LOCOMOTIONFUNCTIONS_H_
#define LOCOMOTIONFUNCTIONS_H_

#include <portLib.h>
#include <string.h>
#include "server/locomotionHeader.h"
#include <controller.h>

#define SIZE_STEP 	1
#define ANG_STEP 	0.1
#define MAX_SPEED 	10
#define MIN_SPEED 	1

	CLIENT* createClient(void);

	POSITION initPosition();

	STATUS movePosition(POSITION pos);

	STATUS moveToPosition(POSITION pos);

	STATUS rotatePosition(ANGLEROTATE ang);

	POSITION* getPosition(void);

	SERVOS* getServos(void);

	STATUS updatePosters(void);

	STATUS checkError(void);

#endif /* LOCOMOTIONFUNCTIONS_H_ */
