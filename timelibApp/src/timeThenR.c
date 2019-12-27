#include <slalib.h>
#include "timesys.h"

/*+
 *   Function name:
 *   timeThenR
 *
 *   Purpose:
 *   Converts a raw time into a sidereal time
 *
 *   Description:
 *   Express a previously read raw time as a sidereal time (in radians) in
 *   a specified timescale.
 *
 *   Invocation:
 *   status = timeThenR (rawt, scale, &theta)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)   rawt   (double)    raw time, SI seconds since TAI 1970 Jan. 1.0
 *      (>)   scale  (timescale) Time scale as an enumerated type
 *      (<)   theta  (double *)  Sidereal time (in radians)
 *
 *   Function value:
 *   (<) status  (int)  Return status, 0 = OK
 *-
 */

int timeThenR ( double rawt, timescale scale, double *theta )
/*
**  - - - - - - - - - -
**   t i m e T h e n R
**  - - - - - - - - - -
**
**  Express a previously-read raw time as an angle in radians in a
**  specified sidereal timescale.
**
**  Given:
**     rawt     double      raw time (SI seconds since TAI 1970 January 1.0)
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
**  Defined in timesys.h:
**    GMST, LAST (enumerators)
**
**  Defined in timesys.h:
**    delstr    double      current LAST-GMST (radians)
**
**  Note:  The conversions (to GMST and LAST) are for the current epoch
**         and may not be correct for times in the past or future.  This
**         is because the value of UT1-UTC that is used is for the
**         present, and because information about leap seconds is
**         confined to the one last to be announced.
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Called:  timeThend, slaGmst, slaDranrm and the present routine
**
**  Last revision:   20 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/
{
   int j;
   double t, d;


/* Express given raw time in specified timescale. */
   switch ( scale ) {
   case GMST:
      t = rawt / 86400.0;
      d = floor ( t );
      t = slaGmsta ( 40587.0 + d, ( t - d ) - datlsd + dutd );
      break;
   case LAST:
      if ( (j = timeThenR ( rawt, GMST, &t )) ) return j;
      t += delstr;
      break;
   default:
      return -1;
   }
   *theta = slaDranrm ( t );
   return 0;
}
