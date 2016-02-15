#include "timeLib.h"

/*+
 *   Function name:
 *   timeNowR
 *
 *   Purpose:
 *   Read the Sidereal time (in radians)
 *
 *   Description:
 *   Read the time from the Gemini time service and express it in a 
 *   specified sidereal timescale as an angle in radians.
 *
 *   Invocation:
 *   status = timeNowR (scale, &theta)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)    scale    (timescale)  Timescale as an enumerated type
 *      (<)    theta    (double *)   Angle in radians
 *
 *   Function value:
 *   (<)  status   (int)  Return status, 0 = OK
 * 
 *-
 */

int timeNowR ( timescale scale, double *theta )
/*
**  - - - - - - - - -
**   t i m e N o w R
**  - - - - - - - - -
**
**  Read the time from the Gemini Time Service and express it in a
**  specified sidereal timescale as an angle in radians.
**
**  Given:
**     scale    timescale   timescale of result:
**
**                            GMST = Greenwich Mean Sidereal Time
**                            LAST = Local Apparent Sidereal Time
**
**  Returned:
**     theta    double*     angle in radians
**
**  Returned (function value):
**              int         status:  0 = OK
**
**  Note:  The conversions (to GMST and LAST) are for the current epoch
**         and may not be accurate for times in the past or future.
**         This is because the value of UT1-UTC that is used is for the
**         present, and because information about leap seconds is confined
**         to the one last to be announced.
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Called:  timeNow, timeThenR
**
**  Last revision:   19 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/
{
   int j;
   double tai;

/* Read the clock. */
   if ( (j = timeNow ( &tai )) ) return j;

/* Express the clock TAI in the specified timescale. */
   return timeThenR ( tai, scale, theta );
}
