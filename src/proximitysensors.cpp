/*
 * proximitysensors.cpp
 *
 *  Created on: 14/07/2014
 *      Author: goac
 */
#include "proximitysensors.h"

int getProximitySensors(double *data, PROXIMITYSENSORS &sensors){
	char *com_aux, *param;
	int i=10, ret, offset;
	double sensorValues[NUM_SENSORS];
	PROXIMITYSENSORS aux;

	for(i=0; i<NUM_SENSORS; i++){
		sensorValues[i] = *(data+i);
	}

	ret = i;
	// Check if amount of sensors is at least equal to NUM_SENSORS constant.
	// else, return (-1) error.
	if(i < NUM_SENSORS){
		return -1;
	}

	offset = 0;

	// Left Sensors
	for(i = 0; i<LEFT_SENSORS; i++){
		aux.left_sensors[i] = sensorValues[i+offset];
	}
	aux.num_left_sensors = LEFT_SENSORS;
	offset += LEFT_SENSORS;

	// Right Sensors
	for(i = 0; i<RIGHT_SENSORS; i++){
		aux.right_sensors[i] = sensorValues[i+offset];
	}
	aux.num_right_sensors = RIGHT_SENSORS;
	offset += RIGHT_SENSORS;

	// Back Sensors
	for(i = 0; i<BACK_SENSORS; i++){
		aux.back_sensors[i] = sensorValues[i+offset];
	}
	aux.num_back_sensors = BACK_SENSORS;
	offset += BACK_SENSORS;

	// Top Sensors
	for(i = 0; i<TOP_SENSORS; i++){
		aux.top_sensors[i] = sensorValues[i+offset];
	}
	aux.num_top_sensors = TOP_SENSORS;
	offset += TOP_SENSORS;



	sensors = aux;

	return ret;
}

// STRCMP from standard C++ Library, but case insensitive
int stricmp (const char *p1, const char *p2)
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
