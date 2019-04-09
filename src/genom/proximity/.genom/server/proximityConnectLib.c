
/* 
 * Copyright (c) 1993-2003 LAAS/CNRS
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

/***********************************************************************
 *
 *        Fonctions ClientInit et ClientEnd du module proximity
 *
 * Attention: inutile si vous utilisez msgLib: c'est deja inclus dedans
 ***********************************************************************/


#include <portLib.h>
#include <genom/genom.h>

#include "proximityMsgLib.h"
 
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
  proximityRecordH2errMsgs();

  /* Appeler la routine d'initialisation d'un client CS */
  return (csClientInit (PROXIMITY_MBOX_NAME, PROXIMITY_MAX_RQST_SIZE,
                        PROXIMITY_MAX_INTERMED_REPLY_SIZE, 
			PROXIMITY_MAX_REPLY_SIZE, 
			pClientId));
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

STATUS proximityClientEnd (clientId)
     CLIENT_ID clientId;          /* Id. du client */
     
{
  /* Appeler la routine de liberation du client */
  return (csClientEnd (clientId));
}


