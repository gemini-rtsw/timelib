#include "timeLib.h"

/*+
 *   Function name:
 *   timeNowD
 *
 *   Purpose:
 *   Read the Modified Julian Date
 *
 *   Description:
 *   Read the time from the Gemini Time Service and express it in a 
 *   specified timescale as a Modified Julian Date.
 *
 *   Invocation:
 *   status = timeNowD (scale, &datemj)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)    scale    (timescale) Time scale as enumeratd type
 *      (<)    datemj   (double *)  Modified Julain Date (JD - 2400000.5)
 *
 *   Function value:
 *   (<)  status  (int)   Return status, o = OK
 *
 *-
 */

int timeNowD ( timescale scale, double *datemj )
/*
**  - - - - - - - - -
**   t i m e N o w D
**  - - - - - - - - -
**
**  Read the time from the Gemini Time Service and express it in a
**  specified timescale as a Modified Julian Date.
**
**  Given:
**     scale    timescale   timescale of result:
**
**                            TAI  = International Atomic Time
**                            UTC  = Coordinated Universal Time
**                            UT1  = Universal Time
**                            TT   = Terrestrial Time
**                            TDB  = Barycentric Dynamical Time
**
**  Returned:
**     datemj   double*     Modified Julian Date (JD - 2400000.5)
**
**  Returned (function value):
**              int         status:  0 = OK
**
**  Notes:
**
**   1) Conversions to UTC are for the current epoch and may not be
**      correct for times in the past or future.  This is because the
**      information about leap seconds is confined to the one last to
**      be announced.
**
**   2) The MJD form of UTC has an ambiguity during a leap second.
**      This is unavoidable.  The choice made here (or rather in the
**      timeThenD function) is during a leap second to repeat the final
**      1/86400 day.
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Called:  timeNow, timeThenD
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
   return timeThenD ( tai, scale, datemj );
}
