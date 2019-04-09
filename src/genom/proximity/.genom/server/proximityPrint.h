
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

#ifndef PROXIMITY_PRINT_H
#define PROXIMITY_PRINT_H

#include "genom/printScan.h"

/* Structures definies par d'autres modules */


#include "proximityPosterLib.h"

/* Prototypes */
#ifdef __cplusplus
extern "C" {
#endif
extern void print_struct_PROXIMITY_STATUS_POSTER_STR ( FILE *out,
     struct PROXIMITY_STATUS_POSTER_STR *x,
     int indent, int nDim, int *dims, FILE *in );
extern void print_struct_PROXIMITY_STR ( FILE *out,
     struct PROXIMITY_STR *x,
     int indent, int nDim, int *dims, FILE *in );
extern void print_struct_PROXIMITYSENSORS ( FILE *out,
     struct PROXIMITYSENSORS *x,
     int indent, int nDim, int *dims, FILE *in );

/* ======================== PRINT DES TYPEDEF ============================= */

extern void print_PROXIMITYSENSORS ( FILE *out,
     PROXIMITYSENSORS *x,
     int indent, int nDim, int *dims, FILE *in );
extern void print_PROXIMITY_STR ( FILE *out,
     PROXIMITY_STR *x,
     int indent, int nDim, int *dims, FILE *in );
extern void print_PROXIMITY_STATUS_POSTER_STR ( FILE *out,
     PROXIMITY_STATUS_POSTER_STR *x,
     int indent, int nDim, int *dims, FILE *in );
#ifdef __cplusplus
}
#endif

#endif /* PROXIMITY_PRINT_H */

