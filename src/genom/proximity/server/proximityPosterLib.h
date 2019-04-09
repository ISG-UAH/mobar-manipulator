
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

#ifndef proximity_POSTER_LIB_H
#define proximity_POSTER_LIB_H

#include "posterLib.h"
#include "genom/genom.h"

/* Types definis ou utilises par le module */
#include "proximityType.h"
#include "proximityError.h"

/* 
 * Definition de la SDI_C 
*/
typedef struct {
  CNTRL_TASK_DESCRIPTION cntrlTask;
  EXEC_TASK_DESCRIPTION execTaskTab[PROXIMITY_NB_EXEC_TASK];
  ACTIVITY_DESCRIPTION activityTab[MAX_ACTIVITIES];
} PROXIMITY_CNTRL_STR;

/* For consistency with other posters */
typedef PROXIMITY_CNTRL_STR PROXIMITY_CNTRL_POSTER_STR;
 
/* 
 * Definition des structures de posters
 */
#include "proximityPosterLibStruct.h"



/* Posters */
#define PROXIMITY_CNTRL_POSTER_NAME genomInstanceSuffixName("proximity", "Cntrl")
#define PROXIMITY_STATUS_POSTER_NAME genomInstanceSuffixName("proximity", "Status")


/*---------------- PROTOTYPES DES FONCTIONS EXTERNES ------------------*/

#ifdef __cplusplus
extern "C" {
#endif
extern STATUS proximityPosterInit ( void );
extern POSTER_ID proximityCntrlPosterID ();
extern STATUS proximityCntrlPosterRead ( PROXIMITY_CNTRL_STR *proximityCntrlStrId );
extern STATUS proximityCntrlPosterInit ( void );
extern STATUS proximityCntrlInstancePosterRead(const char *name, PROXIMITY_CNTRL_STR *proximityCntrlStrId);
extern STATUS proximityStatusPosterInit ( void );
extern POSTER_ID proximityStatusPosterID ( void );
extern STATUS proximityStatusPosterRead ( PROXIMITY_STATUS_POSTER_STR *x );
extern STATUS proximityStatusInstancePosterRead ( const char *, PROXIMITY_STATUS_POSTER_STR * );
extern STATUS proximityStatussensorsPosterRead ( PROXIMITYSENSORS *sensors );
extern STATUS proximityStatussensorsInstancePosterRead( const char *, PROXIMITYSENSORS * );
#ifdef __cplusplus
}
#endif
#endif

