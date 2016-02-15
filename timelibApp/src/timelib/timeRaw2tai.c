#include "timesys.h"

/*+
 *   Function name:
 *   timeRaw2tai
 *
 *   Purpose:
 *   Convert a raw time to TAI
 *
 *   Description:
 *   Convert a raw time to TAI
 *
 *   Invocation:
 *   tai = timeRaw2tai (raw)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)   raw     (double)     Raw time
 *
 *   Function value:
 *   (<)  tai  (double) TAI
 *-
 */

double timeRaw2tai ( double raw )
/*
**  - - - - - - - - - - - -
**   t i m e R a w 2 t a i
**  - - - - - - - - - - - -
**
**  Convert a raw time into TAI.
**
**  Given:
**     raw      double      raw time
**
**  Returned (function value):
**              double      TAI as MJD (JD-2400000.5)
**
**  Defined in timesys.h:
**    TRAW_EPOCH (MJD for raw time zero)
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Last revision:   20 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/
{
   return raw / 86400.0 + TRAW_EPOCH;
}
