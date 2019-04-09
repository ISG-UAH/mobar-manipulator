/*------------------  Fichier généré automatiquement ------------------*/
/*------------------  Ne pas éditer manuellement !!! ------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <portLib.h>
#include <csLib.h>
#include <posterLib.h>
#include "proximityPrintXML.h"

void printXML_struct_PROXIMITY_STATUS_POSTER_STR(FILE *out, char *name, struct PROXIMITY_STATUS_POSTER_STR *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf[80];
  char *indstr;
  indstr=strdup(indentStr2_r(indent, buf));
  indent++;
  FOR_NB_elt(nDim,dims) {
    fprintfBuf(out, "%s<%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));

    printXML_struct_PROXIMITYSENSORS(out, "sensors", &((x+elt)->sensors), indent, 0, NULL, in);

    fprintfBuf(out, "%s</%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));
  } END_FOR
  free(indstr);
}

void printXML_struct_PROXIMITY_STR(FILE *out, char *name, struct PROXIMITY_STR *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf[80];
  char *indstr;
  indstr=strdup(indentStr2_r(indent, buf));
  indent++;
  FOR_NB_elt(nDim,dims) {
    fprintfBuf(out, "%s<%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));

    printXML_struct_PROXIMITYSENSORS(out, "sensors", &((x+elt)->sensors), indent, 0, NULL, in);

    fprintfBuf(out, "%s</%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));
  } END_FOR
  free(indstr);
}

void printXML_struct_PROXIMITYSENSORS(FILE *out, char *name, struct PROXIMITYSENSORS *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf[80];
  char *indstr;
  indstr=strdup(indentStr2_r(indent, buf));
  indent++;
  FOR_NB_elt(nDim,dims) {
    fprintfBuf(out, "%s<%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));

    printXML_int(out, "num_left_sensors", &((x+elt)->num_left_sensors), indent, 0, NULL, in);

    { int dims[1] = {10};
      printXML_double(out, "left_sensors", (double *)((x+elt)->left_sensors), indent, 1, dims, in); }

    printXML_int(out, "num_right_sensors", &((x+elt)->num_right_sensors), indent, 0, NULL, in);

    { int dims[1] = {10};
      printXML_double(out, "right_sensors", (double *)((x+elt)->right_sensors), indent, 1, dims, in); }

    printXML_int(out, "num_top_sensors", &((x+elt)->num_top_sensors), indent, 0, NULL, in);

    { int dims[1] = {10};
      printXML_double(out, "top_sensors", (double *)((x+elt)->top_sensors), indent, 1, dims, in); }

    printXML_int(out, "num_back_sensors", &((x+elt)->num_back_sensors), indent, 0, NULL, in);

    { int dims[1] = {10};
      printXML_double(out, "back_sensors", (double *)((x+elt)->back_sensors), indent, 1, dims, in); }

    fprintfBuf(out, "%s</%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));
  } END_FOR
  free(indstr);
}


/* ======================== PRINT DES TYPEDEF ============================= */

void printXML_PROXIMITYSENSORS(FILE *out, char *name, PROXIMITYSENSORS *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf[80];
  char *indstr;
  indstr=strdup(indentStr2_r(indent, buf));
  indent++;
  FOR_NB_elt(nDim,dims) {
    fprintfBuf(out, "%s<%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));

    printXML_struct_PROXIMITYSENSORS(out, "PROXIMITYSENSORS", &(*(x+elt)), indent, 0, NULL, in);

    fprintfBuf(out, "%s</%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));
  } END_FOR
  free(indstr);
}

void printXML_PROXIMITY_STR(FILE *out, char *name, PROXIMITY_STR *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf[80];
  char *indstr;
  indstr=strdup(indentStr2_r(indent, buf));
  indent++;
  FOR_NB_elt(nDim,dims) {
    fprintfBuf(out, "%s<%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));

    printXML_struct_PROXIMITY_STR(out, "PROXIMITY_STR", &(*(x+elt)), indent, 0, NULL, in);

    fprintfBuf(out, "%s</%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));
  } END_FOR
  free(indstr);
}

void printXML_PROXIMITY_STATUS_POSTER_STR(FILE *out, char *name, PROXIMITY_STATUS_POSTER_STR *x,
                      int indent, int nDim, int *dims, FILE *in)
{
  char buf[80];
  char *indstr;
  indstr=strdup(indentStr2_r(indent, buf));
  indent++;
  FOR_NB_elt(nDim,dims) {
    fprintfBuf(out, "%s<%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));

    printXML_struct_PROXIMITY_STATUS_POSTER_STR(out, "PROXIMITY_STATUS_POSTER_STR", &(*(x+elt)), indent, 0, NULL, in);

    fprintfBuf(out, "%s</%s%s>\n", indstr, name, getIndexesStr2_r(nDim, dims, elt, buf));
  } END_FOR
  free(indstr);
}


