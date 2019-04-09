/**
 ** clampClampTaskCodels.cc
 **
 ** Codels called by execution task clampClampTask
 **
 ** Author: 
 ** Date: 
 **
 **/

#include <portLib.h>
#include "server/clampHeader.h"
#include <string.h>
#include <controller.h>
#include "clampFunctions.h"
#include <math.h>


/*------------------------------------------------------------------------
 *
 * clampInit  --  Initialization codel (fIDS, ...)
 *
 * Description: 
 * 
 * Returns:    OK or ERROR
 */

STATUS
clampInit(int *report)
{
	GRIP_STATE grip;
	int ret;

	grip.open = MAX_APERTURE;
	setGrip(grip);
	SDI_F->grip.open = grip.open;

	ret = checkBarrier(grip);
	SDI_F->grip.barrier = ret;
	*report = OK;
	return OK;
}

/*------------------------------------------------------------------------
 *
 * clampEnd  --  Termination codel
 *
 * Description: 
 * 
 * Returns:    OK or ERROR
 */

STATUS
clampEnd(void)
{
  return OK;
}

/*------------------------------------------------------------------------
 * OpenGrip
 *
 * Description: 
 *
 * Reports:      OK
 */

/* openGripMain  -  codel EXEC of OpenGrip
   Returns:  EXEC END ETHER FAIL ZOMBIE */
ACTIVITY_EVENT
openGripMain(int *report)
{
	GRIP_STATE grip;
	grip.open = MAX_APERTURE;
	setGrip(grip);

	SDI_F->grip.open = grip.open;

	*report = OK;

	return ETHER;
}

/*------------------------------------------------------------------------
 * CloseGrip
 *
 * Description: 
 *
 * Reports:      OK
 */

/* closeGripMain  -  codel EXEC of CloseGrip
   Returns:  EXEC END ETHER FAIL ZOMBIE */
ACTIVITY_EVENT
closeGripMain(int *report)
{
	GRIP_STATE grip;
	grip.open = MIN_APERTURE;
	setGrip(grip);

	SDI_F->grip.open = grip.open;

	*report = OK;

	return ETHER;
}

/*------------------------------------------------------------------------
 * CheckBarrier
 *
 * Description: 
 *
 * Reports:      OK
 */

/* checkBarrierMain  -  codel EXEC of CheckBarrier
   Returns:  EXEC END ETHER FAIL ZOMBIE */
ACTIVITY_EVENT
checkBarrierMain(int *report)
{
	int ret;
	GRIP_STATE grip;

	ret = checkBarrier(grip);

	SDI_F->grip.barrier = ret;

	*report = OK;

	return ETHER;
}

/*------------------------------------------------------------------------
 * GetObject
 *
 * Description: 
 *
 * Reports:      OK
 *              S_clamp_COLOR_NOT_DETECTED
 */

/* getObjectMain  -  codel EXEC of GetObject
   Returns:  EXEC END ETHER FAIL ZOMBIE */
ACTIVITY_EVENT
getObjectMain(int *report)
{
	COLORRGB object;

	object = getColor();

	SDI_F->object = object;

	*report = OK;

	return ETHER;
}


