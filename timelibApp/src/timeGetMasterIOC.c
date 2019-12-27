

#include "timesys.h"

/*+
 *   Function name:
 *   timeGetMasterIOC
 *
 *   Purpose:
 *   Get flag that indicates if this is the timebus master
 *
 *   Description:
 *   Fetch the masterIOC flag from the time library global memory
 *
 *   Invocation:
 *   timeGetMasterIOC (&master)
 *
 *   Parameters: (">" input, "!" modified, "<" output)
 *    (<)  master   (int *)    returned masterIOC flag
 *
 *   External variables:
 *    (>) masterIOC  (int)  The flag that says if this is the master IOC
 *
 *   Prior requirements:
 *   timeSetMasterIOC should have been called prior to this function
 *
 *-
 */

void timeGetMasterIOC (int *master)

{

  *master = masterIOC ;

}
