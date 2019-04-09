/*------------------  Fichier généré automatiquement ------------------*/
/*------------------  Ne pas éditer manuellement !!! ------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <portLib.h>
#include <csLib.h>
#include <posterLib.h>
#include "proximityPrint.h"

void print_struct_PROXIMITY_STATUS_POSTER_STR(FILE *out, struct PROXIMITY_STATUS_POSTER_STR *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_NB_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%ssensors:\n", indstr);
    print_struct_PROXIMITYSENSORS(out, &((x+elt)->sensors), indent, 0, NULL, in);

  } END_FOR
  free(indstr);
}

void print_struct_PROXIMITY_STR(FILE *out, struct PROXIMITY_STR *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_NB_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%ssensors:\n", indstr);
    print_struct_PROXIMITYSENSORS(out, &((x+elt)->sensors), indent, 0, NULL, in);

  } END_FOR
  free(indstr);
}

void print_struct_PROXIMITYSENSORS(FILE *out, struct PROXIMITYSENSORS *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_NB_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%snum_left_sensors = ", indstr);
    print_int(out, &((x+elt)->num_left_sensors), 0, 0, NULL, in);

    fprintf(out, "%sleft_sensors[10]:\n", indstr);
    { int dims[1] = {10};
      print_double(out, (double *)((x+elt)->left_sensors), indent, 1, dims, in); }

    fprintf(out, "%snum_right_sensors = ", indstr);
    print_int(out, &((x+elt)->num_right_sensors), 0, 0, NULL, in);

    fprintf(out, "%sright_sensors[10]:\n", indstr);
    { int dims[1] = {10};
      print_double(out, (double *)((x+elt)->right_sensors), indent, 1, dims, in); }

    fprintf(out, "%snum_top_sensors = ", indstr);
    print_int(out, &((x+elt)->num_top_sensors), 0, 0, NULL, in);

    fprintf(out, "%stop_sensors[10]:\n", indstr);
    { int dims[1] = {10};
      print_double(out, (double *)((x+elt)->top_sensors), indent, 1, dims, in); }

    fprintf(out, "%snum_back_sensors = ", indstr);
    print_int(out, &((x+elt)->num_back_sensors), 0, 0, NULL, in);

    fprintf(out, "%sback_sensors[10]:\n", indstr);
    { int dims[1] = {10};
      print_double(out, (double *)((x+elt)->back_sensors), indent, 1, dims, in); }

  } END_FOR
  free(indstr);
}


/* ======================== PRINT DES TYPEDEF ============================= */

void print_PROXIMITYSENSORS(FILE *out, PROXIMITYSENSORS *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_NB_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%sPROXIMITYSENSORS:\n", indstr);
    print_struct_PROXIMITYSENSORS(out, &(*(x+elt)), indent, 0, NULL, in);

  } END_FOR
  free(indstr);
}

void print_PROXIMITY_STR(FILE *out, PROXIMITY_STR *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_NB_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%sPROXIMITY_STR:\n", indstr);
    print_struct_PROXIMITY_STR(out, &(*(x+elt)), indent, 0, NULL, in);

  } END_FOR
  free(indstr);
}

void print_PROXIMITY_STATUS_POSTER_STR(FILE *out, PROXIMITY_STATUS_POSTER_STR *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf1[80], buf2[80];
  char *indstr;
  indstr=strdup(indentStr_r(nDim?++indent:indent, buf1));
  indent++;
  FOR_NB_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s\n", indentStr_r(indent-2, buf1), getIndexesStr_r(nDim, dims, elt, buf2));

    fprintf(out, "%sPROXIMITY_STATUS_POSTER_STR:\n", indstr);
    print_struct_PROXIMITY_STATUS_POSTER_STR(out, &(*(x+elt)), indent, 0, NULL, in);

  } END_FOR
  free(indstr);
}


