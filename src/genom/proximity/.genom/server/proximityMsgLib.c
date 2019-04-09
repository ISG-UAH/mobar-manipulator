
/* 
 * Copyright (c) 1993-2005 LAAS/CNRS
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

/* --- Message-based communication for module proximity --------------- */

#if defined(__RTAI__) && defined(__KERNEL__)
# include <linux/kernel.h>
#else
# include <stdio.h>
# include <stddef.h>
#endif

#include <portLib.h>
#include <errnoLib.h>

#include <genom/genom.h>

#include "proximityMsgLib.h"
#include "proximityError.h"
 
/*========================================================================
 *
 *  proximityClientInit  -  Routine d'initialisation d'un client de proximity
 *
 *  Description:
 *  Cette routine  doit etre appelee par un client du module, pour pouvoir 
 *  l'utiliser. Attention: la fonction csMboxInit doit etre appelee avant.
 *
 *  Retourne: OK ou ERROR
 */
 
STATUS proximityClientInit (CLIENT_ID *pClientId)

{
  STATUS status;

  /* Appeler la routine d'initialisation d'un client CS */
  status = csClientInit (PROXIMITY_MBOX_NAME, PROXIMITY_MAX_RQST_SIZE,
			 PROXIMITY_MAX_INTERMED_REPLY_SIZE, 
			 PROXIMITY_MAX_REPLY_SIZE, 
			pClientId);
  if (status != OK) 
    h2perror("proximityClientInit");

  proximityRecordH2errMsgs();

  return status;
}
 
/*-------------------------------------------------------------------------
 *
 *  proximityClientEnd - Routine de fin d'un client de proximity
 *
 *  Description:
 *  Cette routine libere les objets alloues par le client.
 *
 *  Retourne : OK ou ERROR
 */

STATUS proximityClientEnd (CLIENT_ID clientId)         /* Id. du client */
     
{
  STATUS status;
  /* Appeler la routine de liberation du client */
  status = csClientEnd (clientId);
  if (status != OK) 
    h2perror("proximityClientEnd");
  return status;
}


/*========================================================================
 *
 *  proximityAbortRqstSend  -  Emission d'une requete de controle
 *
 *  Description: Arret de l'activite specifie'e en parametre
 *
 *  Retourne : OK ou ERROR
 */
 
int proximityAbortRqstSend (CLIENT_ID clientId, 
			   int *pRqstId,
			   int *activity,
			   int replyTimeOut)
{
  errnoSet(0);

  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_ABORT_RQST, (void *) activity,
			sizeof(int), (FUNCPTR) NULL, FALSE, 0, replyTimeOut, 
			pRqstId) == ERROR)
    return(ERROR);

  return(OK);
}

/*-------------------------------------------------------------------------
 *
 *  proximityAbortReplyRcv  -  Reception de la replique finale
 *
 *  Description:
 *
 *  Retourne : ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK ou
 *             WAITING_FINAL_REPLY
 */
 
int proximityAbortReplyRcv (CLIENT_ID clientId, 
			   int rqstId, 
			   int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY */
			   int *bilan)
{
  int status;    /* ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK */

  /* Verifier le flag de type de blocage */
  if (block != NO_BLOCK && block != BLOCK_ON_FINAL_REPLY) {
    errnoSet (S_proximity_stdGenoM_BAD_BLOCK_TYPE);
    return (ERROR);
  }
  
  status = csClientReplyRcv (clientId, rqstId, block, (void *) NULL, 0,  
			     (FUNCPTR) NULL, (void *) NULL,
			     0, (FUNCPTR) NULL);
  if (status == ERROR) {
    *bilan = errnoGet();

    /* Filtrage type d'erreur */
    if (H2_MODULE_ERR_FLAG(*bilan))
      return(FINAL_REPLY_OK);
  }
  
  return(status);
}
 
/*-------------------------------------------------------------------------
 *
 *  proximityAbortRqstAndRcv  -  Requete de controle
 *
 *  Description: Arret de l'activite specifie'e en parametre
 *  Cette fonction emet une requete et attend la replique finale.
 *
 *  Retourne : ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK
 */
 
int proximityAbortRqstAndRcv (CLIENT_ID clientId, 
			     int *activity,
			     int *bilan)

{
  int rqstId;                  /* Ou` mettre l'id de la requete */
  int status;
  
  errnoSet(0);
  *bilan = OK;
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_ABORT_RQST, (void *) activity,
			sizeof(int), (FUNCPTR) NULL, FALSE, 0, 
			TIME_WAIT_REPLY, 
			&rqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique */
  status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_FINAL_REPLY, 
			     (void *) NULL, 0, (FUNCPTR) NULL, 
			     (void *) NULL, 0, 
			     (FUNCPTR) NULL);
  if (status == ERROR) {
    *bilan = errnoGet();
    
    /* Filtrage type d'erreur */
    if (H2_MODULE_ERR_FLAG(*bilan))
      return(FINAL_REPLY_OK);
  }
  
  return(status);
}


/* liste des requetes/repliques de controle : msgCntrl.c */

/* liste des requetes/repliques d'execution : msgExec.c */





/*========================================================================
 *
 *  proximitycheckLeftSensorsRqstSend  -  Emission d'une requete d'execution
 *
 *  Retourne : OK ou ERROR
 */
 
STATUS proximitycheckLeftSensorsRqstSend (CLIENT_ID clientId, int *pRqstId,
				
				int replyTimeOut)
{
  errnoSet(0);

  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKLEFTSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, TIME_WAIT_REPLY, 
			replyTimeOut, pRqstId) == ERROR)
    return ERROR;
  return OK;
}

/*-------------------------------------------------------------------------
 *  proximitycheckLeftSensorsReplyRcv  -  Reception des repliques
 *
 *  Retourne : ERROR ou FINAL_REPLY_OK ou 
 *             WAITING_INTERMED_REPLY ou INTERMED_REPLY_TIMEOUT ou
 *             WAITING_FINAL_REPLY    ou FINAL_REPLY_TIMEOUT
 */
 
int proximitycheckLeftSensorsReplyRcv (CLIENT_ID clientId, int rqstId, 
			       int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY 
					    BLOCK_ON_INTERMED_REPLY */
			       
			       int *activity, int *bilan)
{
  int status;    /* ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK */

  /* Verifier le flag de type de blocage */
  if (block != NO_BLOCK && block != BLOCK_ON_FINAL_REPLY 
      && block != BLOCK_ON_INTERMED_REPLY) {
    errnoSet (S_proximity_stdGenoM_BAD_BLOCK_TYPE);
    return (ERROR);
  }
  
  if ((status = csClientReplyRcv (clientId, rqstId, block, 
				  (void *) activity, sizeof(int), 
				  (FUNCPTR) NULL, (void *) NULL,
				  0, (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
  
  return(status);
}
 
/*-------------------------------------------------------------------------
 *  proximitycheckLeftSensorsRqstAndAck  -  Emet la requete d'exec, 
 *                                  attend la replique intermediaire
 *
 *  Retourne : ERROR ou INTERMED_REPLY_TIMEOUT ou WAITING_FINAL_REPLY ou 
 *             FINAL_REPLY_OK 
 */
 
int proximitycheckLeftSensorsRqstAndAck (CLIENT_ID clientId, int *pRqstId,
				 int replyTimeOut,
				  
				 
				 int *activity, int *bilan)
{
  int status;
  
  errnoSet(0);
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKLEFTSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, 
			TIME_WAIT_REPLY, replyTimeOut, 
			pRqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique intermediaire */
  if ((status = csClientReplyRcv (clientId, *pRqstId, BLOCK_ON_INTERMED_REPLY, 
                             (void *) activity, sizeof(int), (FUNCPTR) NULL, 
                             (void *) NULL, 0, 
                             (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
 
  return(status);
}

/*-------------------------------------------------------------------------
 *  proximitycheckLeftSensorsRqstAndRcv  -  Emet la requete d'execution,
 *                                  Attend les repliques.
 *
 *  Retourne : ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK ou 
 *             INTERMED_REPLY_TIMEOUT
 */
 
int proximitycheckLeftSensorsRqstAndRcv (CLIENT_ID clientId, 
				 int replyTimeOut,
				  
				 
				 int *activity,
				 int *bilan)
{
  int rqstId;                  /* Ou` mettre l'id de la requete */
  int status;
  
  errnoSet(0);
  *bilan = OK;
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKLEFTSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, 
			TIME_WAIT_REPLY, replyTimeOut, 
			&rqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique intermediaire */
  status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_INTERMED_REPLY, 
			     (void *) activity, sizeof(int), (FUNCPTR) NULL, 
			     (void *) NULL, 0, 
			     (FUNCPTR) NULL);
  switch(status) {

  case ERROR:
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
    break;

  case WAITING_FINAL_REPLY:
    if ((status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_FINAL_REPLY, 
			       (void *) NULL, 0, (FUNCPTR) NULL, 
			       (void *) NULL, 0, 
			       (FUNCPTR) NULL)) == ERROR) {
      *bilan = errnoGet();
      if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
    }
  }
  return(status);
}
 


/*========================================================================
 *
 *  proximitycheckRightSensorsRqstSend  -  Emission d'une requete d'execution
 *
 *  Retourne : OK ou ERROR
 */
 
STATUS proximitycheckRightSensorsRqstSend (CLIENT_ID clientId, int *pRqstId,
				
				int replyTimeOut)
{
  errnoSet(0);

  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKRIGHTSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, TIME_WAIT_REPLY, 
			replyTimeOut, pRqstId) == ERROR)
    return ERROR;
  return OK;
}

/*-------------------------------------------------------------------------
 *  proximitycheckRightSensorsReplyRcv  -  Reception des repliques
 *
 *  Retourne : ERROR ou FINAL_REPLY_OK ou 
 *             WAITING_INTERMED_REPLY ou INTERMED_REPLY_TIMEOUT ou
 *             WAITING_FINAL_REPLY    ou FINAL_REPLY_TIMEOUT
 */
 
int proximitycheckRightSensorsReplyRcv (CLIENT_ID clientId, int rqstId, 
			       int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY 
					    BLOCK_ON_INTERMED_REPLY */
			       
			       int *activity, int *bilan)
{
  int status;    /* ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK */

  /* Verifier le flag de type de blocage */
  if (block != NO_BLOCK && block != BLOCK_ON_FINAL_REPLY 
      && block != BLOCK_ON_INTERMED_REPLY) {
    errnoSet (S_proximity_stdGenoM_BAD_BLOCK_TYPE);
    return (ERROR);
  }
  
  if ((status = csClientReplyRcv (clientId, rqstId, block, 
				  (void *) activity, sizeof(int), 
				  (FUNCPTR) NULL, (void *) NULL,
				  0, (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
  
  return(status);
}
 
/*-------------------------------------------------------------------------
 *  proximitycheckRightSensorsRqstAndAck  -  Emet la requete d'exec, 
 *                                  attend la replique intermediaire
 *
 *  Retourne : ERROR ou INTERMED_REPLY_TIMEOUT ou WAITING_FINAL_REPLY ou 
 *             FINAL_REPLY_OK 
 */
 
int proximitycheckRightSensorsRqstAndAck (CLIENT_ID clientId, int *pRqstId,
				 int replyTimeOut,
				  
				 
				 int *activity, int *bilan)
{
  int status;
  
  errnoSet(0);
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKRIGHTSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, 
			TIME_WAIT_REPLY, replyTimeOut, 
			pRqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique intermediaire */
  if ((status = csClientReplyRcv (clientId, *pRqstId, BLOCK_ON_INTERMED_REPLY, 
                             (void *) activity, sizeof(int), (FUNCPTR) NULL, 
                             (void *) NULL, 0, 
                             (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
 
  return(status);
}

/*-------------------------------------------------------------------------
 *  proximitycheckRightSensorsRqstAndRcv  -  Emet la requete d'execution,
 *                                  Attend les repliques.
 *
 *  Retourne : ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK ou 
 *             INTERMED_REPLY_TIMEOUT
 */
 
int proximitycheckRightSensorsRqstAndRcv (CLIENT_ID clientId, 
				 int replyTimeOut,
				  
				 
				 int *activity,
				 int *bilan)
{
  int rqstId;                  /* Ou` mettre l'id de la requete */
  int status;
  
  errnoSet(0);
  *bilan = OK;
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKRIGHTSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, 
			TIME_WAIT_REPLY, replyTimeOut, 
			&rqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique intermediaire */
  status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_INTERMED_REPLY, 
			     (void *) activity, sizeof(int), (FUNCPTR) NULL, 
			     (void *) NULL, 0, 
			     (FUNCPTR) NULL);
  switch(status) {

  case ERROR:
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
    break;

  case WAITING_FINAL_REPLY:
    if ((status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_FINAL_REPLY, 
			       (void *) NULL, 0, (FUNCPTR) NULL, 
			       (void *) NULL, 0, 
			       (FUNCPTR) NULL)) == ERROR) {
      *bilan = errnoGet();
      if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
    }
  }
  return(status);
}
 


/*========================================================================
 *
 *  proximitycheckTopSensorsRqstSend  -  Emission d'une requete d'execution
 *
 *  Retourne : OK ou ERROR
 */
 
STATUS proximitycheckTopSensorsRqstSend (CLIENT_ID clientId, int *pRqstId,
				
				int replyTimeOut)
{
  errnoSet(0);

  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKTOPSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, TIME_WAIT_REPLY, 
			replyTimeOut, pRqstId) == ERROR)
    return ERROR;
  return OK;
}

/*-------------------------------------------------------------------------
 *  proximitycheckTopSensorsReplyRcv  -  Reception des repliques
 *
 *  Retourne : ERROR ou FINAL_REPLY_OK ou 
 *             WAITING_INTERMED_REPLY ou INTERMED_REPLY_TIMEOUT ou
 *             WAITING_FINAL_REPLY    ou FINAL_REPLY_TIMEOUT
 */
 
int proximitycheckTopSensorsReplyRcv (CLIENT_ID clientId, int rqstId, 
			       int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY 
					    BLOCK_ON_INTERMED_REPLY */
			       
			       int *activity, int *bilan)
{
  int status;    /* ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK */

  /* Verifier le flag de type de blocage */
  if (block != NO_BLOCK && block != BLOCK_ON_FINAL_REPLY 
      && block != BLOCK_ON_INTERMED_REPLY) {
    errnoSet (S_proximity_stdGenoM_BAD_BLOCK_TYPE);
    return (ERROR);
  }
  
  if ((status = csClientReplyRcv (clientId, rqstId, block, 
				  (void *) activity, sizeof(int), 
				  (FUNCPTR) NULL, (void *) NULL,
				  0, (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
  
  return(status);
}
 
/*-------------------------------------------------------------------------
 *  proximitycheckTopSensorsRqstAndAck  -  Emet la requete d'exec, 
 *                                  attend la replique intermediaire
 *
 *  Retourne : ERROR ou INTERMED_REPLY_TIMEOUT ou WAITING_FINAL_REPLY ou 
 *             FINAL_REPLY_OK 
 */
 
int proximitycheckTopSensorsRqstAndAck (CLIENT_ID clientId, int *pRqstId,
				 int replyTimeOut,
				  
				 
				 int *activity, int *bilan)
{
  int status;
  
  errnoSet(0);
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKTOPSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, 
			TIME_WAIT_REPLY, replyTimeOut, 
			pRqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique intermediaire */
  if ((status = csClientReplyRcv (clientId, *pRqstId, BLOCK_ON_INTERMED_REPLY, 
                             (void *) activity, sizeof(int), (FUNCPTR) NULL, 
                             (void *) NULL, 0, 
                             (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
 
  return(status);
}

/*-------------------------------------------------------------------------
 *  proximitycheckTopSensorsRqstAndRcv  -  Emet la requete d'execution,
 *                                  Attend les repliques.
 *
 *  Retourne : ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK ou 
 *             INTERMED_REPLY_TIMEOUT
 */
 
int proximitycheckTopSensorsRqstAndRcv (CLIENT_ID clientId, 
				 int replyTimeOut,
				  
				 
				 int *activity,
				 int *bilan)
{
  int rqstId;                  /* Ou` mettre l'id de la requete */
  int status;
  
  errnoSet(0);
  *bilan = OK;
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKTOPSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, 
			TIME_WAIT_REPLY, replyTimeOut, 
			&rqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique intermediaire */
  status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_INTERMED_REPLY, 
			     (void *) activity, sizeof(int), (FUNCPTR) NULL, 
			     (void *) NULL, 0, 
			     (FUNCPTR) NULL);
  switch(status) {

  case ERROR:
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
    break;

  case WAITING_FINAL_REPLY:
    if ((status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_FINAL_REPLY, 
			       (void *) NULL, 0, (FUNCPTR) NULL, 
			       (void *) NULL, 0, 
			       (FUNCPTR) NULL)) == ERROR) {
      *bilan = errnoGet();
      if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
    }
  }
  return(status);
}
 


/*========================================================================
 *
 *  proximitycheckBackSensorsRqstSend  -  Emission d'une requete d'execution
 *
 *  Retourne : OK ou ERROR
 */
 
STATUS proximitycheckBackSensorsRqstSend (CLIENT_ID clientId, int *pRqstId,
				
				int replyTimeOut)
{
  errnoSet(0);

  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKBACKSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, TIME_WAIT_REPLY, 
			replyTimeOut, pRqstId) == ERROR)
    return ERROR;
  return OK;
}

/*-------------------------------------------------------------------------
 *  proximitycheckBackSensorsReplyRcv  -  Reception des repliques
 *
 *  Retourne : ERROR ou FINAL_REPLY_OK ou 
 *             WAITING_INTERMED_REPLY ou INTERMED_REPLY_TIMEOUT ou
 *             WAITING_FINAL_REPLY    ou FINAL_REPLY_TIMEOUT
 */
 
int proximitycheckBackSensorsReplyRcv (CLIENT_ID clientId, int rqstId, 
			       int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY 
					    BLOCK_ON_INTERMED_REPLY */
			       
			       int *activity, int *bilan)
{
  int status;    /* ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK */

  /* Verifier le flag de type de blocage */
  if (block != NO_BLOCK && block != BLOCK_ON_FINAL_REPLY 
      && block != BLOCK_ON_INTERMED_REPLY) {
    errnoSet (S_proximity_stdGenoM_BAD_BLOCK_TYPE);
    return (ERROR);
  }
  
  if ((status = csClientReplyRcv (clientId, rqstId, block, 
				  (void *) activity, sizeof(int), 
				  (FUNCPTR) NULL, (void *) NULL,
				  0, (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
  
  return(status);
}
 
/*-------------------------------------------------------------------------
 *  proximitycheckBackSensorsRqstAndAck  -  Emet la requete d'exec, 
 *                                  attend la replique intermediaire
 *
 *  Retourne : ERROR ou INTERMED_REPLY_TIMEOUT ou WAITING_FINAL_REPLY ou 
 *             FINAL_REPLY_OK 
 */
 
int proximitycheckBackSensorsRqstAndAck (CLIENT_ID clientId, int *pRqstId,
				 int replyTimeOut,
				  
				 
				 int *activity, int *bilan)
{
  int status;
  
  errnoSet(0);
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKBACKSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, 
			TIME_WAIT_REPLY, replyTimeOut, 
			pRqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique intermediaire */
  if ((status = csClientReplyRcv (clientId, *pRqstId, BLOCK_ON_INTERMED_REPLY, 
                             (void *) activity, sizeof(int), (FUNCPTR) NULL, 
                             (void *) NULL, 0, 
                             (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
 
  return(status);
}

/*-------------------------------------------------------------------------
 *  proximitycheckBackSensorsRqstAndRcv  -  Emet la requete d'execution,
 *                                  Attend les repliques.
 *
 *  Retourne : ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK ou 
 *             INTERMED_REPLY_TIMEOUT
 */
 
int proximitycheckBackSensorsRqstAndRcv (CLIENT_ID clientId, 
				 int replyTimeOut,
				  
				 
				 int *activity,
				 int *bilan)
{
  int rqstId;                  /* Ou` mettre l'id de la requete */
  int status;
  
  errnoSet(0);
  *bilan = OK;
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, PROXIMITY_CHECKBACKSENSORS_RQST, (void *) NULL,
			0, (FUNCPTR) NULL, TRUE, 
			TIME_WAIT_REPLY, replyTimeOut, 
			&rqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique intermediaire */
  status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_INTERMED_REPLY, 
			     (void *) activity, sizeof(int), (FUNCPTR) NULL, 
			     (void *) NULL, 0, 
			     (FUNCPTR) NULL);
  switch(status) {

  case ERROR:
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
    break;

  case WAITING_FINAL_REPLY:
    if ((status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_FINAL_REPLY, 
			       (void *) NULL, 0, (FUNCPTR) NULL, 
			       (void *) NULL, 0, 
			       (FUNCPTR) NULL)) == ERROR) {
      *bilan = errnoGet();
      if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
    }
  }
  return(status);
}
 
