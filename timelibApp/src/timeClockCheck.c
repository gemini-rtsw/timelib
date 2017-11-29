
#include <registryFunction.h>
#include <epicsExport.h>
#include <dbDefs.h>
#include <bc635.h>
#include <epicsStdio.h>
#include "timesys.h"


/*+
 *   Function name:
 *   timeClockCheck
 *
 *   Purpose:
 *   Checks the time initialisation parameters 
 *   set by timeClockInit are consistent with the hardware that is present.
 *
 *   Description:
 *   The routine does a number of consistency checks to verify that the 
 *   parameters set in the call to timeClockInit are valid. For example
 *   the IOC may have been booted to read time from a Bancomm card but
 *   actually there is no hardware present to allow this.
 *
 *   Invocation:
 *   timeClockCheck()
 *
 *   External functions:
 *   bcTestCard   (Bancomm library) Check if Bancomm card is present
 *   bc635_read   (Bancomm library) Read raw TAI 
 *   bcReadStat   (Bancomm library) Read status of Bancomm hardware
 *   bcGetGpsLeap (Bancomm library) Fetch number of GPS leap seconds
 *
 *   Prior requirements:
 *   The routine timeClockInit should have been called first
 *
 *   Deficiencies:
 *   The main problem is trying to identify whether a Bancomm 637 or 635
 *   card is present. This has to be done by trying to fetch the number
 *   of GPS leap seconds. If the number comes back as 0 it is a 635, if the
 *   number is positive then it is a 637. 
 *
 *-
 */

void timeClockCheck( )

{

  int simulate ;       /* TRUE if time system is to be simulated */
  int master ;         /* TRUE if this is the timing master */
  int numGPSLeapSecs ; /* no. of leap seconds read from Bancomm */
  int bcReadStat ;     /* status from bc635_read call */
  double tai ;         /* TAI returned from bc635_read */
  int unsynched ;      /* TRUE if Bancomm is not synched */
  int tooLarge ;       /* TRUE if Time offset from Bancomm is too large */
  int freqOffsetErr ;  /* TRUE if freq. offset error from Bancomm */

/* check if Bancomm card is present */

  absent = bcTestCard() ;

/* fetch flags from time system global memory */

  timeGetSimFlag (&simulate) ;
  timeGetMasterIOC (&master) ;

/* Do first consistency check. Has system been started as time bus master
 * but there is no hardware to support this mode ?
 */

  if (absent && !simulate)
  {
   simulate = TRUE ;
   timeSetSimFlag(simulate) ;

/* for present, simply output a message but what really needs to happen 
 * is for TCS:TIME:health to be set to Bad and a message to be logged to
 * the log record
 */

   printf ("Warning: Time being simulated - no Bancomm available\n") ;
  }

/* If a Bancomm has been found then check whether it is a bc637 or bc635
 * and whether it is synched to its time source 
 */

  if (!absent)
  {
   numGPSLeapSecs = bcGetGpsLeap( ) ;
   bcReadStat     = bc635_read(&tai) ;
   unsynched = bcReadStat & 0x01 ;
   tooLarge  = bcReadStat & 0x02 ;
   freqOffsetErr = bcReadStat & 0x04 ;

   if (master)                /* handle cases where master is requested */
   {
    if (numGPSLeapSecs > 0 )  /* a bc637 has been detected */
    {
     if (unsynched)           /* Bancomm not locked to satellite */
     {
/* should really set TCS:TIME:health to Bad and log an erro message
 * instead just use printf for the time being
 */
      printf ("Warning: bc637 is not locked to GPS satellite\n") ;
     }
     else
     {
/* Everything is in order. Should check leap seconds are consistent with
 * slaDat. For time being print a message
 */
      printf ("Bc637 is locked to satellite\n") ;
     }
    }
    else                        /* Number of leap secs is 0 */
    {
     if (unsynched)
     {
/* This is an ambiguous case. Possible options are
 * a) there is a bc637 present but the GPS signal isn't available
 * b) Actually we are running with a bc635 but the IRIG-B signal isn't
 *    synched
 *
 * Since startup has specified this as the master, the assumption is that
 * option a) is correct
 *
 * Should set TCS:TIME:health to Bad and log a message, instead use printf
 */
      printf("Warning: bc637 not synched to satellite\n" ) ;
     }
     else                       /* Bancomm card is synched */
     {
/* This is an error. The user has specified a bc637 but in fact there is a
 * bc635 in the crate. Overide the master flag and warn user
 */
      master = FALSE ;
      timeSetMasterIOC(master) ;
      printf ("Warning: This crate has a bc635 NOT a bc637\n") ;
     }
    }                           /* End number of leap secs = 0 */

   }                           /* End of master section */

   else                        /* Handles cases for a bc635 */
   {
    if (numGPSLeapSecs > 0 )   /* Actually it appears to be a 637 */
    {
     if (unsynched)            /* time signal isn't synched */
     {
/* This is a possible inconsistency but an allowable one. it implies we
 * are using a bc637 in slave mode but it isn't synched.  
 * we should log a message and set TCS:TIME;health to Bad
 */
      printf("Warning: Bancomm not synched to external signal\n") ;
      printf("Warning: This crate contains a bc637 NOT a bc635\n") ;

     }
     else                      /* Time signal is synched */
     {
/* Everything OK except that a bc637 card is present yet the crate has
 * been booted assuming a bc635.
 * Should check that no. of leap secs is consistent with slaDat and log
 * a warning if not
 */
      printf ("Warning: This crate contains a bc637 running in salve mode\n");
 
     }
    }
    else                       /* no. of leap secs = 0 */
    {
     if (unsynched)            /* IRIG-B signal isn't synched */
     {
/* This situation will occur with a bc635 when the IRIG-B signal isn't
 * available. e.g. the TCS may not be running
 */
      printf ("Warning: bc635 not synched to external source\n" );

     }
     else                      /* Normal status for a bc635 */
     {
/* This is the desired state for a crate containing a bc635 */
      printf("Bc635 synched to external time signal\n") ;
     }
    }                          /* End no. of leap secs = 0 */
   }                           /* End handle cases for bc 635 */
 
  }                            /* End if (!absent)  */
}

/* Register these symbols for use by IOC code */

epicsRegisterFunction( timeClockCheck );
