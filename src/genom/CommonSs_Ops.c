/*
 * CommonSs_Ops.cc
 *
 *  Created on: 15/07/2014
 *      Author: goac
 */
#include "CommonSs_Ops.h"

/*
 * This global variable is used to store CLIENT_ID data struct, because
 * GenoM can not create Test program if there are an uncommon data type
 * in module's internal struct.
 */
CLIENT_ID cliente;


STATUS CommonClientStart(MailBOX* mBox)
{
	STATUS status;

	int* activity, report;

	// Create the MailBox
	status = csMboxInit(mBox->mboxname, mBox->idSsClient, PROXIMITY_CLIENT_MBOX_REPLY_SIZE);
    if(status != OK)
    {
        printf(">>> CommonClientStart Error %d: %s cannot create a MailBox.\n", status, mBox->mboxname);
        return ERROR;
    }else
    {
    	printf("CommonClientStart: %s creates a Mailbox.\n", mBox->mboxname);
    }

    // Initialize the client
    switch(mBox->idSsTarget){
		case S_LOCOMOTION:
			status = locomotionClientInit(&cliente);
			if(status != OK)
			{
				printf(">>> Cannot init %s client for %s\n",(char*)S_LOCOMOTION_NAME, mBox->mboxname);
				status = locomotionClientEnd(cliente);
				return ERROR;
			}
			break;

		case S_CLAMP:
			status = clampClientInit(&cliente);
			if(status != OK)
			{
				printf(">>> Cannot init %s client for %s\n",(char*)S_CLAMP_NAME, mBox->mboxname);
				status = clampClientEnd(cliente);
				return ERROR;
			}
			break;
		case S_PROXIMITY:
			status = proximityClientInit(&cliente);

			if(status != OK)
			{
				printf(">>> Cannot init %s client for %s\n",(char*)S_PROXIMITY_NAME, mBox->mboxname);
				status = proximityClientEnd(cliente);
				return ERROR;
			}
			break;
		default:
			printf(">>> Client %s not found.\n", cliente);
			status = csMboxEnd();
			return ERROR;
			break;
    }

    return status;
}

STATUS CommonClientClose(MailBOX* mBox)
{
    // Close connection with locomotion module
    if(csClientEnd(cliente) != OK || csMboxEnd() != OK )
    {
        printf ("Problem closing %s mbox.\n", cliente);
        return ERROR;
    }
    return OK;
}

STATUS CommonCheckLeftSensors(MailBOX* mBox){
	STATUS error;

	error =  proximitycheckLeftSensorsRqstAndRcv(cliente, REPLY_TIME_OUT, &mBox->activity, &mBox->report);

	if(error != FINAL_REPLY_OK){
		return ERROR;
	}

	return OK;
}

STATUS CommonCheckRightSensors(MailBOX* mBox){
	STATUS error;

	error =  proximitycheckRightSensorsRqstAndRcv(cliente, REPLY_TIME_OUT, &mBox->activity, &mBox->report);

	if(error != FINAL_REPLY_OK){
		return ERROR;
	}

	return OK;

}

STATUS CommonCheckTopSensors(MailBOX* mBox){
	STATUS error;

	error =  proximitycheckTopSensorsRqstAndRcv(cliente, REPLY_TIME_OUT, &mBox->activity, &mBox->report);

	if(error != FINAL_REPLY_OK){
		return ERROR;
	}

	return OK;

}

STATUS CommonCheckBackSensors(MailBOX* mBox){
	STATUS error;

	error =  proximitycheckBackSensorsRqstAndRcv(cliente, REPLY_TIME_OUT, &mBox->activity, &mBox->report);

	if(error != FINAL_REPLY_OK){
		return ERROR;
	}

	return OK;

}
