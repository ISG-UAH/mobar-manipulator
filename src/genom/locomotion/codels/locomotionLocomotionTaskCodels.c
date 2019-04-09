/**
 ** locomotionLocomotionTaskCodels.cc
 **
 ** Codels called by execution task locomotionLocomotionTask
 **
 ** Author: 
 ** Date: 
 **
 **/

#include <portLib.h>
#include <string.h>
#include <math.h>
#include <controller.h>
#include "server/locomotionHeader.h"
#include "locomotionFunctions.h"
#include "CommonSs_Ops.h"


/*------------------------------------------------------------------------
 *
 * locomotionInit  --  Initialization codel (fIDS, ...)
 *
 * Description: 
 * 
 * Returns:    OK or ERROR
 */

STATUS
locomotionInit(int *report)
{
	CLIENT *clnt;
	POSITION pos, *res;
	STATUS ret_status=OK;
	int  *result;
	char *initcontroller_1_arg;

	clnt = createClient();
	result = initcontroller_1((void*)&initcontroller_1_arg, clnt);
	if (result == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	clnt_destroy (clnt);

	res = getPosition();
	pos = *res;

	// Update posters
	updatePosters();
	SDI_F->angle.angle = 0;
	SDI_F->angle.axis = 0;
	SDI_F->velocity = 3;

	// Create client to Proximity server.
	strcpy(SDI_F->MBproximity.mboxname, (char*)S_LOCOMOTION_NAME);
	SDI_F->MBproximity.idSsClient = S_LOCOMOTION;
	SDI_F->MBproximity.idSsTarget =  S_PROXIMITY; //S_CLAMP;

	ret_status = CommonClientStart(&SDI_F->MBproximity);
	if(ret_status != OK){
		printf("Error al iniciar cliente\n");
	}
	*report = ret_status;

  return ret_status;
}

/*------------------------------------------------------------------------
 *
 * locomotionEnd  --  Termination codel
 *
 * Description: 
 * 
 * Returns:    OK or ERROR
 */

STATUS
locomotionEnd(void)
{
	int report = OK;

	// Close connections
	report = CommonClientClose(&SDI_F->MBproximity);

	return report;
}

/*------------------------------------------------------------------------
 * MoveRef
 *
 * Description: 
 *
 * Reports:      OK
 *              S_locomotion_TOO_FAR_AWAY
 */

/* moveRefMain  -  codel EXEC of MoveRef
   Returns:  EXEC END ETHER ETHER ZOMBIE */
ACTIVITY_EVENT
moveRefMain(float movement[3], int *report)
{
	POSITION  pos;
	STATUS status;
	int i;

	pos = initPosition();
	for (i = 0; i<3; i++){
		pos.p[i] = movement[i];
	}

	status = movePosition(pos);
	if(status == OK){
		// Update posters
		updatePosters();
		*report = status;
		return ETHER;
	}else{
		*report = S_locomotion_TOO_FAR_AWAY;
		return ETHER;
	}

	return ETHER;
}

/*------------------------------------------------------------------------
 * MoveTo
 *
 * Description: 
 *
 * Reports:      OK
 *              S_locomotion_TOO_FAR_AWAY
 *              S_locomotion_LEFT_COLLISION
 *              S_locomotion_RIGHT_COLLISION
 *              S_locomotion_TOP_COLLISION
 *              S_locomotion_BACK_COLLISION
 */

/* moveToMain  -  codel EXEC of MoveTo
   Returns:  EXEC END ETHER ETHER ZOMBIE */
ACTIVITY_EVENT
moveToMain(float movement[3], int *report)
{
	POSITION  posNew, remain;
	STATUS status;
	int i, goalReached = 0, sign[3];

	/*
	 * Calculate next position adding SIZE_STEP to the present value.
	 * The size of the step depends linear on velocity.
	 */
	posNew = SDI_F->position;

	for (i = 0; i<3; i++){
		remain.p[i] = movement[i] - SDI_F->position.p[i];
		if(remain.p[i]>0){
			sign[i] = 1;
		}else{
			sign[i] = -1;
		}
		if(abs(remain.p[i]) < SIZE_STEP*SDI_F->velocity){
			posNew.p[i] = movement[i];
			goalReached++;
		}else{
			posNew.p[i] += sign[i]*SIZE_STEP*SDI_F->velocity;
		}
	}

	/*
	 * Check sensors in order to detect possible collisions. The codel needs to
	 * communicate with proximity sensor module. The array 'sign' is used to
	 * pre-calculate the next movement in cartesian coordinate system.
	 */
	// Check left
	if(sign[0]<0){
		status = CommonCheckLeftSensors(&SDI_F->MBproximity);
		if(status != OK || (SDI_F->MBproximity.report != OK && SDI_F->MBproximity.report != S_proximity_LEFT_NEAR_OBSTACLE)){
			*report = S_locomotion_LEFT_COLLISION;
			return ETHER;
		}
	}
	// Check right
	if(sign[0]>0){
		status = CommonCheckRightSensors(&SDI_F->MBproximity);
		if(status != OK || (SDI_F->MBproximity.report != OK && SDI_F->MBproximity.report != S_proximity_RIGHT_NEAR_OBSTACLE)){
			*report = S_locomotion_RIGHT_COLLISION;
			return ETHER;
		}
	}
	// Check back
	if(sign[1]<0){
		status = CommonCheckBackSensors(&SDI_F->MBproximity);
		if(status != OK || (SDI_F->MBproximity.report != OK && SDI_F->MBproximity.report != S_proximity_BACK_NEAR_OBSTACLE)){
			*report = S_locomotion_BACK_COLLISION;
			return ETHER;
		}
	}
	// Check top
	if(sign[2]>0){
		status = CommonCheckTopSensors(&SDI_F->MBproximity);
		if(status != OK || (SDI_F->MBproximity.report != OK && SDI_F->MBproximity.report != S_proximity_TOP_NEAR_OBSTACLE)){
			*report = S_locomotion_TOP_COLLISION;
			return ETHER;
		}
	}

	/*
	 * moveToPosition communicates with RPC program. Then check the answer.
	 */
	status = moveToPosition(posNew);

	if(status == OK){
		// Update posters
		updatePosters();
		*report = status;
	}else{
		*report = S_locomotion_TOO_FAR_AWAY;
		return END;
	}

	if(goalReached == 3){
		return END;
	}

	return EXEC;
}

/*------------------------------------------------------------------------
 * RotateRef
 *
 * Description: 
 *
 * Reports:      OK
 *              S_locomotion_TOO_FAR_AWAY
 */

/* rotateRefMain  -  codel EXEC of RotateRef
   Returns:  EXEC END ETHER ETHER ZOMBIE */
ACTIVITY_EVENT
rotateRefMain(ANGLEROTATE *angleparams, int *report)
{
	ANGLEROTATE target, newAng, actualAng;
	STATUS status;
	int sign = 1, goalReached = 0;

	actualAng = SDI_F->angle;
	target = *angleparams;

	newAng.axis = target.axis;
	SDI_F->angle.axis = target.axis;
	if(target.angle < 0){
		sign = -1;
	}
	newAng.angle = sign*ANG_STEP;

	if(fabsf(actualAng.angle) >= fabsf(target.angle)){
		newAng.angle = sign*abs(target.angle - actualAng.angle);
		goalReached = 1;
	}

	status = rotatePosition(newAng);
	if(status == OK){
		// Update posters
		updatePosters();
		SDI_F->angle.angle += newAng.angle;
		*report = status;
	}else{
		*report = S_locomotion_TOO_FAR_AWAY;
		SDI_F->angle.angle = 0;
		SDI_F->angle.axis  = 0;
		return ETHER;
	}

	if(goalReached == 1){
		SDI_F->angle.angle = 0;
		SDI_F->angle.axis  = 0;
		return ETHER;
	}else{
		return EXEC;
	}
}

/*------------------------------------------------------------------------
 * SetVel
 *
 * Description:
 *
 * Reports:      OK
 *              S_locomotion_TOO_HIGH_VELOCITY
 *              S_locomotion_TOO_LOW_SPEED
 */

/* SetVelMain  -  codel EXEC of SetVel
   Returns:  EXEC END ETHER FAIL ZOMBIE */
ACTIVITY_EVENT
SetVelMain(double *velocity, int *report)
{
	if(*velocity < MIN_SPEED){
		*report = S_locomotion_TOO_LOW_SPEED;
	}else if(*velocity > MAX_SPEED){
		*report = S_locomotion_TOO_HIGH_SPEED;
	}else{
		*report = OK;
		SDI_F->velocity = *velocity;
	}
	return ETHER;
}


/*------------------------------------------------------------------------
 * SetSpeed
 *
 * Description: 
 *
 * Reports:      OK
 *              S_locomotion_TOO_HIGH_SPEED
 *              S_locomotion_TOO_LOW_SPEED
 */

/* SetSpeedMain  -  codel EXEC of SetSpeed
   Returns:  EXEC END ETHER FAIL ZOMBIE */
ACTIVITY_EVENT
SetSpeedMain(double *velocity, int *report)
{
	if(*velocity < MIN_SPEED){
		*report = S_locomotion_TOO_LOW_SPEED;
	}else if(*velocity > MAX_SPEED){
		*report = S_locomotion_TOO_HIGH_SPEED;
	}else{
		*report = OK;
		SDI_F->velocity = *velocity;
	}
	return ETHER;
}


