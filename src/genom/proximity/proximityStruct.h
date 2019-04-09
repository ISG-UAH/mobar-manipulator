/* 
 * File:   proximityStruct.h
 * Author: Juan Alejandro Mora Prieto
 *
 * Created on July 14th, 2014
 */
//#include "../SubSystem_Defs.h"

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


#endif	/* PROXIMITYSTRUCT_H */
