
/* 
 * Copyright (c) 1993-2003,2012 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution and use  in source  and binary  forms,  with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice,  this list of  conditions and the following disclaimer in
 *      the  documentation  and/or  other   materials provided  with  the
 *      distribution.
 *
 * THIS  SOFTWARE IS PROVIDED BY  THE  COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND  ANY  EXPRESS OR IMPLIED  WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES  OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR  PURPOSE ARE DISCLAIMED. IN  NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR      CONTRIBUTORS  BE LIABLE FOR   ANY    DIRECT, INDIRECT,
 * INCIDENTAL,  SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE   OF THIS SOFTWARE, EVEN   IF ADVISED OF   THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

/*------------------  Fichier généré automatiquement ------------------*/
/*------------------  Ne pas éditer manuellement !!! ------------------*/

#ifndef proximity_MSG_LIB_H
#define proximity_MSG_LIB_H

#include "portLib.h"
#include "csLib.h"
#include "posterLib.h"
 
#include "proximityType.h"

#define TIME_WAIT_REPLY           /*2000*/ 0
 



/*----------- Tailles des messages et boites aux lettres  ----------*/

/* Taille du buffer utilise dans la bal pour un mesage de taille size
 * (cf servMbox pour UNIX et h2rgnLib pour VXWORKS )
 */


#define BUF_SIZE(size) \
   (size + 4 - (size & 3) + sizeof(LETTER_HDR) + 8)

/**
 ** Boite aux lettres reception requetes 
 **/
#define  PROXIMITY_MBOX_NAME                  genomInstanceName("proximity")

#define  PROXIMITY_MAX_RQST_SIZE              4

#define  PROXIMITY_MBOX_RQST_SIZE       \
 (BUF_SIZE(PROXIMITY_MAX_RQST_SIZE) * SERV_NMAX_RQST_ID)

/** 
 ** Taille max repliques (serveur)
 **/
#define  PROXIMITY_MAX_INTERMED_REPLY_SIZE   (sizeof(int))

#define  PROXIMITY_MAX_REPLY_SIZE             8

/** 
 ** Taille bal clients pour reception repliques 
 **/
#define  PROXIMITY_CLIENT_MBOX_REPLY_SIZE   \
((BUF_SIZE(PROXIMITY_MAX_REPLY_SIZE) \
   + BUF_SIZE(PROXIMITY_MAX_INTERMED_REPLY_SIZE)) \
 * CLIENT_NMAX_RQST_ID)

/*-------------------------- Liste des requetes  -------------------------*/

#define PROXIMITY_CHECKLEFTSENSORS_RQST 0
#define PROXIMITY_CHECKRIGHTSENSORS_RQST 1
#define PROXIMITY_CHECKTOPSENSORS_RQST 2
#define PROXIMITY_CHECKBACKSENSORS_RQST 3
#define PROXIMITY_ABORT_RQST 4


/*---------------- PROTOTYPES DES FONCTIONS EXTERNES ------------------*/

#ifdef __cplusplus
extern "C" {
#endif

STATUS proximityClientInit (CLIENT_ID *pClientId);

STATUS proximityClientEnd (CLIENT_ID clientId);

int proximityAbortRqstSend (CLIENT_ID clientId, 
			   int *pRqstId,
			   int *activity,
			   int replyTimeOut);

int proximityAbortReplyRcv (CLIENT_ID clientId, 
			   int rqstId, 
			   int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY */
			   int *bilan);

int proximityAbortRqstAndRcv (CLIENT_ID clientId, 
			     int *activity,
			     int *bilan);




extern STATUS proximitycheckLeftSensorsRqstSend (CLIENT_ID clientId, int *pRqstId,
					 
					 int replyTimeOut);

extern int proximitycheckLeftSensorsReplyRcv (CLIENT_ID clientId, int rqstId, 
				      int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY 
						     BLOCK_ON_INTERMED_REPLY */
				      
				      int *activity, int *bilan);


extern int proximitycheckLeftSensorsRqstAndAck (CLIENT_ID clientId, int *pRqstId,
					int replyTimeOut,
					 
					
					int *activity, int *bilan);

extern int proximitycheckLeftSensorsRqstAndRcv (CLIENT_ID clientId, 
					int replyTimeOut,
					 
					
					int *activity,
					int *bilan);




extern STATUS proximitycheckRightSensorsRqstSend (CLIENT_ID clientId, int *pRqstId,
					 
					 int replyTimeOut);

extern int proximitycheckRightSensorsReplyRcv (CLIENT_ID clientId, int rqstId, 
				      int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY 
						     BLOCK_ON_INTERMED_REPLY */
				      
				      int *activity, int *bilan);


extern int proximitycheckRightSensorsRqstAndAck (CLIENT_ID clientId, int *pRqstId,
					int replyTimeOut,
					 
					
					int *activity, int *bilan);

extern int proximitycheckRightSensorsRqstAndRcv (CLIENT_ID clientId, 
					int replyTimeOut,
					 
					
					int *activity,
					int *bilan);




extern STATUS proximitycheckTopSensorsRqstSend (CLIENT_ID clientId, int *pRqstId,
					 
					 int replyTimeOut);

extern int proximitycheckTopSensorsReplyRcv (CLIENT_ID clientId, int rqstId, 
				      int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY 
						     BLOCK_ON_INTERMED_REPLY */
				      
				      int *activity, int *bilan);


extern int proximitycheckTopSensorsRqstAndAck (CLIENT_ID clientId, int *pRqstId,
					int replyTimeOut,
					 
					
					int *activity, int *bilan);

extern int proximitycheckTopSensorsRqstAndRcv (CLIENT_ID clientId, 
					int replyTimeOut,
					 
					
					int *activity,
					int *bilan);




extern STATUS proximitycheckBackSensorsRqstSend (CLIENT_ID clientId, int *pRqstId,
					 
					 int replyTimeOut);

extern int proximitycheckBackSensorsReplyRcv (CLIENT_ID clientId, int rqstId, 
				      int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY 
						     BLOCK_ON_INTERMED_REPLY */
				      
				      int *activity, int *bilan);


extern int proximitycheckBackSensorsRqstAndAck (CLIENT_ID clientId, int *pRqstId,
					int replyTimeOut,
					 
					
					int *activity, int *bilan);

extern int proximitycheckBackSensorsRqstAndRcv (CLIENT_ID clientId, 
					int replyTimeOut,
					 
					
					int *activity,
					int *bilan);


#ifdef __cplusplus
}
#endif
#endif /* proximity_MSG_LIB_H */

