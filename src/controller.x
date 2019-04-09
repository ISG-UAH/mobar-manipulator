/* controller.x: 
 *	RPC code generator with rpcgen.
 *
 */


/*
 *	locomotionStruct.h definitions
 */

#ifndef LOCOMOTIONSTRUCT_H
#define	LOCOMOTIONSTRUCT_H
struct POSITION {
	float n[3];
	float o[3];
	float a[3];
	float p[3];
};

struct SERVOS {
	int num_servos;
	float q[5];
};

struct ANGLEROTATE {
	int axis;
	float angle;
};
#endif

/*
 *	clampStruct.h definitions
 */
#ifndef CLAMPSTRUCT_H
#define	CLAMPSTRUCT_H
struct GRIP_STATE{
	double open;
	int barrier;
};

struct COLORRGB{
	int red;
	int green;
	int blue;
	double tolerance;
	int idname;
	char code[7];
};
#endif


/*
 *	proximityStruct.h definitions
 */
#ifndef PROXIMITYSTRUCT_H
#define	PROXIMITYSTRUCT_H

struct PROXIMITYSENSORS {
	int num_left_sensors;
	double left_sensors[10];
	int num_right_sensors;
	double right_sensors[10];
	int num_top_sensors;
	double top_sensors[10];
	int num_back_sensors;
	double back_sensors[10];
};


#endif

/*
 *	functions to implement
 */
program CONTROLLER {
version CONTROLLERVERS {
   
            int INITCONTROLLER(void) = 1;
            int MOVECONTROLLER(POSITION) = 2;
            int MOVETOCONTROLLER(POSITION) = 3;
            int ROTATECONTROLLER(ANGLEROTATE) = 4;
            POSITION GETPOSCONTROLLER(void) = 5;
            SERVOS GETSERVOSCONTROLLER(void) = 6;
            int CHECKERROR(void) = 7;
            
            int SETGRIPCONTROLLER(double) = 8;
            int CHECKBARRIERCONTROLLER(void) = 9;
            COLORRGB GETCOLORCONTROLLER(void) = 10;
            PROXIMITYSENSORS GETSENSORSCONTROLLER(void) = 11;
	} = 1;
} = 0x20000001;
