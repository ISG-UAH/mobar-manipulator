#include "proximityError.h"

static const H2_ERROR const proximityH2errMsgs[] = PROXIMITY_H2_ERR_MSGS;

int proximityRecordH2errMsgs(void)
{
  /* std errors from genom */
  genomRecordH2errMsgs();

  /* proximity errors */
  if (!h2recordErrMsgs("proximityRecordH2errMsgs", "proximity", M_proximity, 
		      sizeof(proximityH2errMsgs)/sizeof(H2_ERROR),
		      proximityH2errMsgs))
    return 0;

  /* others modules errros */
  

  return 1;
}
