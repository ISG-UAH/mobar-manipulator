/* 
 * File:   locomotionStruct.h
 * Author: Juan Alejandro Mora Prieto
 *
 * Created on June 17th, 2014
 */
#include "../SubSystem_Defs.h"

#ifndef LOCOMOTIONSTRUCT_H
#define	LOCOMOTIONSTRUCT_H

typedef struct POSITION {
	float n[3];
	float o[3];
	float a[3];
	float p[3];
}POSITION;

typedef struct SERVOS {
	int num_servos;
	float q[5];
}SERVOS;

typedef struct ANGLEROTATE {
	int axis;
	float angle;
}ANGLEROTATE;

#endif	/* LOCOMOTIONSTRUCT_H */
