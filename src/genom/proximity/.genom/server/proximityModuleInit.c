
/* --- FILE GENERATED WITH GENOM, DO NOT EDIT BY HAND ------------------ */

/* 
 * Copyright (c) 2004-2005
 *      Autonomous Systems Lab, Swiss Federal Institute of Technology.
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

/* module proximity initialization routines */

#if defined(__RTAI__) && defined(__KERNEL__)
# include <linux/init.h>
# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/sched.h>

#else
# include <stdio.h>
#  include <sys/param.h>
#  include <stdlib.h>
#  include <fcntl.h>
#  include <string.h>
#  include <errno.h>
#  include <unistd.h>
#  include <sys/utsname.h>

#  define PID_FILE
#endif /* RTAI && KERNEL */

#include <portLib.h>
#include <shellLib.h>
#include <h2initGlob.h>

#include <taskLib.h>
#include <errnoLib.h>
#include <h2evnLib.h>

#include "proximityHeader.h"
#include "commonStructLib.h"
#include "proximityMsgLib.h"

#include "genom/genom.h"

#define CNTRL_TASK_MIN_STACK_SIZE 4096
#define EXEC_TASK_MIN_STACK_SIZE  4096


/* --- Global variables ---------------------------------------------- */

/* Initialization semaphores */
SEM_ID semproximityCntrlTaskInit;
SEM_ID semproximityInitExecTab[PROXIMITY_NB_EXEC_TASK];

/* Les taches */
void *proximityCntrlTask (void *);
extern void *proximityproximityTask(void *);

typedef struct {
	char *name;
	int priority;
	int size;
	void *(*func)(void *);
} PROXIMITY_EXEC_TASK_STR;

PROXIMITY_EXEC_TASK_STR proximityExecTaskTab[] = {
	{ "proximityproximityTask", 100, 20000, proximityproximityTask } };


/* Internal data structures */
struct PROXIMITY_STR *proximityDataStrId = NULL;
PROXIMITY_CNTRL_STR *proximityCntrlStrId = NULL;

#ifdef PID_FILE
/* name of the file containing module's PID */
char pidFilePath[MAXPATHLEN];
#endif

/****************************************************************************
*
*  proximityTaskInit  -  Initialisation de l'module de proximity
*
*  Description:
*  Routine d'initialisation du module proximity, a etre appelee au moment
*  du boot.
*
*  Retourne: OK ou ERROR
*/

STATUS
proximityTaskInit()
{
  int i;
#ifdef PID_FILE
  FILE* pidFile;
  int pidFileFd;
  char *pidDir;
  struct utsname uts;
#endif /* PID_FILE */
  char strerr[64];

  /*
   * Create internal data structures
   */

  /* SDI_f */
  if (commonStructCreate (sizeof (struct PROXIMITY_STR), 
			  (void *) &proximityDataStrId) 
      != OK)  {
    h2perror ("proximityTaskInit: cannot create the Functional Data Base");
    goto error;
  }
  
  /* SDI_c */
  if (commonStructCreate (sizeof (PROXIMITY_CNTRL_STR), 
			  (void *) &proximityCntrlStrId) 
      != OK)  {
    h2perror ("proximityTaskInit: cannot create the Control Data Base");
    goto error;
  }

#ifdef PID_FILE
  /*
   * Create pid file
   */
  pidDir = getenv("H2DEV_DIR");
  if (pidDir == NULL) {
      pidDir = getenv("HOME");
  }
  if (pidDir == NULL) {
      pidDir = "/tmp";
  }
  if (uname(&uts) == -1) {
      errnoSet(errno);
      goto error;
  }
  snprintf(pidFilePath, MAXPATHLEN, "%s/.%s.pid-%s", pidDir, 
      genomInstanceName("proximity"), uts.nodename);
  pidFileFd = open(pidFilePath, O_CREAT|O_EXCL|O_WRONLY, 0644);
  if (pidFileFd < 0) {
      fprintf(stderr, "proximity: error creating %s: %s\n",
	      pidFilePath, strerror(errno));
      goto error;
  }
  pidFile = fdopen(pidFileFd, "w");
  if (pidFile == NULL) {
      perror("proximity: error opening pid file");
      goto error;
  }
  fprintf(pidFile, "%d ", getpid());
  fputc('\n', pidFile);
  fclose(pidFile);
#endif /* PID_FILE */

  /* 
   * Spawn control task
   */
  logMsg("proximity: spawning control task.\n");

  semproximityCntrlTaskInit = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
  if (semproximityCntrlTaskInit == NULL) {
     h2perror("proximityTaskInit: cannot create control semaphore");
     goto error; 
  }
  
  if (taskSpawn2(genomInstanceSuffixName("proximity", "CntrlTask"), 
	  10 /* priorite */, VX_FP_TASK,
		PROXIMITY_MAX_RQST_SIZE + CNTRL_TASK_MIN_STACK_SIZE /*size*/, 
	  proximityCntrlTask, NULL) == ERROR) {
     h2perror("proximityTaskInit: cannot spawn proximityCntrlTask");
     goto error;
  }
  /* wait for initialization */
  if (semTake(semproximityCntrlTaskInit, WAIT_FOREVER) != OK)
     goto error;
  
  semDelete(semproximityCntrlTaskInit);
  semproximityCntrlTaskInit = NULL;

  /* check task status */
  if (CNTRL_TASK_STATUS == ERROR) {
     errnoSet(CNTRL_TASK_BILAN);
     h2perror("proximity: Control task failed");
     goto error;
  }

  /* 
   * Spawn execution tasks
   */
  for (i=0; i<PROXIMITY_NB_EXEC_TASK; i++) {
    logMsg("proximity: spawning task %s.\n", proximityExecTaskTab[i].name);

    semproximityInitExecTab[i] = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
    if (semproximityInitExecTab[i] == NULL) {
       h2perror("proximityTaskInit: cannot create init semaphore");
       goto error; 
    }
    
    if (taskSpawn2(proximityExecTaskTab[i].name,
		  proximityExecTaskTab[i].priority, VX_FP_TASK, 
		  proximityExecTaskTab[i].size + EXEC_TASK_MIN_STACK_SIZE, 
	    proximityExecTaskTab[i].func, NULL) == ERROR) {
       h2perror("proximityTaskInit: cannot spawn exec task");
       goto error;
    }
    /* wait for initialization */
    if (semTake(semproximityInitExecTab[i], WAIT_FOREVER) != OK) {
       h2perror("proximityTaskInit: semTake init sem");
       goto error;
    }
    
    semDelete(semproximityInitExecTab[i]);
    semproximityInitExecTab[i] = NULL;

    /* check task status */
    if (EXEC_TASK_STATUS(i) == ERROR) {
       logMsg("proximityTaskInit: Exec task %s failed : %s\n", 
	      proximityExecTaskTab[i].name, h2getErrMsg(EXEC_TASK_BILAN(i), strerr, 64));
       errnoSet(EXEC_TASK_BILAN(i));
       goto error;
    }
  }

  /* wake up control task once for poster initialization */
  h2evnSignal(CNTRL_TASK_ID);

  /*
   * Module is ready
   */
  logMsg("proximity: all tasks are spawned\n");

  return OK;

  error:
  /* Destroy initialization semaphores */
  for (i=0; i<PROXIMITY_NB_EXEC_TASK; i++) if (semproximityInitExecTab[i]) {
     semDelete(semproximityInitExecTab[i]);
     semproximityInitExecTab[i] = NULL;
  }

  if (semproximityCntrlTaskInit) {
     semDelete(semproximityCntrlTaskInit);
     semproximityCntrlTaskInit = NULL;
  }

  /* Destroy internal data structures */
  if (proximityCntrlStrId) {
     commonStructDelete(proximityCntrlStrId);
     proximityCntrlStrId = NULL;
  }

  if (proximityDataStrId) {
     commonStructDelete(proximityDataStrId);
     proximityDataStrId = NULL;
  }
  return ERROR;
}


#if defined(__RTAI__) && defined(__KERNEL__)

/*
 * --- Module initialization for RTAI -----------------------------------
 *
 */

MODULE_DESCRIPTION("proximity module");

int
proximity_module_init(void)
{
   if (proximityTaskInit() == ERROR) {
      /* we don't report an error to insmod: there might still be
       * suspended tasks lying around and the module's code must remain
       * in memory. */
      h2perror("Error moduleInit");
   }

   return 0;
}

void
proximity_module_exit(void)
{
   ;
}

module_init(proximity_module_init);
module_exit(proximity_module_exit);

#endif /* RTAI && KERNEL */


#if !defined(__KERNEL__)
/*
 * --- main for unix ----------------------------------------------------
 */
static char *prompt = "proximity> ";
extern char *optarg;
extern int optind;

static void 
usage(void)
{
    fprintf(stderr, "usage: proximity [-b|-e|-s] [-f freq]\n");
    exit(3);
}

int
main(int argc, char *argv[])
{
    int c;
    /* Options par defaut */
    int backgroundOpt = 0, essaiOpt = 0, shellOpt = 0;
    int frequence = 100;		/* frequence par defaut */
    int errFlag = 0;
    int waitPipe[2];
    char buf[80];

    /* Parse les options */
    while ((c = getopt(argc, argv, "besf:")) != -1) {
	switch (c) {
	  case 'b':
	    backgroundOpt++;
	    break;
	  case 'e':
	    if (shellOpt) {
		errFlag++;
	    } else {
		essaiOpt++;
	    }
	  case 's':
	    if (essaiOpt) {
		errFlag++;
	    } else {
		shellOpt++;
	    }
	    break;
	  case 'f':
	    frequence = atoi(optarg);
	    break;
	  case '?':
	    errFlag++;
	    break;
	} /* switch */
    } /* while */
    if (errFlag) {
	/* Erreur */
	usage();
    }

    if (backgroundOpt) {
	if (shellOpt || essaiOpt) {
	    /* Erreur */
	    usage();
	}
	/* Creation d'un pipe pour envoyer un message a la fin de l'init */
	pipe(waitPipe);
	
	/* Lancement en background */
	if (fork() != 0) {
	    /* close(waitPipe[1]); */
	    /* Attend le lancement du module par une lecture sur le pipe */
	    if (read(waitPipe[0], buf, sizeof(buf)) <= 0) {
		fprintf(stderr, 
			"proximity moduleInit: error waiting for module start\n");
	    } 
	    exit(0);
	}
	/* close(waitPipe[0]); */
    }

    /* Initialise environnement comLib */
#if 1
    if ((0) > 0)
      errFlag = h2initGlob(0);
    else
      errFlag = h2initGlob(100);
#else
    errFlag = h2initGlob(0);
#endif
    if (errFlag == ERROR) {
	/* informe la tache en avant plan */
	write(waitPipe[1], "proximity: h2initGlob error\n", 17); 
	exit(2);
    }
    /* Demarrage des taches du module */
    if (proximityTaskInit() == ERROR) {
	printf("proximity: Error moduleInit: ");
	h2printErrno(errnoGet());
	/* informe la tache en avant plan */
	write(waitPipe[1], "proximity: moduleTaskInit error\n", 21); 
	exit(1);
    }
    if (shellOpt) {
	/* Lance un shell */
	shellMainLoop(stdin, stdout, stderr, prompt);
    } else if (essaiOpt) {
	fprintf(stderr, "proximity: not implemented\n");
    } 
    /* informe la tache en avant plan */
    if (backgroundOpt) {
	write(waitPipe[1], "proximity: OK\n", 3);
	/* close(waitPipe[1]); */
    }
    /* Wait forever */
    while (1) {
	select(64, NULL, NULL, NULL, NULL);
    }
    unlink(pidFilePath);
    exit(0);
}
#endif /* !__KERNEL__ */
