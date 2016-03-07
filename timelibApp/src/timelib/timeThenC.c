#include <epicsMath.h>
#include <slalib.h>
#include "timesys.h"

/*+
 *   Function name:
 *   timeThenC
 *
 *   Purpose:
 *   Convert a raw time into a date and time 
 *
 *   Description:
 *   Express a previously read raw time as a claendar date and time in
 *   a specified timescale.
 *
 *   Invocation:
 *   status = timeThenC (rawt, scale, ndp, ymdhmsf)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)    rawt   (double)    Raw time, SI seconds since TAI 1970 Jan. 1.0
 *      (>)    scale  (timescale) Timescale as an enumerated type
 *      (>)    ndp    (int)       No. of digits in fraction
 *      (<)    ymdhmsf (int[7])   Year, month, day, hour, min, sec, fraction
 *
 *   Function value:
 *   (<)  status  (int) Return status, 0 = OK
 *-
 */

int timeThenC ( double rawt, timescale scale, int ndp, int *ymdhmsf )
/*
**  - - - - - - - - - -
**   t i m e T h e n C
**  - - - - - - - - - -
**
**  Express a previously-read raw time as a calendar date and time in
**  in a specified timescale.
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
**     ndp      int         number of digits required in fraction
**
**  Returned:
**     ymdhmsf  int[7]      year, month, day, hour, min, sec, fraction
**
**  Returned (function value):
**              int         status:  0 = OK
**
**  Given (global data):
**     djmls    double      MJD following next UTC leap second
**     datlsd   double      TAI-UTC before djmls
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
**  Called:  timeThenD, slaDd2tf, slaDjcl
**
**  Last revision:   20 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/
{
   int j;
   double t, d, f;
   char s;


/* Express the clock time as an MJD in the specified timescale. */
   if ( (j = timeThenD ( rawt, scale, &t )) ) return j;

/* Separate days and fraction of a day. */
   d = floor ( t );
   f = t - d;

/* Format the time of day. */
   slaDd2tf ( ndp, f, &s, ymdhmsf + 3 );

/* If rounded past 24 hours, carry. */
   if ( ymdhmsf [ 3 ] >= 24 ) {
      ymdhmsf [ 3 ] -= 24;
      d++;
   }

/* Handle UTC leap second case. */
   if ( ( scale == UTC ) && ( ymdhmsf [ 5 ] == 59 ) ) {
      if ( (j = timeThenD ( rawt, TAI, &t )) ) return j;
      f = 86400.0 * ( t - datlsd - djmls );
      if ( f >= 0.0 && f < 1.0 ) ymdhmsf [ 5 ] = 60;
   }

/* Format the date, and exit. */
   slaDjcl ( d, ymdhmsf, ymdhmsf + 1, ymdhmsf + 2, &f, &j );
   return j; 
}
