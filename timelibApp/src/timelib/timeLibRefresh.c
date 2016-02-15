#include <slalib.h>
#include "timesys.h"


/*+
 *   Function name:
 *   timeLibRefresh
 *
 *   Purpose:
 *   Updates slowly-changing variables in timeLib internal memory
 *
 *   Description:
 *   Store new values of TDB-TT and LAST-GMST in internal memory
 *
 *   Invocation:
 *   timeLibRefresh ( )
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)  deltdb    (double)   current TDB-TT (days)
 *
 *   External variables:
 *   (>)  elongt    (double)   east longitude (true, radians)
 *   (<)  deltdbd   (double)   current TDB-TT (days)
 *   (<)  delstr    (double)   current LAST/GMST (radians)
 *-
 */


void timeLibRefresh ( double deltdb )
{
  double tdb;

  deltdbd = deltdb;
  timeNowD ( TDB, &tdb );
  delstr = slaDranrm ( elongt + slaEqeqx ( tdb ) );
}
