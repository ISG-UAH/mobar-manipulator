/*
 * proximitysensors.h
 *
 *  Created on: 14/07/2014
 *      Author: goac
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef PROXIMITYSTRUCT_H
#define	PROXIMITYSTRUCT_H

typedef struct PROXIMITYSENSORS {
	int num_left_sensors;
	double left_sensors[10];
	int num_right_sensors;
	double right_sensors[10];
	int num_top_sensors;
	double top_sensors[10];
	int num_back_sensors;
	double back_sensors[10];
}PROXIMITYSENSORS;
#endif


#ifndef PROXIMITYSENSORS_H_
#define PROXIMITYSENSORS_H_

#define NUM_SENSORS 8
#define LEFT_SENSORS 3
#define RIGHT_SENSORS 3
#define TOP_SENSORS 1
#define BACK_SENSORS 1

int getProximitySensors(double *data, PROXIMITYSENSORS &sensors);
int stricmp (const char *p1, const char *p2);



#endif /* PROXIMITYSENSORS_H_ */
