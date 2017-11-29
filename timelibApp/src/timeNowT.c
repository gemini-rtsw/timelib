#include "timeLib.h"

/*+
 *   Function name:
 *   timeNowT
 *
 *   Purpose:
 *   Read time of day
 *
 *   Description:
 *   Read the time from the Gemini time service and express it in a
 *   specifed timescale as a time of day.
 *
 *   Invocation:
 *   status = timeNowT (scale, ndp, hmsf)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)   scale   (timescale)  Time scale as an enumerated type
 *      (>)   ndp     (int)        Number of digits required in fraction
 *      (<)   hmsf    (int[4])     hour, min, sec, fraction
 *
 *   Function value:
 *   (<)  status  (int) Return status, 0 = OK
 *-
 */

int timeNowT ( timescale scale, int ndp, int *hmsf )
/*
**  - - - - - - - - -
**   t i m e N o w T
**  - - - - - - - - -
**
**  Read the time from the Gemini Time Service and express it in a
**  specified timescale as a time of day.
**
**  Given:
**     scale    timescale   timescale of result:
**
**                            TAI  = International Atomic Time
**                            UTC  = Coordinated Universal Time
**                            UT1  = Universal Time
**                            TT   = Terrestrial Time
**                            TDB  = Barycentric Dynamical Time
**                            GMST = Greenwich Mean Sidereal Time
**                            LAST = Local Apparent Sidereal Time
**
**     ndp      int         number of digits required in fraction
**
**  Returned:
**     hmsf     int[4]      hour, min, sec, fraction
**
**  Returned (function value):
**              int         status:  0 = OK
**
**  Notes:
**
**   1) Conversions to GMST, LAST and UTC are for the current epoch and
**      may not be correct for times in the past or future.  This is
**      because the value of UT1-UTC that is used is for the present,
**      and because information about leap seconds is confined to the
**      one last to be announced.
**
**   2) The largest usable ndp value depends on the architecture of the
**      machine, in particular on the size of the int type.  A value of
**      4 or less is safe.
**
**   3) An ndp value less than zero is interpreted as zero.
**
**   4) Times wrap at 24 hours;  date information is discarded.
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Called:  timeNow, timeThenT
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

/* Express the clock time in the specified timescale and format. */
   return timeThenT ( tai, scale, ndp, hmsf );
}
