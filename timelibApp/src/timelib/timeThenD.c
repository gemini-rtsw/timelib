#include "timesys.h"

/*+
 *   Function name:
 *   timeThenD
 *
 *   Purpose:
 *   Convert a raw time into a Modified Julian Date.
 *
 *   Description:
 *   Expresses a previously read raw time as a Modified Julian Date in a
 *   specified timescale.
 *
 *   Invocation:
 *   status = timeThenD (rawt, scale, &datemj)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)    rawt    (double)    raw time, SI seconds since TAI 1970 Jan 1.0
 *      (>)    scale   (timescale) Time scale as an enumerated type
 *      (<)    datemj  (double *)  Modified Julian date (JD - 2400000.5)
 *
 *   Function value:
 *   (<)  status  (int)  Return status, 0 = OK
 *-
 */

int timeThenD ( double rawt, timescale scale, double *datemj )
/*
**  - - - - - - - - - -
**   t i m e T h e n D
**  - - - - - - - - - -
**
**  Express a previously-read raw time as a Modified Julian Date in
**  a specified timescale.
**
**  Given:
**     rawt     double      raw time (SI seconds since TAI 1970 January 1.0)
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
**  Defined in timesys.h:
**    TAI, UTC, UT1 (enumerations)
**
**  Given (global data):
**    initd     int         FALSE = yet to be initialized
**    datlsd    double      current TAI-UTC (days)
**    djmls     double      MJD following next leap second
**    dutd      double      current UT1-UTC (days)
**    dttd      double      TT-TAI (days)
**    deltdbd   double      current TDB-TT (days)
**
**  Notes:
**
**   1) Conversions to UTC are for the current epoch and may not be
**      correct for times in the past or future.  This is because the
**      information about leap seconds is confined to the one last to
**      be announced.
**
**   2) The MJD form of UTC has an ambiguity during a leap second.
**      This is unavoidable.  The choice made here is during a leap
**      second to repeat the final 1/86400 day.
**
**  Called:  timeInit, timeRaw2tai and the present routine
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Last revision:   20 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/

/* 1 second in days: 1/86400 */
#define SEC 0.000011574074074074074074

{
   int j;
   double d;


/* Initialize as required. */
   if ( ! initd ) if ( (j = timeInit ( )) ) return j;

/* Express given raw time in specified timescale. */
   switch ( scale ) {
   case TAI:
      *datemj = timeRaw2tai ( rawt );
      break;
   case UTC:
      if ( (j = timeThenD ( rawt, TAI, &d )) ) return j;
      d -= datlsd;
      *datemj = d - ( d >= djmls ? SEC : 0.0 );
      break;
   case UT1:
      if ( (j = timeThenD ( rawt, TAI, &d )) ) return j;
      *datemj = d - datlsd + dutd;
      break;
   case TT:
      if ( (j = timeThenD ( rawt, TAI, &d )) ) return j;
      *datemj = d + dttd;
      break;
   case TDB:
      if ( (j = timeThenD ( rawt, TT, &d )) ) return j;
      *datemj = d + deltdbd;
      break;
   default:
      return -1;
   }
   return 0;
}
