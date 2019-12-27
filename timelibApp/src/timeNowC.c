#include "timeLib.h"

/*+
 *   Function name:
 *   timeNowC
 *
 *   Purpose:
 *   Read the date and time
 *
 *   Description:
 *   Reads the time from the Gemini Time Service and expresses it in a 
 *   specifed timescale as a calendar date and time.
 *
 *   Invocation:
 *   status = timeNowC(scale, ndp, ymdhmsf)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)    scale    (timescale) Enumerated type specifying timescale
 *      (>)    ndp      (int)       number of digits in fraction
 *      (<)    ymdhmsf  (int[7])    year, month, day, hour, min, sec, fraction
 *
 *   Function value:
 *   (<)   status   (int)  return status, 0 = OK
 * 
 *-
 */

int timeNowC ( timescale scale, int ndp, int *ymdhmsf )
/*
**  - - - - - - - - -
**   t i m e N o w C
**  - - - - - - - - -
**
**  Read the time from the Gemini Time Service and express it in a
**  specified timescale as a calendar date and time.
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
**     ndp      int         number of digits required in fraction
**
**  Returned:
**     ymdhmsf  int[7]      year, month, day, hour, min, sec, fraction
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
**   2) The largest usable ndp value depends on the architecture of the
**      machine, in particular on the size of the int type.  A value of
**      4 or less is safe.
**
**   3) An ndp value less than zero is interpreted as zero.
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Called:  timeNow, timeThenC
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
   return timeThenC ( tai, scale, ndp, ymdhmsf );
}
