/**
 ** proximityproximityTaskCodels.cc
 **
 ** Codels called by execution task proximityproximityTask
 **
 ** Author: 
 ** Date: 
 **
 **/

#include <portLib.h>

#include "server/proximityHeader.h"
#include "proximityFunctions.h"


/*------------------------------------------------------------------------
 *
 * proximityInit  --  Initialization codel (fIDS, ...)
 *
 * Description: 
 * 
 * Returns:    OK or ERROR
 */

STATUS
proximityInit(int *report)
{
	PROXIMITYSENSORS sensors;

	sensors = getAllSensors();
	UpdatePosters(sensors);

	*report = OK;

	return OK;
}

/*------------------------------------------------------------------------
 *
 * proximityEnd  --  Termination codel
 *
 * Description: 
 * 
 * Returns:    OK or ERROR
 */

STATUS
proximityEnd(void)
{
  return OK;
}

/*------------------------------------------------------------------------
 * checkLeftSensors
 *
 * Description: 
 *
 * Reports:      OK
 *              S_proximity_LEFT_NEAR_OBSTACLE
 *              S_proximity_LEFT_COLLISION
 */

/* checkLeftSensorsMain  -  codel EXEC of checkLeftSensors
   Returns:  EXEC END ETHER FAIL ZOMBIE */
ACTIVITY_EVENT
checkLeftSensorsMain(int *report)
{
	int i;
	PROXIMITYSENSORS sensors;

	sensors = getAllSensors();
	UpdatePosters(sensors);

	for(i = 0; i<sensors.num_left_sensors; i++){
		if(sensors.left_sensors[i] < COLLISION){
			*report = S_proximity_LEFT_COLLISION;
			return ETHER;

		}else if(sensors.left_sensors[i] < NEAR_OBSTACLE){
			*report = S_proximity_LEFT_NEAR_OBSTACLE;
			return ETHER;
		}
	}

	*report = OK;
	return ETHER;
}

/*------------------------------------------------------------------------
 * checkRightSensors
 *
 * Description: 
 *
 * Reports:      OK
 *              S_proximity_RIGHT_NEAR_OBSTACLE
 *              S_proximity_RIGHT_COLLISION
 */

/* checkRightSensorsMain  -  codel EXEC of checkRightSensors
   Returns:  EXEC END ETHER FAIL ZOMBIE */
ACTIVITY_EVENT
checkRightSensorsMain(int *report)
{
	int i;
	PROXIMITYSENSORS sensors;

	sensors = getAllSensors();
	UpdatePosters(sensors);

	for(i = 0; i<sensors.num_right_sensors; i++){
		if(sensors.right_sensors[i] < COLLISION){
			*report = S_proximity_RIGHT_COLLISION;
			return ETHER;

		}else if(sensors.right_sensors[i] < NEAR_OBSTACLE){
			*report = S_proximity_RIGHT_NEAR_OBSTACLE;
			return ETHER;
		}
	}

	*report = OK;
	return ETHER;
}

/*------------------------------------------------------------------------
 * checkTopSensors
 *
 * Description: 
 *
 * Reports:      OK
 *              S_proximity_TOP_NEAR_OBSTACLE
 *              S_proximity_TOP_COLLISION
 */

/* checkTopSensorsMain  -  codel EXEC of checkTopSensors
   Returns:  EXEC END ETHER FAIL ZOMBIE */
ACTIVITY_EVENT
checkTopSensorsMain(int *report)
{
	int i;
	PROXIMITYSENSORS sensors;

	sensors = getAllSensors();
	UpdatePosters(sensors);

	for(i = 0; i<sensors.num_top_sensors; i++){
		if(sensors.top_sensors[i] < COLLISION){
			*report = S_proximity_TOP_COLLISION;
			return ETHER;

		}else if(sensors.top_sensors[i] < NEAR_OBSTACLE){
			*report = S_proximity_TOP_NEAR_OBSTACLE;
			return ETHER;
		}
	}

	*report = OK;
	return ETHER;
}

/*------------------------------------------------------------------------
 * checkBackSensors
 *
 * Description: 
 *
 * Reports:      OK
 *              S_proximity_BACK_NEAR_OBSTACLE
 *              S_proximity_BACK_COLLISION
 */

/* checkBackSensorsMain  -  codel EXEC of checkBackSensors
   Returns:  EXEC END ETHER FAIL ZOMBIE */
ACTIVITY_EVENT
checkBackSensorsMain(int *report)
{
	int i;
	PROXIMITYSENSORS sensors;

	sensors = getAllSensors();
	UpdatePosters(sensors);

	for(i = 0; i<sensors.num_back_sensors; i++){
		if(sensors.back_sensors[i] < COLLISION){
			*report = S_proximity_BACK_COLLISION;
			return ETHER;

		}else if(sensors.back_sensors[i] < NEAR_OBSTACLE){
			*report = S_proximity_BACK_NEAR_OBSTACLE;
			return ETHER;
		}
	}

	*report = OK;
	return ETHER;
}


