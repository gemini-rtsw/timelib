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
   int simulate;               /* TRUE if time is to be simulated */ 

   /* fetch flags from time global memory */
   timeGetSimFlag ( &simulate );
   //timeGetMasterIOC ( &master );

   /* Set the "initialized" flag so that timeNow doesn't call timeInit. */
   initd = 1;

   // Set the ST offset
   setDelStr();

   return 0;
}
