
/* --- FILE GENERATED WITH GENOM, DO NOT EDIT BY HAND ------------------ */

/* 
 * Copyright (c) 2004-2005
 *      Autonomous Systems Lab, Swiss Federal Institute of Technology.
 * Copyright (c) 1993-2005,2014 LAAS/CNRS
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

/*
 * Execution task body
 */

#if defined(__RTAI__) && defined(__KERNEL__)
# include <linux/time.h>
# define gettimeofday(x,y)	do_gettimeofday(x)
#else
# include <sys/time.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#endif /* not RTAI && KERNEL */

#include <taskLib.h>
#include <errnoLib.h>

#include <commonStructLib.h>
#include <h2evnLib.h>
#include <h2timeLib.h>
#include <h2timerLib.h>

#include <genom/genom.h>
#include <genom/modules.h>
#include <genom/moduleEvents.h>

#include "proximityHeader.h"


/* Print debugging information */
#define GENOM_DEBUG_EXECTASK

#ifdef GENOM_DEBUG_EXECTASK
# define LOGDBG(x)	logMsg x
#else
# define LOGDBG(x)
#endif

/* Size of the mailbox receiving replies from servers */
#define PROXIMITY_PROXIMITYTASK_MBOX_REPLY_SIZE 


/*--------------------------- EXTERNAL VARIABLES -------------------------*/
/* WARNING: use unique names! */

/* Initialization semaphore */
extern SEM_ID semproximityInitExecTab[];

/*---------------- PROTOTYPES OF INTERNAL FUNCTIONS ----------------------*/

/* Execution task initialization */
static STATUS proximityproximityTaskInitTaskFunc (H2TIMER_ID *execTimerId);

/* Suspend the the execution task */
static void proximityproximityTaskSuspend (BOOL giveFlag);

/* Call of user functions */
static ACTIVITY_EVENT execTaskCallUserFunc (ACTIVITY_STATE state, 
					    int activityId);

/* Signal a transition after handling it */
static BOOL filterAndSendEvn (ACTIVITY_STATE state,
			     ACTIVITY_EVENT evn);

/* Returns a string representing the h2 event */
char const * h2GetEvnStateString(int num);

/* Compute micro-seconds difference between 2 H2TIMESPEC */
static long h2timespec_udiff(const H2TIMESPEC *, const H2TIMESPEC *);

/*--------------------------- LOCAL VARIABLES -------------------------*/
/* WARNING: use unique names */

/*---------------- User function prototypes -------------------*/

/* Permanent activity functions */
#if (0)
STATUS dummy (int *bilan);
#endif
#if (0)
STATUS dummy (int *bilan);
#endif

/* Initialisation function */
#if (1)
int proximityInit ();
#endif

/*---------------------- EXPORTED PROCEDURES --------------------------------*/

/*****************************************************************************
 *
 *  proximityproximityTask  -  Exec task
 *
 *  Description : Execute fonctions in the module
 *
 *
 *  Returns: never
 */

void *
proximityproximityTask (void *arg)
{
  static H2TIMER_ID proximityproximityTaskTimerId;
  int i, nb, nbActi;
  int prevExecTaskBilan;
  int wakeUpCntrlTask;
#if (0) /* client task */
  int extEvn;
#endif
  int periodOverShot=0;

  MODULE_EVENT_STR moduleEvent;
  H2TIMESPEC tpBegin, tpBeginPrev, tpEnd; /* time measure */

  unsigned long  meanDuration, nbIter=1;
  int firstChrono=TRUE;
  int computeMeanFlag = FALSE; /* try to compute meanDuration only if main exec loop and not abnormal overshot */

  /* Initialization of task */
  errnoSet(0);
  EXEC_TASK_STATUS(0) = 
    proximityproximityTaskInitTaskFunc (&proximityproximityTaskTimerId);
  prevExecTaskBilan = EXEC_TASK_BILAN(0) = errnoGet();
  
  /* Release the initialization semaphore */
  semGive (semproximityInitExecTab[0]);
  
  /* suspend ourselves in case of problems */
  if(EXEC_TASK_STATUS(0) != OK)
    proximityproximityTaskSuspend (FALSE);
  moduleEvent.moduleNum = 2005;
  moduleEvent.taskNum = 0;

  tpBeginPrev.tv_sec = 0;
  tpBeginPrev.tv_nsec = 0;

  /* main loop */
  FOREVER {
#if (1) /* Periodic task */
    if (h2timerPause (proximityproximityTaskTimerId) != OK) {
      logMsg("proximityproximityTask: h2timerPause error\n");
      proximityproximityTaskSuspend (FALSE);
    }
#elif (0)
    if (dummy() != OK) {
      logMsg("proximityproximityTask: dummy error\n");
      proximityproximityTaskSuspend(FALSE);
    }
#else /* wait for external events */
    if (h2evnSusp(0) != TRUE) {
      printf ("proximityproximityTask: h2evnSusp error\n");
      proximityproximityTaskSuspend (FALSE);
    }
#endif

    /* Get time */
    moduleEvent.eventType = EXEC_START_EVENT;
    sendModuleEvent(&moduleEvent);

    h2GetTimeSpec(&tpBegin);

    if (firstChrono) {
	    firstChrono=FALSE; 
	    memcpy(&tpBeginPrev, &tpBegin, sizeof(H2TIMESPEC));
    }

    computeMeanFlag = 1;
#if (1) /* periodic task */
    EXEC_TASK_ON_PERIOD(0) = 
	h2timespec_udiff(&tpBegin, &tpBeginPrev);
    if (EXEC_TASK_ON_PERIOD(0) 
	> EXEC_TASK_MAX_PERIOD(0)) {
      EXEC_TASK_MAX_PERIOD(0) = EXEC_TASK_ON_PERIOD(0);
      computeMeanFlag = 0;
    }
    memcpy(&tpBeginPrev, &tpBegin, sizeof(H2TIMESPEC));

    /* Previous computation overshot */
    if (EXEC_TASK_DURATION_LOOP(0) > (1000000.0*EXEC_TASK_PERIOD(0))) {
      periodOverShot = (int) ((EXEC_TASK_DURATION_LOOP(0)
			       /(1000000.0*EXEC_TASK_PERIOD(0))));
      
      if (GENOM_PRINT_TIME_FLAG) 
	printf("proximityproximityTask overshot of %d periods (d=%ldus mean=%ldus p=%ldus max=%ldus th=%dus)\n",
	       periodOverShot,
	       EXEC_TASK_DURATION_LOOP(0),
	       meanDuration,
	       EXEC_TASK_ON_PERIOD(0),
	       EXEC_TASK_MAX_PERIOD(0),
	       (int)(1000000*EXEC_TASK_PERIOD(0)));
    }
    else
      periodOverShot = 0;
#endif

    wakeUpCntrlTask = FALSE;

    /* Interruption requested */
    if (STOP_MODULE_FLAG) {
      
#if (1)
      /* Execute end codel */
      proximityEnd ();
#endif
      
      /* free posters, clients, mailboxes */
        if (EXEC_TASK_POSTER_ID(0)[0] != (POSTER_ID)NULL)
    posterDelete(EXEC_TASK_POSTER_ID(0)[0]);

#if (0) /* client task */
      
      csMboxEnd();
#else
#if (!1) /* non-periodic task */
      /* free device created to manage h2evn required to aperiodic tasks */
      mboxEnd(0);
#endif
#endif
      /* free semaphore */
      EXEC_TASK_WAKE_UP_FLAG(0) = FALSE;
      h2evnSignal(CNTRL_TASK_ID);

      return NULL;
    }

    /* take IDS access */
      if (commonStructTake (proximityCntrlStrId) != OK ||
	  commonStructTake (proximityDataStrId) != OK) {
	logMsg("proximityproximityTask: commonStructTake error\n");
	proximityproximityTaskSuspend (FALSE);
      }
    
    /* Check for a received reply (XXX: see remark down) */
#if (0) /* client task */
    extEvn = FALSE;
    if (csMboxStatus(REPLY_MBOX) & REPLY_MBOX)
      extEvn = TRUE;
#endif


    /* If last period overshot, does not execute activities this time */
    /* XXXXXX NOT YET: AFTER COGNIRON, SEEMS TO HAVE SIDES EFFECTS
       if (!periodOverShot) { */
    
    /* permanent activity 1 */
#if (0) 
    moduleEvent.eventType = STATE_START_EVENT;
    moduleEvent.activityNum = -1;
    moduleEvent.activityState = EXEC;
    sendModuleEvent(&moduleEvent);    

    CURRENT_ACTIVITY_NUM(0) = -1;
    EXEC_TASK_BILAN(0) = OK;
    if (dummy (&EXEC_TASK_BILAN(0)) != OK) {
      logMsg("proximityproximityTask: permanent activity error\n");
      proximityproximityTaskSuspend (TRUE);
    }

    moduleEvent.eventType = STATE_END_EVENT;
    sendModuleEvent(&moduleEvent);   
#endif

    /* Look for activities */
    nbActi = EXEC_TASK_NB_ACTI(0);
    for (i = 0, nb = 0; nb < nbActi && i < MAX_ACTIVITIES; i++) 
      if (ACTIVITY_TASK_NUM(i) == 0) {
	
	nb++;

	/* Wake up sleeping activities */
	/* XXX pb: on peut louper l'info extEvn: si la lecture est effectuer par une autre activite ou par la fonction permanente et que la "reply" arrive apres le test extEvn et avant la lecture !!!
1ere solution: eveiller systematiquement les activites SLEEP ?!? 
2nd solution: la fonction qui recoit la replique execute EXEC_TASK_WAKE_UP */
	if (ACTIVITY_STATUS(i) == SLEEP && ACTIVITY_EVN(i) == NO_EVENT)
	  ACTIVITY_EVN(i) = EXEC;

	/* Execute, depending on the event */
	switch (ACTIVITY_EVN(i)) {
	  
	  /* Nothing (ZOMBIE, SLEEP activity) */
	case NO_EVENT:
	  break;
	  
	  /* Handled by the control task */
	case ETHER:
	case ZOMBIE:
	case INIT:
	case SLEEP:
	  break;
	  
	  /* This is for us */
	case START:
	  EXEC_TASK_MAX_PERIOD(0) = 0;
	case END:
	case FAIL:
	case INTER:
	  computeMeanFlag = 0;
	case EXEC:
	  moduleEvent.eventType = STATE_START_EVENT;
	  moduleEvent.activityNum = i;
	  moduleEvent.activityState = ACTIVITY_EVN(i);
	  moduleEvent.rqstType = ACTIVITY_RQST_TYPE(i);
	  sendModuleEvent(&moduleEvent);

	  /* record new state */
	  ACTIVITY_STATUS(i) = (ACTIVITY_STATE) ACTIVITY_EVN(i);
	  
	  /* Call the processing function */
	  CURRENT_ACTIVITY_NUM(0) = i;
	  ACTIVITY_EVN(i) = execTaskCallUserFunc(ACTIVITY_STATUS(i), i);
	  
	  /* Check event validity */
	  if(filterAndSendEvn(ACTIVITY_STATUS(i), ACTIVITY_EVN(i)))
	    wakeUpCntrlTask = TRUE;

	  moduleEvent.eventType = STATE_END_EVENT;
	  sendModuleEvent(&moduleEvent);
	  break;
	  
	default:
	  logMsg("proximityproximityTask, activity %d: incoherent evn %s\n",
		  i, h2GetEvnStateString (ACTIVITY_EVN(i)));
	  proximityproximityTaskSuspend (TRUE);
	}           /* switch evn */ 
      }        /* while there are activities */ 
    
    /*****XXXX Temporary test */
    if (nb != nbActi)
      logMsg("proximityproximityTask: invalid number of activities %d (expected %d) !\n", nb, nbActi);

/*XXXXXXXX     }  *//* period overshot test */

    /* permanent activity 2 */
#if (0) 
    moduleEvent.eventType = STATE_START_EVENT;
    moduleEvent.activityNum = -1;
    moduleEvent.activityState = EXEC;
    sendModuleEvent(&moduleEvent);    

    CURRENT_ACTIVITY_NUM(0) = -1;
    EXEC_TASK_BILAN(0) = OK;
    if (dummy (&EXEC_TASK_BILAN(0)) != OK) {
      logMsg("proximityproximityTask: permanent activity 2 error\n");
      proximityproximityTaskSuspend (TRUE);
    }

    moduleEvent.eventType = STATE_END_EVENT;
    sendModuleEvent(&moduleEvent);   
#endif

    /* no more activity */
    CURRENT_ACTIVITY_NUM(0) = -2;

    /* update "auto" posters */
      {
  PROXIMITY_STATUS_POSTER_STR x;
  int offset, size;

  offset = offsetof(PROXIMITY_STATUS_POSTER_STR, sensors);
  size = sizeof(x.sensors);
  if (posterWrite(PROXIMITY_STATUS_POSTER_ID, offset, (char *)&(proximityDataStrId->sensors), size) != size) proximityproximityTaskSuspend (TRUE);
  }
    
    /* Time elapsed since previous read */
    moduleEvent.eventType = EXEC_END_EVENT;
    sendModuleEvent(&moduleEvent);

    h2GetTimeSpec(&tpEnd);

#if (!1)
    EXEC_TASK_ON_PERIOD(0) = h2timespec_udiff(&tpEnd, &tpBegin);
    if (EXEC_TASK_ON_PERIOD(0) > EXEC_TASK_MAX_PERIOD(0)) {
      EXEC_TASK_MAX_PERIOD(0) = EXEC_TASK_ON_PERIOD(0);
      computeMeanFlag = 0;
    }
#endif

    EXEC_TASK_DURATION_LOOP(0) = 
	h2timespec_udiff(&tpEnd, &tpBegin);
    if (computeMeanFlag) {
      meanDuration = (int)
	(((double)((nbIter-1)*meanDuration + EXEC_TASK_DURATION_LOOP(0))
	  /((double)nbIter)));
      nbIter++;
    }

    /* display about time */
    if (GENOM_PRINT_TIME_FLAG) {
      printf("proximityproximityTask: d %4ld mean %4ld p %4ld max %4ld th %4d\n",
	     EXEC_TASK_DURATION_LOOP(0),
	     meanDuration,
	     EXEC_TASK_ON_PERIOD(0),
	     EXEC_TASK_MAX_PERIOD(0),
	     (int)(EXEC_TASK_PERIOD(0)*1000000.0)); 
    }

    /* The result changed: inform the control task */
    if (prevExecTaskBilan != EXEC_TASK_BILAN(0)) {
      prevExecTaskBilan =  EXEC_TASK_BILAN(0);
      wakeUpCntrlTask = TRUE;
    }

    if(wakeUpCntrlTask)
      h2evnSignal(CNTRL_TASK_ID);

    /* Wake-up others tasks ? */
    for (i=0; i<CNTRL_NB_EXEC_TASKS; i++) {
      if (EXEC_TASK_WAKE_UP_FLAG(i)) {
	h2evnSignal(EXEC_TASK_ID(i));
	EXEC_TASK_WAKE_UP_FLAG(i) = FALSE;
      }
    }

    if (commonStructGive (proximityDataStrId) != OK ||
	commonStructGive (proximityCntrlStrId) != OK) {
      logMsg("proximityproximityTask: commonStructGive error\n");
      proximityproximityTaskSuspend (FALSE);
    }

    /* relinquish CPU to give a chance to other tasks to run in heavily
     * contended situations (small period) */
    taskDelay(0);
  }     /* FOREVER */
  return NULL;
}



/*----------------------  ROUTINES LOCALES ---------------------------------*/



/*****************************************************************************
*
*  proximityproximityTaskInitTaskFunc  -  Routine initialisation
*
*  Description:
*  Cette fonction alloue et initialise tous les objets utilises par la tache
*  d'execution.
*
*  Retourne : OK ou ERROR
*/

static STATUS proximityproximityTaskInitTaskFunc (H2TIMER_ID *execTimerId)

{
#if (1)
  int bilan;
#endif

    /* Enregistrer l'id de la tache */
  EXEC_TASK_ID(0) = taskIdSelf ();

#if (0) /* Client */
  /* Creation de la boite aux lettres de reception des repliques */
  if (csMboxInit ("proximityproximityTask", 0,  
                  PROXIMITY_PROXIMITYTASK_MBOX_REPLY_SIZE) != OK) {
    h2perror("proximityproximityTaskInitTaskFunc: csMboxInit");
    return (ERROR); 
  }

  /* S'initialiser comme client */
  
#endif

#if (1) /* Periodic */

    /*** XXXX
      Passer la periode en milliseconde. Verifier que c'est un
      nombre entier de tic:
      Utiliser sysClkRateGet au lieu de NTICKS_PER_SEC
      Sinon sortir 
      XXX */
  /* Allouer un timer h2 */
  if ((*execTimerId = h2timerAlloc ()) == NULL) {
    h2perror("proximityproximityTaskInitTaskFunc: h2timerAlloc");
    return (ERROR);
  }
  LOGDBG(("proximityproximityTaskInitTaskFunc: timer allocated\n"));

  /* Demarrer le timer d'asservissement */
  if (h2timerStart (*execTimerId, 1, 0) != OK) {
    h2perror("proximityproximityTaskInitTaskFunc: h2timerStart");
    return (ERROR);
  }
  LOGDBG(("proximityproximityTaskInitTaskFunc: timer started\n"));
  
  /* Obtenir la periode d'asservissement */
  EXEC_TASK_PERIOD(0) = 
    ((double) 1 / (double) NTICKS_PER_SEC);
#else
  EXEC_TASK_PERIOD(0) = 0;
#endif /* Periodic */

  EXEC_TASK_MAX_PERIOD(0) = 0;
  EXEC_TASK_ON_PERIOD(0) = 0;
  EXEC_TASK_WAKE_UP_FLAG(0) = FALSE;

  /* Creer le poster */
    if (posterCreate(PROXIMITY_STATUS_POSTER_NAME,
    sizeof(PROXIMITY_STATUS_POSTER_STR),
    &(EXEC_TASK_POSTER_ID(0)[0])) != OK) {
     logMsg("proximityproximityTaskInitTaskFunc: cannot create poster Status\n");
     return(ERROR);
    }
    {
       int size = sizeof(PROXIMITY_STATUS_POSTER_STR);
       char *tmp = malloc(size);
       if (tmp == NULL) {
          fprintf (stderr, "proximityproximityTaskInitTaskFunc : not enough mem to init poster Status\n");
       }
       else {
          memset(tmp, 0, size);
          if (posterWrite(EXEC_TASK_POSTER_ID(0)[0], 0, tmp, size) != size) {
             fprintf (stderr, "proximityproximityTaskInitTaskFunc : cannot init poster Status\n");
             free(tmp);
             return(ERROR);
          }
          free(tmp);
       }
    }

  LOGDBG(("proximityproximityTaskInitTaskFunc: posters created\n"));
      
  /* S'initialiser comme client des Posters */
  
  LOGDBG(("proximityproximityTaskInitTaskFunc: client posters initialized\n"));

  /* Enregister le nom de la tache */
/*  strcpy (EXEC_TASK_NAME(0), "execTaskName"); */

#if (1)
    /* Execution de la fonction d'initialisation */
  if (proximityInit (&bilan) != OK) {
    errnoSet(bilan);
    h2perror("proximityproximityTaskInitTaskFunc: proximityInit");
    return (ERROR);
  }
#endif

  /* Record errors */
/*   proximityRecordH2errMsgs(); */

   /* Donner le sem de fin d'initialisation */
  LOGDBG(("proximityproximityTaskInitTaskFunc: ok\n"));
  return (OK);
}

/*****************************************************************************
*
*  proximityproximityTaskSuspend  -  Suspension de la tache d'asservissement
*
*  Description :
*  Cette fonction signale qu'une erreur a ete retrouvee a l'interieur de 
*  la tache et se suspend par la suite
*
*  Retourne: Neant
*/

static void proximityproximityTaskSuspend (BOOL giveFlag)

{
  char str[64];

  /* Indiquer qu'une erreur a ete detectee */
  EXEC_TASK_STATUS(0) = ERROR;
  if (EXEC_TASK_BILAN(0) == OK)
    EXEC_TASK_BILAN(0) = errnoGet();

  logMsg("Suspend proximityproximityTask: %s\n", 
	  h2getErrMsg(EXEC_TASK_BILAN(0), str, 64));

  /* Eveiller la tache de controle */
  h2evnSignal(CNTRL_TASK_ID);

  /* Liberer les SDIs */
  if (giveFlag) {
    commonStructGive ((void *) proximityDataStrId);
    commonStructGive ((void *) proximityCntrlStrId);    
  }

  /* Suspendre la tache */
  taskSuspend (0);
}

/*****************************************************************************
*
*  execTaskCallUserFunc  -  Appele la fonction utilisateur correspondant au 
*                           numero d'activite et on etat. 
*
*  Description : 
*
*  Retourne: L'etat retourne par la fonction utilisateur 
*/

static ACTIVITY_EVENT execTaskCallUserFunc (ACTIVITY_STATE state, 
					    int activityId)

{
  void *inputId;          /* Adresse input */
  void *outputId;         /* Adresse output */
  int inputFlag;          /* Flag input */
  int outputFlag;         /* Flag output */
  int *bilan;             /* Adresse bilan */
  int activityNum;        /* Numero d'activite */

  /* Tableau des fonction d'execution (user) */
  static ACTIVITY_EVENT (*proximityproximityTaskExecFuncTab[])() = {
  checkLeftSensorsMain,
  checkRightSensorsMain,
  checkTopSensorsMain,
  checkBackSensorsMain,
  NULL};

  /* Tableau des fonctions de demarrage (user)*/
  static ACTIVITY_EVENT (*proximityproximityTaskExecFuncStartTab[])() = {
  NULL,
  NULL,
  NULL,
  NULL,
  NULL};

  /* Tableau des fonctions de terminaison (user) */
  static ACTIVITY_EVENT (*proximityproximityTaskExecFuncEndTab[])() = {
  NULL,
  NULL,
  NULL,
  NULL,
  NULL};

  /* Tableau des fonctions de terminaison en cas d'interruption (user) */
  static ACTIVITY_EVENT (*proximityproximityTaskExecFuncInterTab[])() = {
  NULL,
  NULL,
  NULL,
  NULL,
  NULL};
 
  /* Tableau des fonctions de terminaison en cas d'echec (user) */
  static ACTIVITY_EVENT (*proximityproximityTaskExecFuncFailTab[])() = {
  NULL,
  NULL,
  NULL,
  NULL,
  NULL};
 

  /* Numero de requete */
  activityNum = ACTIVITY_RQST_TYPE(activityId);

  /* Adresse des parametres */
  inputId = ACTIVITY_INPUT_ID(activityId);
  outputId = ACTIVITY_OUTPUT_ID(activityId);
  bilan =  &ACTIVITY_BILAN(activityId);

  /* Flag parametres */
  inputFlag = inputId == NULL ? FALSE : TRUE;
  outputFlag = outputId == NULL ? FALSE : TRUE;

  switch(state) {
    /* Fonction de demarage */
  case START:
    if (proximityproximityTaskExecFuncStartTab [activityNum] == NULL)
      return (EXEC);
    if (inputFlag && outputFlag)
      return((*proximityproximityTaskExecFuncStartTab [activityNum])
	     (inputId, outputId, bilan));
    if (inputFlag)
      return((*proximityproximityTaskExecFuncStartTab [activityNum])
	     (inputId, bilan));
    if (outputFlag)
      return((*proximityproximityTaskExecFuncStartTab [activityNum])
	     (outputId, bilan));
    return((*proximityproximityTaskExecFuncStartTab [activityNum])
	   (bilan));
    
    /* Fonction d'execution */
  case EXEC:
    if (proximityproximityTaskExecFuncTab [activityNum] == NULL)
      return (END);
    if (inputFlag && outputFlag)
      return((*proximityproximityTaskExecFuncTab [activityNum])
	     (inputId, outputId, bilan));
    if (inputFlag)
      return((*proximityproximityTaskExecFuncTab [activityNum])
	     (inputId, bilan));
    if (outputFlag)
      return((*proximityproximityTaskExecFuncTab [activityNum])
	     (outputId, bilan));
    return((*proximityproximityTaskExecFuncTab [activityNum])
	   (bilan));
    
    /* Fonction de terminaison normale, sur pb ou sur interruption */
  case END:
    if (proximityproximityTaskExecFuncEndTab [activityNum] == NULL)
      return(ETHER);
    if (inputFlag && outputFlag)
      return((*proximityproximityTaskExecFuncEndTab [activityNum])
	     (inputId, outputId, bilan));
    if (inputFlag)
      return((*proximityproximityTaskExecFuncEndTab [activityNum])
	     (inputId, bilan));
    if (outputFlag)
      return((*proximityproximityTaskExecFuncEndTab [activityNum])
	     (outputId, bilan));
    return((*proximityproximityTaskExecFuncEndTab [activityNum])
	   (bilan));

    /* Fonction a appeler en cas d'echec */
  case FAIL:
    if (proximityproximityTaskExecFuncFailTab [activityNum] == NULL)
      return(ZOMBIE);
    if (inputFlag && outputFlag)
      return((*proximityproximityTaskExecFuncFailTab [activityNum])
	     (inputId, outputId, bilan));
    if (inputFlag)
      return((*proximityproximityTaskExecFuncFailTab [activityNum])
	     (inputId, bilan));
    if (outputFlag)
      return((*proximityproximityTaskExecFuncFailTab [activityNum])
	     (outputId, bilan));
    return((*proximityproximityTaskExecFuncFailTab [activityNum])
	   (bilan));

    /* Fonction a appeler en cas d'interruption */
  case INTER:
    if (proximityproximityTaskExecFuncInterTab [activityNum] == NULL)
      return(ETHER);
    if (inputFlag && outputFlag)
      return((*proximityproximityTaskExecFuncInterTab [activityNum])
	     (inputId, outputId, bilan));
    if (inputFlag)
      return((*proximityproximityTaskExecFuncInterTab [activityNum])
	     (inputId, bilan));
    if (outputFlag)
      return((*proximityproximityTaskExecFuncInterTab [activityNum])
	     (outputId, bilan));
    return((*proximityproximityTaskExecFuncInterTab [activityNum])
	   (bilan));

    /* Etats impossibles: ZOMBIE ETHER INIT et autres */
  default:
    logMsg("proximityproximityTask: Activity %d status %s incoherent\n",
	    activityId, h2GetEvnStateString(state));
    proximityproximityTaskSuspend (TRUE);
    return(ZOMBIE);
  }  /* Switch state */
}
    
/*****************************************************************************
*
*  filterAndSendEvn  -  Teste et signal les transitions
*
*  Description : Cette fonction teste si une transition d'une activite
*                est valide. Sinon la tache est suspendue.
*
*  Retourne: TRUE s'il faut eveiller la tache de controle.
*/
	
static BOOL filterAndSendEvn (ACTIVITY_STATE state,
			      ACTIVITY_EVENT evn)
{
  BOOL transition=FALSE;
  BOOL wakeUpCntrlTask=FALSE;

  /* Transitions autorisees */
  switch (state) {
  case START:
    if (evn == EXEC || evn == END || evn == FAIL || 
	evn == ETHER || evn == ZOMBIE || evn == SLEEP || evn == START)
      transition = TRUE;
    break;
  case EXEC: 
    if (evn == EXEC || evn == END || evn == FAIL || 
	evn == ETHER || evn == ZOMBIE || evn == SLEEP)
      transition = TRUE;
    break;
  case END:
    if (evn == ZOMBIE || evn == ETHER || evn == END || evn == FAIL)
      transition = TRUE;
    break;
  case FAIL:
    if (evn == ZOMBIE)
      transition = TRUE;
    break;
  case INTER:
    if (evn == ZOMBIE || evn == ETHER || evn == FAIL || evn == INTER)
      transition = TRUE;
    break;
  default:
    logMsg("proximityproximityTask: status %s incoherent\n",
	    h2GetEvnStateString(state));
    proximityproximityTaskSuspend (TRUE);  
  } 

  /* Transition autorisee */
  if (transition) {

#if (!1) /* tache non periodique */
    /* Tache non periodique: Auto reveille sauf evn SLEEP */
    if (evn != SLEEP)
      h2evnSignal(EXEC_TASK_ID(0));
/*    semGive (EXEC_TASK_SEM_ID(0));*/
#endif

    /* Changement d'etat en vu : il faudra prevenir la tache de controle */
    if (state != (ACTIVITY_STATE) evn) 
      wakeUpCntrlTask = TRUE;
  }

  /* Transition interdite */
  else {
    /* Ne pas appeler 2 fois la fonction h2GetEvnStateString dans le logMsg */
    logMsg("proximityproximityTask: event %s ", h2GetEvnStateString(evn));
    logMsg("from state %s not allowed\n", h2GetEvnStateString(state)); 
    proximityproximityTaskSuspend (TRUE);
  }
  return wakeUpCntrlTask;
}

static long
h2timespec_udiff(const H2TIMESPEC *x, const H2TIMESPEC *y)
{
	H2TIMESPEC yy;

	memcpy(&yy, y, sizeof(H2TIMESPEC));

	/* Carry */
	if (x->tv_nsec < y->tv_nsec) {
		long sec = (y->tv_nsec - x->tv_nsec) / 1000000000 + 1;
		yy.tv_nsec -= 1000000000 * sec;
		yy.tv_sec += sec;
	}
	if (x->tv_nsec - y->tv_nsec > 1000000000) {
		int sec = (x->tv_nsec - y->tv_nsec) / 1000000000;
		yy.tv_nsec += 1000000000 * sec;
		yy.tv_sec -= sec;
	}

	return 1000000*(x->tv_sec - yy.tv_sec) 
	    + (x->tv_nsec - yy.tv_nsec)/1000;
}
