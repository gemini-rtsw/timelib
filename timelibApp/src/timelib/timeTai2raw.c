#include "timesys.h"

/*+
 *   Function name:
 *   timeTai2raw
 *
 *   Purpose:
 *   Convert a TAI int a raw time
 *
 *   Description:
 *   Convert a TAI int a raw time
 *
 *   Invocation:
 *   raw = timeTai2raw (tai)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)   tai     (double)     TAI
 *
 *   Function value:
 *   (<)  raw  (double) Raw time
 *-
 */

double timeTai2raw ( double taimjd )
/*
**  - - - - - - - - - - - -
**   t i m e T a i 2 r a w
**  - - - - - - - - - - - -
**
**  Convert a TAI into a raw time.
**
**  Given:
**     tai      double      TAI as MJD (JD-2400000.5)
**
**  Returned (function value):
**              double      raw time
**
**  Defined in timesys.h:
**    TRAW_EPOCH (MJD for raw time zero)
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Last revision:   19 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/
{
   return ( taimjd - TRAW_EPOCH ) * 86400.0;
}
