/***************************************************************/
/*                                                             */
/*  Please note that there are two different versions of this  */
/*  function, for online use (under VxWorks) and offline use   */
/*  (under Unix) respectively.  The alternative sets of code   */
/*  are selected through the vxWorks environment variable.     */
/*                                                             */
/***************************************************************/



#include <epicsTime.h>
#include "timesys.h"
#include "stdio.h"

#if defined (vxWorks) || defined (__rtems__)

#include <bc635.h>

/*
**  --------------
**  ONLINE VERSION
**  --------------
*/



/*+
 *   Function name:
 *   timeNow
 *
 *   Purpose:
 *   Read the raw time (including any offset-for-testing)
 *
 *   Description:
 *   If running on a vxWorks system, read the current raw time from the Gemini Time Service.
 *   If running off line on a Unix system then read the Unix clock. In this case, the
 *   highest resolution clock available is used (this may only give a resultion of 1 second)
 *   In both cases the raw time returned is in SI seconds since 1970 January 1.0 TAI.
 *
 *   Invocation:
 *   status = timeNow (&rawt)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (<)    rawt     (double)   raw time (SI seconds since 1970 Jan 1.0 TAI)
 *
 *   Function value:
 *   (<)  status     (int)  0 = OK
 * 
 *   External functions:
 *   bc635_read  (Bancomm library) Read raw time
 *   clock_gettime (clockLib) Fetch current time as a timespec structure
 *
 *   External variables:
 *   (>)  biass    (double)   offset for testing (sec):  added to clock reading
 *
 *-
 */


int timeNow ( double *rawt )
/*
**  - - - - - - - -
**   t i m e N o w
**  - - - - - - - -
**
**  Read the current raw time, including the offset-for-testing.
**
**  Returned:
**     rawt     double*     raw time (SI seconds since TAI 1970 January 1.0)
**
**  Given (global data):
**     biass    double      offset for testing (sec):  added to clock reading
**
**  Returned (function value):
**              int         status:  0 = OK
**
**  Called:  bc635_read
**           clock_gettime
**
**  If time is being simulated the time library must be initialised so
**  that datlsd is available.
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Last revision:   20 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/
{
   int j;
   struct timespec tspec;

   if (tsim && absent)
   {

	/*printf("tsim=%d,absent=%d\n", tsim, absent);*/

      if ( (j = clock_gettime(CLOCK_REALTIME, &tspec )) ) return j;
      *rawt = tspec.tv_sec + (double)tspec.tv_nsec / 1000000000.0;
      if ( !initd ) if ( (j = timeInit()) ) return j;
      *rawt += datlsd * 86400.0;
   }
   else
   {

/* Why zero all error bits here ? */
      if ( (j = bc635_read ( rawt )) & (~0x07) ) return j;
   }
   *rawt += biass;
   return 0;
}

#else

/*
**  ---------------
**  OFFLINE VERSION
**  ---------------
*/

int timeNow ( double *rawt )
/*
**  - - - - - - - -
**   t i m e N o w
**  - - - - - - - -
**
**  Read the current raw time, including the offset-for-testing.
**
**  USE OF THIS ROUTINE IN OFFLINE APPLICATIONS IS DEPRECATED.
**
**  Returned:
**     rawt     double*     platform-dependent, but assumed to be SI
**                          seconds since TAI 1970 January 1.0
**
**  Given (global data):
**     datlsd   double      TAI-UTC before latest leap second (days)
**     biass    double      offset for testing (sec):  added to clock reading
**
**  Returned (function value):
**              int         status:  0 = OK
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Last revision:   20 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/
{

#ifdef _POSIX_TIMERS
  
   struct timespec tspec ;

   (void) clock_gettime (CLOCK_REALTIME, &tspec ) ;
   *rawt = (double)tspec.tv_sec + (double)tspec.tv_nsec/1000000000.0
           + datlsd * 86400.0 + biass;

#else

   *rawt = (double) time ( (time_t*) NULL ) + datlsd * 86400.0 + biass;

#endif
   return 0;
}

#endif
