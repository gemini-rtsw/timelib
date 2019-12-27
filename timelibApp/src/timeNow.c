/***************************************************************/
/*                                                             */
/*  Please note that there are two different versions of this  */
/*  function, for online use (under VxWorks) and offline use   */
/*  (under Unix) respectively.  The alternative sets of code   */
/*  are selected through the vxWorks environment variable.     */
/*                                                             */
/*  20190701  mdw  The above is no longer true. There is now   */
/*                 just one version that calls the EPICS OSI   */
/*                 layer to get the current time of day.       */
/***************************************************************/



#include <epicsTime.h>
#include "timesys.h"

/*
**  -----------------
**  EPICS OSI VERSION
**  -----------------
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
**  Called:  epicsGetTimeCurrent
**
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Last revision:   20 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/
{
   epicsTimeStamp ts;
   epicsTimeGetCurrent(&ts);
   *rawt = ts.secPastEpoch + ts.nsec/1000000000.0; /* convert to a double */
   *rawt += datlsd/86400.0 + biass;                /* add leap seconds and testing offset */
   *rawt += TS_EPICS_TO_UNIX_EPOCH;                /* convert from EPICS to Unix epoch */
   return 0;
}
