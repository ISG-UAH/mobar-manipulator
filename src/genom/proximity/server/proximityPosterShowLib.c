
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

/*--------------  file automaticaly generated with GenoM -------------*/
/*-----------------------  DO NOT EDIT BY HAND -----------------------*/
 
/****************************************************************************/
/*   LABORATOIRE D'AUTOMATIQUE ET D'ANALYSE DE SYSTEMES - LAAS / CNRS       */
/*   PROJET HILARE II - INTERACTION PAR POSTER AVEC LE MODULE  PROXIMITY     */
/*   FICHIER SOURCE : "proximityPosterLib.c"                                 */
/****************************************************************************/

/* DESCRIPTION :
   Bibliotheque de fonctions qui permettent la lecture ou l'affichage
   du contenu des posters du module
*/


#include <stdio.h>
#include <stdlib.h>
#include <portLib.h>

#include "proximityPosterShowLib.h"
#include "proximityPrint.h"


  
static char *proximityExecTaskNameTab[] = {
"proximityTask"
};

/*  prototypes de la bibliotheque printState */
extern char const * h2GetEvnStateString(int num);

static void proximityActivityShow (PROXIMITY_CNTRL_STR *sdic);


/* ---------------- LE POSTER DE CONTROLE ------------------------------ */

/* ----------------------------------------------------------------------
 *
 *  proximityCntrlPosterShow - Affichage du poster de controle du module
 *
 */

STATUS proximityCntrlPosterShow ()
{
  char strmsg[64];
  PROXIMITY_CNTRL_STR *sdic;
  int i;

  /* Lecture de la SDI de controle */
  sdic = (PROXIMITY_CNTRL_STR *)malloc(sizeof(PROXIMITY_CNTRL_STR));
  if (sdic == NULL) {
    h2perror ("proximityCntrlPosterShow");
    return ERROR;
  }
  if (proximityCntrlPosterRead (sdic) != OK) {
    free(sdic);
    return ERROR;
  }

  /* 
   * Affichage 
   */

  printf ("-- TASKS           (status)      (last duration)         (bilan)\n");
  /* Tache de controle */
  printf (" Control Task        %-5s                               %s\n",  
	  M_CNTRL_TASK_STATUS(sdic)==OK ? "OK":"ERROR",
	  h2getErrMsg(M_CNTRL_TASK_BILAN(sdic), strmsg, 64));
  
  /* Taches d'execution */
  for (i=0; i<PROXIMITY_NB_EXEC_TASK; i++) {
    printf (" %d:%-15s   %-5s ",  
	    i, proximityExecTaskNameTab[i],
	    M_EXEC_TASK_STATUS(sdic,i) == OK ? "OK": "ERROR");
    if (M_EXEC_TASK_PERIOD(sdic,i) != 0) {
      printf ("    %.2lfms ", M_EXEC_TASK_ON_PERIOD(sdic,i)/1000.0);
      printf ("(max %.2lf, ", M_EXEC_TASK_MAX_PERIOD(sdic,i)/1000.0);
      printf ("th %.2lf)", M_EXEC_TASK_PERIOD(sdic,i)*1000.);
    }
    else {
      printf ("    %.2lfms ", M_EXEC_TASK_ON_PERIOD(sdic,i)/1000.0);
      printf ("(max %.2lf)        ", M_EXEC_TASK_MAX_PERIOD(sdic,i)/1000.0);      
    }
    printf ("\t %s\n", h2getErrMsg(M_EXEC_TASK_BILAN(sdic,i), strmsg, 64));

  }
  printf ("\n");

  /* Activites */
  proximityActivityShow (sdic);

  free(sdic);
  return OK;
}

STATUS proximityCntrlPosterActivityShow ()
{
  PROXIMITY_CNTRL_STR *sdic;

  /* Lecture de la SDI de controle */
  sdic = (PROXIMITY_CNTRL_STR *)malloc(sizeof(PROXIMITY_CNTRL_STR));
  if (sdic == NULL) {
    h2perror ("proximityCntrlPosterActivityShow");
    return ERROR;
  }
  if (proximityCntrlPosterRead (sdic) != OK) {
    free(sdic);
    return ERROR;
  }

  proximityActivityShow (sdic);

  free(sdic);
  return OK;
}

/* Fonciton locale d'affichage des activitées */
static void proximityActivityShow (PROXIMITY_CNTRL_STR *sdic)
{
#define PROXIMITY_NB_RQST_EXEC 4
#if PROXIMITY_NB_RQST_EXEC != 0

  
static char *proximityExecRqstNameTab[] = {
"checkLeftSensors",
"checkRightSensors",
"checkTopSensors",
"checkBackSensors"
};
static int proximityExecRqstNumTab[] = {
0, 1, 2, 3};

  int i,oneActivity=FALSE;
  ACTIVITY_EVENT evn;
  ACTIVITY_STATE status;
  int bilan;
  int rqst;
  char strmsg[64];
  
  /*  printf (" ACTIVITY  REQUEST         TASK        FROM -> TO    BILAN\n");*/
  printf ("-- ACTIVITIES\n");
  for (i=0; i<MAX_ACTIVITIES; i++) {
    status = M_ACTIVITY_STATUS(sdic,i);
    bilan = M_ACTIVITY_BILAN(sdic,i);

    /* Affichage */
    if (status != ETHER || bilan != OK) {
      evn = M_ACTIVITY_EVN(sdic,i);
      
      /* Identification de la requete */
      rqst=0;
      while(proximityExecRqstNumTab[rqst] != M_ACTIVITY_RQST_TYPE(sdic,i) 
	    && rqst<PROXIMITY_NB_RQST_EXEC)
	rqst++;

      if (status != ETHER) 
	oneActivity = TRUE;

      printf ("%s%2d  %2d:%-10s    %-10s\t%5s %s%s\n", 
	      status == ETHER ? " (" : "  ",

	      M_ACTIVITY_ID(sdic,i), 

	      rqst == PROXIMITY_NB_RQST_EXEC ? 
	      M_ACTIVITY_RQST_TYPE(sdic,i) : proximityExecRqstNumTab[rqst],

	      rqst == PROXIMITY_NB_RQST_EXEC?
	      "Unknown" : proximityExecRqstNameTab[rqst], 
	      
	      M_ACTIVITY_TASK_NUM(sdic,i) == -1 ? 
	      "not exec" : proximityExecTaskNameTab[M_ACTIVITY_TASK_NUM(sdic,i)], 
	      
	      evn == NO_EVENT ?
	      h2GetEvnStateString(status) : h2GetEvnStateString(evn), 

	      h2getErrMsg(bilan, strmsg, 64),
	      
	      status == ETHER ? ")" : "");
    }

  }	/* for */
  if (!oneActivity) printf ("  No activity\n");
#endif /* PROXIMITY_NB_RQST_EXEC != 0 */
}

/* ---------------- LES POSTERS FONCTIONNELS ------------------------------ */

/* --  Status ------------------------------------------------- */

STATUS proximityStatusPosterShow(void)
{
  PROXIMITY_STATUS_POSTER_STR *x;

  if ((x = (PROXIMITY_STATUS_POSTER_STR *)malloc(sizeof(* x))) == NULL) {
     h2perror("proximityStatusPosterShow");
     return ERROR;
  }
  if (proximityStatusPosterRead(x) == ERROR) {
     h2perror("proximityStatusPosterShow");
     free(x);
     return ERROR;
  }
  print_struct_PROXIMITY_STATUS_POSTER_STR(stdout, x, 0, 0, NULL, stdin);
  free(x);
  return OK;
}

/* --  Status -> sensors ---------------------------------------- */

STATUS proximityStatussensorsPosterShow(void)
{
  PROXIMITYSENSORS *sensors;
  int *dims = NULL;

  if ((sensors = malloc(sizeof(struct PROXIMITYSENSORS))) == NULL) {
     h2perror("proximityStatussensorsPosterShow");
     return ERROR;
  }
  if (proximityStatussensorsPosterRead(sensors) == ERROR) {
     h2perror("proximityStatussensorsPosterRead");
     free(sensors);
     return ERROR;
  }
  printf ("sensors:\n");
  print_struct_PROXIMITYSENSORS(stdout, sensors, 1, 0, dims, stdin);
  free(sensors);
  return OK;
}


