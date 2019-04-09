/**
 ** proximityproximityTaskCodels.c
 **
 ** Codels called by execution task proximityproximityTask
 **
 ** Author: 
 ** Date: 
 **
 **/

#include <portLib.h>

#include "server/proximityHeader.h"


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
  /* ... add your code here ... */
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
  /* ... add your code here ... */
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
  /* ... add your code here ... */
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
  /* ... add your code here ... */
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
  /* ... add your code here ... */
  return ETHER;
}


