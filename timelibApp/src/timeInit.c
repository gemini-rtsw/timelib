/***************************************************************/
/*                                                             */
/*  Please note that there are two different versions of this  */
/*  function, for online use (under VxWorks) and offline use   */
/*  (under Unix) respectively.  The alternative sets of code   */
/*  are selected through the vxWorks environment variable.     */
/*                                                             */
/***************************************************************/


#include <dbDefs.h>
#include <slalib.h>
#include <epicsTime.h>
#include "timesys.h"
#include "bc635Aliases.h"
#include <epicsStdio.h>
#include <epicsExport.h>
#include <errlog.h>

static void setDelStr() {
   double tdb;

/* LAST minus GMST */
   timeNowD ( TDB, &tdb );
   delstr = slaDranrm ( elongt + slaEqeqx ( tdb ) );
}


/* Add forward declaration of TSgetUnixTime. This is declared in drvTS.c 
 * not drvTS.h and so can't be included here
*/
/*+
 *   Function name:
 *   timeInit
 *
 *   Purpose:
 *   Initialize (or re-initialize) the Gemini time library.
 *
 *   Description:
 *   This routine initialises the Gemini time library. If it is not 
 *   called explicitly it will be called automatically when one of
 *   the other time library routines are called. The routine performs
 *   a number of consistency checks to ensure that the correct hardware
 *   is present to support the mode in which the time library is being
 *   run.
 *
 *   Invocation:
 *   timeInit()
 *
 *   Function value:
 *   (<)  status   (int)  Return status, 0 = OK, else = error
 * 
 *   External variables:
 *   (<)  initd   (int)  Initialised flag
 *   (>)  elongt  (double) True longitude of site (radians)
 *   (<)  delstr  (double) current LAST - GMST (radians)
 *
 *-
 */


int timeInit ( )
/*
**  - - - - - - - - -
**   t i m e I n i t
**  - - - - - - - - -
**
**  Initialize (or re-initialize) the Gemini time library.
**
**  Returned (global data):
**     initd    int       "initialized" flag:  set true
**     biass    double    offset for testing (sec):  zeroed
**     dttd     double    TT-TAI (days)
**     djmls    double    MJD following the next leap second
**     datlsd   double    TAI-UTC before that date (days)
**     dutd     double    UT1-UTC before that date (days)
**     deltdbd  double    current TDB-TT (days)
**     delstr   double    current LAST-GMST (radians)
**
**  Returned (function value):
**              int       status:  0 = OK
**
**  Notes:
**
**  1)  This function is called automatically by the other time routines
**      if they detect that the system has not yet been initialized.
**
**  2)  Applications can elect to call this function themselves, in
**      order to control when the associated computations occur.
**
**  3)  Another reason for an explicit call is to update the various
**      items, all of which are slowly varying.
**
**  4)  For a year or so after a leap second, the TCS will supply a value
**      for djmls which is some arbitrarily-large number.  Only when a new
**      leap second has been announced will djmls receive an actual date.
**      Once the leap second has occurred, the TCS initialization
**      arrangements need to make the following three changes all at
**      once:
**
**               * the MJD for next leap second set to (say) 999999.9.
**               * the current TAI-UTC incremented by 1 second
**               * the current UT1-UTC incremented by 1 second.
**
**      The time functions will continue to work correctly until these
**      changes have been made.
**
**      The only required TCS action on announcement of a leap second is
**      to replace the 999999.9 with the actual MJD.
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Last revision:   3 May 1996
**
**  Copyright 1996 RAL.  All rights reserved.
*/
{

   int j;
   struct timespec tspec;
   int simulate;               /* TRUE if time is to be simulated */
   int master;                 /* TRUE if this is the timing master */
   time_t gpstime;             /* GPS time */
   struct tm *gtime;           /* GPS time */
   char rtctime[15];           /* String to send to RTC */
   epicsTimeStamp now;
   

/* First check that parameters given with timeClockInit are consistent
 * with the hardware that is present. This repeats the tests of 
 * timeClockCheck so that routine may be able to be scrapped 
 */

   absent = bcTestCard ( );

/* fetch flags from time global memory */

   timeGetSimFlag ( &simulate );
   timeGetMasterIOC ( &master );

   if (absent == 127) // Fake Bancomm driver
   {
       errlogPrintf("timelib: timeInit detected a fake bancomm driver. Initializing ST and returning\n");
       goto bail;
   }

/* First consistency check. Has system been started in non-simulate mode
 * but there is no hardware to support this ?
 */

   if (absent && !simulate)
   {
      simulate = TRUE;
      timeSetSimFlag ( simulate );
   }

/* Set the CPU RTC. If this results in an error then only report the fact
 * if running in simulate mode. This is equivalent to interpreting 
 * "simulate" to mean "standalone"
 */
    if (epicsTimeGetCurrent(&now) != 0)
        return -1;

    if (epicsTimeToTimespec(&tspec, &now) != 0)
        return -1;

   tspec.tv_sec = tspec.tv_sec - TS_1900_TO_UNIX_EPOCH;
   j = clock_settime ( CLOCK_REALTIME, &tspec );
   if (simulate && j) return j;

/* If not simulating then read the Bancomm hardware. If there are no errors
 * then there is no problem but there can be many causes of a non-zero
 * return. Cases identified so afar are
 *
 * bc635 - no input IRIG-B or signal not locked
 *      This is a serious problem. If the simulate flag is set then put
 *      the Bancomm into RTC mode using the best estimate of GPS time
 *      to set the clock. If not simulating then we expect the IRIG-B
 *      signal to soon be available. 
 * bc637 - hardware reboot of crate
 *      Time readout goes to zero for 10 to 15 seconds. Then it locks to
 *      correct time but bits 0 (unsynched) and 2 (freqoffset) stay
 *      set for upto 60 seconds and 10 minutes respectively. 
 * bc637 - software reboot
 *      bit 0 (unsynched) can be set for 10 to 15 seconds
 */


/* If the system is to be run in simulation BUT there is Bancomm hardware 
 * available then use its RTC. When running in this mode, calls to
 * bc635_read always appear to come back with bit 0 clear but bits
 * 1 & 2 set. 
 */

   if (!absent && simulate)
   {
      gpstime = (time_t)(tspec.tv_sec + datlsd*86400.0 - 19);
      gtime   = gmtime( &gpstime );
/* make sure time offset is 0 */
      bcSendTfp("M+00");
      sprintf ( rtctime, "%02hd%02hd%02hd%02hd%02hd%02hd%c", (gtime->tm_year)%100, 
                gtime->tm_mon+1, gtime->tm_mday, gtime->tm_hour, gtime->tm_min,
                gtime->tm_sec,'\0' );
/*
   AJF - If you want to force the date and time to Dec 31st 2005, 23:59:45
         in the startup of the TCS, this is where you do it!
      sprintf(rtctime, "%s", "L051231235945");
*/
      bcSendTfp ( rtctime );
      bcSendTfp( "A3" );
   }


bail:
/* Set the "initialized" flag so that timeNow doesn't call timeInit. */
   initd = 1;

   // Set the ST offset
   setDelStr();

   return 0;
}

epicsExportAddress(int, absent);
