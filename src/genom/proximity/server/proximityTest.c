
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
 
/****************************************************************************
 *   LABORATOIRE D'AUTOMATIQUE ET D'ANALYSE DE SYSTEMES - LAAS / CNRS       
 *   PROJET HILARE II - ROUTINE D'TEST INTERACTIF       
 *   FICHIER SOURCE: proximityTest.c                                          
 ****************************************************************************/

/* VERSION ACTUELLE / HISTORIQUE DES MODIFICATIONS :
 */

/* DESCRIPTION :
   Cette routine doit etre appelee dans le shell de vxWorks ou UNIX.
   */

/*----------------------------- INCLUSIONS ---------------------------------*/

/* Inclusions generales */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include <portLib.h>
#include <h2initGlob.h>
#include <taskLib.h>

/* Inclusions pour acceder au module */
#include "proximityMsgLib.h"
#include "proximityPosterShowLib.h" 
#include "proximityPrint.h"
#include "proximityScan.h"

/* #include "proximityReports.h" */

#include "genom/genom.h"
#include "genom/testLib.h"


void proximityTest (int testNumber);

/*------------------- PROTOTYPES DES FONCTIONS LOCALES --------------------*/

static void proximityTestInitTask (TEST_STR* testStr);

static BOOL proximityTestcheckLeftSensors(TEST_STR *id, int rq, int ac, BOOL ch);
static BOOL proximityTestcheckRightSensors(TEST_STR *id, int rq, int ac, BOOL ch);
static BOOL proximityTestcheckTopSensors(TEST_STR *id, int rq, int ac, BOOL ch);
static BOOL proximityTestcheckBackSensors(TEST_STR *id, int rq, int ac, BOOL ch);

static  TEST_RQST_DESC_STR proximityTestRqstFuncTab[] = {
  {PROXIMITY_CHECKLEFTSENSORS_RQST, proximityTestcheckLeftSensors, 0, 0},
  {PROXIMITY_CHECKRIGHTSENSORS_RQST, proximityTestcheckRightSensors, 0, 0},
  {PROXIMITY_CHECKTOPSENSORS_RQST, proximityTestcheckTopSensors, 0, 0},
  {PROXIMITY_CHECKBACKSENSORS_RQST, proximityTestcheckBackSensors, 0, 0}
};
  
static char *proximityTestRequestNameTab[] = {
"checkLeftSensors (E)",
"checkRightSensors (E)",
"checkTopSensors (E)",
"checkBackSensors (E)"
};

  
static char *proximityTestPosterNameTab[] = {
"Status"
};
  
static STATUS (*proximityTestPosterShowFuncTab[])() = {
proximityStatusPosterShow,
proximityCntrlPosterShow,
proximityCntrlPosterActivityShow
};


/*--------------------------------------------------------------------------*/

int
main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "usage: proximityTest <numMBox>\n");
    exit(1);
  }
  if (h2initGlob(0) == ERROR) {
      exit(2);
  }
  proximityTest(atoi(argv[1]));
  /*NOTREACHED*/
  return 0;
} /* main */

/**
 **   proximityTest - Fonction de test du module proximity
 **/

void
proximityTest (int testNumber)     
{
  TEST_STR *testStr;
  
  /* Allocation de la structure */
  if ((testStr = testInit(testNumber, "proximity",
			  PROXIMITY_CLIENT_MBOX_REPLY_SIZE,
			  PROXIMITY_ABORT_RQST,
			  4,
			  proximityTestRequestNameTab,
			  proximityTestRqstFuncTab,
			  1, 
			  proximityTestPosterNameTab,
			  proximityTestPosterShowFuncTab)) == NULL)
    return;

  /* Init specifiques */
  proximityTestInitTask (testStr);
  
  /* Fonction principale */
  testMain(testStr);
}

/*----------------------- ROUTINES LOCALES ---------------------------------*/


/**
 **  proximityTestInitTask - Routine d'initialisation de la tache d'essai
 **/

static void proximityTestInitTask (TEST_STR *testStr)
     
{
  /* S'initialiser comme client */
  printf ("client init ...");
  if (csClientInit (PROXIMITY_MBOX_NAME, PROXIMITY_MAX_RQST_SIZE,
		    PROXIMITY_MAX_INTERMED_REPLY_SIZE, 
		    PROXIMITY_MAX_REPLY_SIZE, &TEST_CID(testStr)) != OK) {
      (void) h2perror("Client init failed");
      testEnd(testStr);
  }
  
  /* S'initialiser comme client des posters */
  printf ("ok.   Poster init ...");
  if (proximityPosterInit () != OK)
    h2perror ("Warning: not found all proximity's posters");

  printf ("ok.");
}
 


/* 
 * Copyright (c) 2005 LAAS/CNRS
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


/**
 **  Emission/reception de la requete d'execution proximityTestcheckLeftSensors
 **/

static BOOL proximityTestcheckLeftSensors (TEST_STR *testId, int rqstNum, 
				    int acti, BOOL silent)
{
  
  

  /* Saisie données */
  if (!TEST_ACTIVITY_ON(testId, acti)) {
    
  }
  
  /* Emission d'une requete + replique intermediaire 
     ET/OU reception replique finale (selon TEST_ACTIVITY_RQST_ID) */
  if (!testSendAndOrRcvExecRqst(testId, rqstNum, acti, silent))
    return FALSE;

  
  return TRUE;
}



/* 
 * Copyright (c) 2005 LAAS/CNRS
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


/**
 **  Emission/reception de la requete d'execution proximityTestcheckRightSensors
 **/

static BOOL proximityTestcheckRightSensors (TEST_STR *testId, int rqstNum, 
				    int acti, BOOL silent)
{
  
  

  /* Saisie données */
  if (!TEST_ACTIVITY_ON(testId, acti)) {
    
  }
  
  /* Emission d'une requete + replique intermediaire 
     ET/OU reception replique finale (selon TEST_ACTIVITY_RQST_ID) */
  if (!testSendAndOrRcvExecRqst(testId, rqstNum, acti, silent))
    return FALSE;

  
  return TRUE;
}



/* 
 * Copyright (c) 2005 LAAS/CNRS
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


/**
 **  Emission/reception de la requete d'execution proximityTestcheckTopSensors
 **/

static BOOL proximityTestcheckTopSensors (TEST_STR *testId, int rqstNum, 
				    int acti, BOOL silent)
{
  
  

  /* Saisie données */
  if (!TEST_ACTIVITY_ON(testId, acti)) {
    
  }
  
  /* Emission d'une requete + replique intermediaire 
     ET/OU reception replique finale (selon TEST_ACTIVITY_RQST_ID) */
  if (!testSendAndOrRcvExecRqst(testId, rqstNum, acti, silent))
    return FALSE;

  
  return TRUE;
}



/* 
 * Copyright (c) 2005 LAAS/CNRS
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


/**
 **  Emission/reception de la requete d'execution proximityTestcheckBackSensors
 **/

static BOOL proximityTestcheckBackSensors (TEST_STR *testId, int rqstNum, 
				    int acti, BOOL silent)
{
  
  

  /* Saisie données */
  if (!TEST_ACTIVITY_ON(testId, acti)) {
    
  }
  
  /* Emission d'une requete + replique intermediaire 
     ET/OU reception replique finale (selon TEST_ACTIVITY_RQST_ID) */
  if (!testSendAndOrRcvExecRqst(testId, rqstNum, acti, silent))
    return FALSE;

  
  return TRUE;
}

