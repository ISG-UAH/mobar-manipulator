/*
 * SubSystem_Defs.h
 *
 *  Created on: 15/07/2014
 *  Author: Juan A. Mora Prieto
 */

#ifndef SUBSYSTEM_DEFS_H_
#define SUBSYSTEM_DEFS_H_

/** Number of defined subsystems. */
#define N_SUBSYSTEMS 3

/** Minimum subsystem number. */
#define MIN_SUBSYSTEM_NUMBER 1

/** Numerical equivalence of the subsystems. */
#define S_LOCOMOTION    3
#define S_CLAMP                 2
#define S_PROXIMITY     1

/** Subsystems name. */
#define S_LOCOMOTION_NAME       "locomotionMB"
#define S_CLAMP_NAME            "clampMB"
#define S_PROXIMITY_NAME        "proximity"

/** Max string length for subsystem name. */
#define S_NAME_MAX_LENGTH 32

/** Check if a S/S number is valid. */
#define VALID_SS_NUMBER(ssnum) \
 (ssnum > 0 && ssnum <= N_SUBSYSTEMS ? TRUE : FALSE)

/** Check subsystem status. */
#define SS_STATUS_OK(sstat) \
 (sstat >= S_STAT_FAIL || sstat <= S_STAT_FAIL_OFF ? FALSE : TRUE)

/** Subsystem status definition. */
#define S_STAT_FAIL_OFF     -1
#define S_STAT_OFF          0  // Less than 0 are power off
#define S_STAT_ON           1  // Greater than 0 are power on (inclusive fail or int)
#define S_STAT_OK           2  // Implies SS is initialized
#define S_STAT_INT          10
#define S_STAT_FAIL         20 // FAIL codes
#define S_STAT_FAIL_POWER   21
#define S_STAT_FAIL_INIT    22

/** Mailbox parameters for subsystem connection. */
typedef struct MailBOX {
    int  activity;
    int  report;
    //int idcpbox;                      /* id client */
    int  idSsClient;                    /* id subsystem client */
    int  idSsTarget;                    /* id subsystem target server */
    char mboxname[32];
} MailBOX;

#endif /* SUBSYSTEM_DEFS_H_ */
