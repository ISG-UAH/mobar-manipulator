/*
 * CommonSs_Ops.h
 *
 *  Created on: 15/07/2014
 *      Author: goac
 */

#ifndef COMMONSS_OPS_H_
#define COMMONSS_OPS_H_

#include <stdio.h>
#include "SubSystem_Defs.h"
#include "locomotion/server/locomotionMsgLib.h"
#include "proximity/server/proximityMsgLib.h"
#include "clamp/server/clampMsgLib.h"

#define REPLY_TIME_OUT 1

/** Create and initialize a mailbox with the defined module to send commands.
 * @param MailBOX struct with the module associated data.
 * @return OK when correct, ERROR otherwise.
 */
STATUS CommonClientStart(MailBOX* mBox);

/** Close the mailbox.
 * @param MailBOX struct with the module associated data.
 * @return OK when correct, ERROR otherwise.
 */
STATUS CommonClientClose(MailBOX* mBox);

/** Call to Proximity module left sensors.
 * @param MailBOX struct with the module associated data.
 * @return OK when correct, ERROR otherwise.
 */
STATUS CommonCheckLeftSensors(MailBOX* mBox);

/** Call to Proximity module right sensors.
 * @param MailBOX struct with the module associated data.
 * @return OK when correct, ERROR otherwise.
 */
STATUS CommonCheckRightSensors(MailBOX* mBox);

/** Call to Proximity module top sensors.
 * @param MailBOX struct with the module associated data.
 * @return OK when correct, ERROR otherwise.
 */
STATUS CommonCheckTopSensors(MailBOX* mBox);

/** Call to Proximity module back sensors.
 * @param MailBOX struct with the module associated data.
 * @return OK when correct, ERROR otherwise.
 */
STATUS CommonCheckBackSensors(MailBOX* mBox);

#endif /* COMMONSS_OPS_H_ */
