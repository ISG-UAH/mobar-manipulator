/*
 * proximityFunctions.h
 *
 *  Created on: 14/07/2014
 *      Author: goac
 */

#ifndef PROXIMITYFUNCTIONS_H_
#define PROXIMITYFUNCTIONS_H_

#include <portLib.h>
#include <string.h>
#include "server/proximityHeader.h"
#include <controller.h>

#define NEAR_OBSTACLE 	600
#define COLLISION		200

	CLIENT* createClient(void);

	PROXIMITYSENSORS getAllSensors(void);
	void UpdatePosters(PROXIMITYSENSORS sensors);


#endif /* PROXIMITYFUNCTIONS_H_ */
