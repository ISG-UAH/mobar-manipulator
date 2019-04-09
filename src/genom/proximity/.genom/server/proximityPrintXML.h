
/* 
 * Copyright (c) 2003 LAAS/CNRS
 * Sara Fleury
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

#ifndef PROXIMITY_PRINTXML_H
#define PROXIMITY_PRINTXML_H

#include "genom/printScan.h"
#include "genom/printXMLProto.h"

/* Structures definies par d'autres modules */


#include "proximityPosterLib.h"

/* Prototypes */
extern void printXML_struct_PROXIMITY_STATUS_POSTER_STR ( FILE *out, char *name,
     struct PROXIMITY_STATUS_POSTER_STR *x,
     int indent, int nDim, int *dims, FILE *in );
extern void printXML_struct_PROXIMITY_STR ( FILE *out, char *name,
     struct PROXIMITY_STR *x,
     int indent, int nDim, int *dims, FILE *in );
extern void printXML_struct_PROXIMITYSENSORS ( FILE *out, char *name,
     struct PROXIMITYSENSORS *x,
     int indent, int nDim, int *dims, FILE *in );

/* ======================== PRINT DES TYPEDEF ============================= */

extern void printXML_PROXIMITYSENSORS ( FILE *out, char *name,
     PROXIMITYSENSORS *x,
     int indent, int nDim, int *dims, FILE *in );
extern void printXML_PROXIMITY_STR ( FILE *out, char *name,
     PROXIMITY_STR *x,
     int indent, int nDim, int *dims, FILE *in );
extern void printXML_PROXIMITY_STATUS_POSTER_STR ( FILE *out, char *name,
     PROXIMITY_STATUS_POSTER_STR *x,
     int indent, int nDim, int *dims, FILE *in );

#endif /* PROXIMITY_PRINT_XML_H */

