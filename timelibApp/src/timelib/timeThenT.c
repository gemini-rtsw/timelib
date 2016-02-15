#include <math.h>
#include <slalib.h>
#include "timesys.h"

/*+
 *   Function name:
 *   timeThenT
 *
 *   Purpose:
 *   Converts a raw time into a time of day.
 *
 *   Description:
 *   Expresses a previously read raw time as a time of day in a specified
 *   timescale.
 *
 *   Invocation:
 *   status = timeThenT (rawt, scale, ndp, hmsf)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)   rawt  (double)     raw time, SI seconds since TAI 1970 Jan. 1.0
 *      (>)   scale  (timescale) Time scale as an enumerated type
 *      (>)   ndp   (int)        number of digits in fraction
 *      (<)   hmsf  (int[4])     hour, min, sec, fraction
 *
 *   Function value:
 *   (<)  status  (int)  Return status, 0 = OK
 *-
 */

int timeThenT ( double rawt, timescale scale, int ndp, int *hmsf )
/*
**  - - - - - - - - - -
**   t i m e T h e n T
**  - - - - - - - - - -
**
**  Express a previously-read raw time as a time of day in a
**  specified timescale.
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
**  Called:  timeThenD, timeThenR, slaDd2tf
**
**  Last revision:   20 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/

#define D2PI 6.283185307179586476925286766559

{
   int j;
   double t, d;
   char s;


/* Express the clock time as a time of day in the specified timescale. */
   switch ( scale ) {
   case TAI:
   case UTC:
   case UT1:
   case TT:
   case TDB:
      if ( (j = timeThenD ( rawt, scale, &t )) ) return j;
      t = t - floor ( t );
      break;

   case GMST:
   case LAST:
      if ( (j = timeThenR ( rawt, scale, &t )) ) return j;
      t /= D2PI;
      break;

   default:
      return -1;
   }

/* Format the time of day. */
   slaDd2tf ( ndp, t, &s, hmsf );

/* If rounded past 24 hours, wrap. */
   if ( hmsf [ 0 ] >= 24 ) hmsf [ 0 ] -= 24;

/* Handle UTC leap second case. */
   if ( ( scale == UTC ) && ( hmsf [ 2 ] == 59 ) ) {
      if ( (j = timeThenD ( rawt, TAI, &t )) ) return j;
      d = 86400.0 * ( t - datlsd - djmls );
      if ( d >= 0.0 && d < 1.0 ) hmsf [ 2 ] = 60;
   }

/* Exit. */
   return 0;
}
