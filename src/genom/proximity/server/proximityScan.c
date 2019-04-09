/*------------------  Fichier généré automatiquement ------------------*/
/*------------------  Ne pas éditer manuellement !!! ------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <portLib.h>
#include <csLib.h>
#include <posterLib.h>
#include "proximityScan.h"

int scan_struct_PROXIMITY_STATUS_POSTER_STR(FILE *in, FILE *out,
                 struct PROXIMITY_STATUS_POSTER_STR *x, int indent, int nDim, int *dims)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_EACH_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%ssensors:\n", indstr);
    if (scan_struct_PROXIMITYSENSORS(in, out, &(x+elt)->sensors, 
                     indent, 0, NULL) == ABORT) {
      free (indstr);
      return ABORT; }

  } END_FOR
  free(indstr);
  return OK;
}

int scan_struct_PROXIMITY_STR(FILE *in, FILE *out,
                 struct PROXIMITY_STR *x, int indent, int nDim, int *dims)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_EACH_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%ssensors:\n", indstr);
    if (scan_struct_PROXIMITYSENSORS(in, out, &(x+elt)->sensors, 
                     indent, 0, NULL) == ABORT) {
      free (indstr);
      return ABORT; }

  } END_FOR
  free(indstr);
  return OK;
}

int scan_struct_PROXIMITYSENSORS(FILE *in, FILE *out,
                 struct PROXIMITYSENSORS *x, int indent, int nDim, int *dims)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_EACH_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%snum_left_sensors ", indstr);
    if (scan_int(in, out, &(x+elt)->num_left_sensors, 
                     0, 0, NULL) == ABORT) {
      free (indstr);
      return ABORT; }

    fprintf(out, "%sleft_sensors[10]:\n", indstr);
    { int dims[1] = {10};
      if (scan_double(in, out, (double *)(x+elt)->left_sensors, 
                     indent, 1, dims) == ABORT) {
      free (indstr);
      return ABORT; } }

    fprintf(out, "%snum_right_sensors ", indstr);
    if (scan_int(in, out, &(x+elt)->num_right_sensors, 
                     0, 0, NULL) == ABORT) {
      free (indstr);
      return ABORT; }

    fprintf(out, "%sright_sensors[10]:\n", indstr);
    { int dims[1] = {10};
      if (scan_double(in, out, (double *)(x+elt)->right_sensors, 
                     indent, 1, dims) == ABORT) {
      free (indstr);
      return ABORT; } }

    fprintf(out, "%snum_top_sensors ", indstr);
    if (scan_int(in, out, &(x+elt)->num_top_sensors, 
                     0, 0, NULL) == ABORT) {
      free (indstr);
      return ABORT; }

    fprintf(out, "%stop_sensors[10]:\n", indstr);
    { int dims[1] = {10};
      if (scan_double(in, out, (double *)(x+elt)->top_sensors, 
                     indent, 1, dims) == ABORT) {
      free (indstr);
      return ABORT; } }

    fprintf(out, "%snum_back_sensors ", indstr);
    if (scan_int(in, out, &(x+elt)->num_back_sensors, 
                     0, 0, NULL) == ABORT) {
      free (indstr);
      return ABORT; }

    fprintf(out, "%sback_sensors[10]:\n", indstr);
    { int dims[1] = {10};
      if (scan_double(in, out, (double *)(x+elt)->back_sensors, 
                     indent, 1, dims) == ABORT) {
      free (indstr);
      return ABORT; } }

  } END_FOR
  free(indstr);
  return OK;
}


/* ======================== SCAN DES TYPEDEF ============================= */

int scan_PROXIMITYSENSORS(FILE *in, FILE *out,
                 PROXIMITYSENSORS *x, int indent, int nDim, int *dims)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_EACH_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%s*PROXIMITYSENSORS:\n", indstr);
    if (scan_struct_PROXIMITYSENSORS(in, out, x+elt, 
                     indent, 0, NULL) == ABORT) {
      free (indstr);
      return ABORT; }

  } END_FOR
  free(indstr);
  return OK;
}

int scan_PROXIMITY_STR(FILE *in, FILE *out,
                 PROXIMITY_STR *x, int indent, int nDim, int *dims)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_EACH_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%s*PROXIMITY_STR:\n", indstr);
    if (scan_struct_PROXIMITY_STR(in, out, x+elt, 
                     indent, 0, NULL) == ABORT) {
      free (indstr);
      return ABORT; }

  } END_FOR
  free(indstr);
  return OK;
}

int scan_PROXIMITY_STATUS_POSTER_STR(FILE *in, FILE *out,
                 PROXIMITY_STATUS_POSTER_STR *x, int indent, int nDim, int *dims)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_EACH_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%s*PROXIMITY_STATUS_POSTER_STR:\n", indstr);
    if (scan_struct_PROXIMITY_STATUS_POSTER_STR(in, out, x+elt, 
                     indent, 0, NULL) == ABORT) {
      free (indstr);
      return ABORT; }

  } END_FOR
  free(indstr);
  return OK;
}


