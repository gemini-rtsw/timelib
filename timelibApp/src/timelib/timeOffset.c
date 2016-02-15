#include <math.h>
#include "timesys.h"

/*+
 *   Function name:
 *   timeOffset
 *
 *   Purpose:
 *   Set the offset-for-testing
 *
 *   Description:
 *   Set the offset-for-testing, which the timeNow function will then add
 *   to the clock reading. The purpose of this function is to allow 
 *   running the observatory on a time offset from the present for 
 *   testing purposes.
 *
 *   Invocation:
 *   status = timeOffset(offset)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)    offset   (double)   Offset (in days)
 *
 *   Function value:
 *   (<)  status  (int)  Return status, 0 = OK
 *-
 */

int timeOffset ( double offset )
/*
**  - - - - - - - - - - -
**   t i m e O f f s e t
**  - - - - - - - - - - -
**
**  Set the offset-for-testing, which the timeNow function adds to
**  the clock reading.
**
**  Given:
**     offset   double      offset (days)
**
**  Returned (global data):
**     biass    double      offset for testing (sec):  added to clock reading
**
**  Returned (function value):
**              int         status:  0 = OK
**                                  -1 = excessive bias
**  Notes:
**
**  1  A positive offset value will cause the time library to return times
**     that are in the future, and a negative offset produces times in the
**     the past.
**
**  2  Obvious uses for this "time travel" facility include (i) recreating
**     the circumstances under which some malfunction was observed, and
**     (ii) rehearsing some time-critical future observing programme.
**
**  3  Applications need to call timeOffset only when the subsystem
**     concerned requires its own time bias for some reason (e.g. stand-
**     alone testing).  Normally, a master bias held by the TCS is used
**     to synchronize all subsystems to the same epoch.
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Last revision:   23 April 1996
**
**  Copyright 1996 RAL.  All rights reserved.
*/
{
   if ( fabs ( offset ) > 20000.0 ) {
      return -1;
   } else {
      biass = offset * 86400.0;
      return 0;
   }
}
