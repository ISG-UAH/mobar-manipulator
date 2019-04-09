
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

/*------------------  Fichier généré automatiquement ------------------*/
/*------------------  Ne pas éditer manuellement !!! ------------------*/


#ifndef proximity_ERROR_H
#define proximity_ERROR_H

#include "h2errorLib.h"
#include "genom/genomError.h"

#define M_proximity    (2005)

/* -- MODULES ERRORS -------------------------------------------------- */

/* demo errors */
#define S_proximity_BACK_COLLISION       H2_ENCODE_ERR(M_proximity , 1)
#define S_proximity_BACK_NEAR_OBSTACLE   H2_ENCODE_ERR(M_proximity , 2)
#define S_proximity_TOP_COLLISION        H2_ENCODE_ERR(M_proximity , 3)
#define S_proximity_TOP_NEAR_OBSTACLE    H2_ENCODE_ERR(M_proximity , 4)
#define S_proximity_RIGHT_COLLISION      H2_ENCODE_ERR(M_proximity , 5)
#define S_proximity_RIGHT_NEAR_OBSTACLE  H2_ENCODE_ERR(M_proximity , 6)
#define S_proximity_LEFT_COLLISION       H2_ENCODE_ERR(M_proximity , 7)
#define S_proximity_LEFT_NEAR_OBSTACLE   H2_ENCODE_ERR(M_proximity , 8)


/* std errors */
#define S_proximity_stdGenoM_ACTIVITY_INTERRUPTED       131458049   /* 2005<<16 | 0x8000 | 100<<8 | 1 */
#define S_proximity_stdGenoM_TOO_MANY_ACTIVITIES        131458050   /* 2005<<16 | 0x8000 | 100<<8 | 2 */
#define S_proximity_stdGenoM_ACTIVITY_FAILED            131458051   /* 2005<<16 | 0x8000 | 100<<8 | 3 */
#define S_proximity_stdGenoM_WAIT_ABORT_ZOMBIE_ACTIVITY 131458052   /* 2005<<16 | 0x8000 | 100<<8 | 4 */
#define S_proximity_stdGenoM_UNKNOWN_ACTIVITY           131458053   /* 2005<<16 | 0x8000 | 100<<8 | 5 */
#define S_proximity_stdGenoM_FORBIDDEN_ACTIVITY_TRANSITION 131458054   /* 2005<<16 | 0x8000 | 100<<8 | 6 */
#define S_proximity_stdGenoM_SYSTEM_ERROR               131458055   /* 2005<<16 | 0x8000 | 100<<8 | 7 */
#define S_proximity_stdGenoM_ACTIVITY_ALREADY_ENDED     131458056   /* 2005<<16 | 0x8000 | 100<<8 | 8 */
#define S_proximity_stdGenoM_WAIT_INIT_RQST             131458057   /* 2005<<16 | 0x8000 | 100<<8 | 9 */
#define S_proximity_stdGenoM_CONTROL_CODEL_ERROR        131458058   /* 2005<<16 | 0x8000 | 100<<8 | 10 */
#define S_proximity_stdGenoM_EXEC_TASK_SUSPENDED        131458059   /* 2005<<16 | 0x8000 | 100<<8 | 11 */
#define S_proximity_stdGenoM_BAD_BLOCK_TYPE             131458060   /* 2005<<16 | 0x8000 | 100<<8 | 12 */
#define S_proximity_stdGenoM_BAD_POSTER_TYPE            131458061   /* 2005<<16 | 0x8000 | 100<<8 | 13 */



/* static H2_ERROR[] */
#define PROXIMITY_H2_ERR_MSGS {\
    {"BACK_COLLISION", 1},\
    {"BACK_NEAR_OBSTACLE", 2},\
    {"TOP_COLLISION", 3},\
    {"TOP_NEAR_OBSTACLE", 4},\
    {"RIGHT_COLLISION", 5},\
    {"RIGHT_NEAR_OBSTACLE", 6},\
    {"LEFT_COLLISION", 7},\
    {"LEFT_NEAR_OBSTACLE", 8},\
}

#ifdef __cplusplus
extern "C" {
#endif

extern int proximityRecordH2errMsgs(void);

#ifdef __cplusplus
}
#endif


/*-------------------------- end file loading ---------------------------*/
#endif
