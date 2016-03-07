
#include <epicsTime.h>
#include <bc635.h>
#include "timesys.h"

/* Add declaration of TSgetUnixTime. This should be got from an include
 * file but the only definition is in drvTS.c which we don't want to 
 * include.
*/

long TSgetUnixTime (struct timespec* ts) ;

/*+
 *   Function name:
 *   timeCheck
 *
 *   Purpose:
 *   Compare and check time standards
 *
 *   Description:
 *   This routine will check if the time returned by the Bancomm card
 *   is consistent with that fetched by NTP. This is only done if
 *   the time system isn't being simulated and reading the Bancomm
 *   returns an error. The routine is needed to try and separate
 *   serious errors reported by the Bancomm from less serious ones.
 *   For example, the Bancomm 637 will report errors whilst it is
 *   locking to the satellite signals, this can take 10s of minutes
 *   during which time a perfectly satisfactory time signal is 
 *   being generated
 *
 *   Invocation:
 *   timeCheck(&diff)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (<)    timeDiff (double)   Difference between BC and NTP (secs)
 *
 *   Function value:
 *   (<) status  (int) Status from bc635_read. Possible values are 
 *                     
 *           bit0 => time is unsynched
 *           bit1 => time offset too large
 *           bit2 => Frequency offset error
 * 
 *   External functions:
 *    bc635_read  (Bancomm library) Reads raw TAI
 *    TSgetUnixTime (EPICS Time Stamp library) Fetch UTC from NTP server
 *
 *-
 */

int timeCheck (double *BCmNTP)
{
  int readstat ;               /* status return from bc635_read */
  double taiBC ;               /* TAI returned by Bancomm */
  double taiNTP ;              /* TAI derived from NTP */
  struct timespec tspec ;
  int simulate ;               /* Flag if time is being simulated */

  readstat = 0 ;
  *BCmNTP  = 0.0 ;

  timeGetSimFlag(&simulate) ;

  if (!simulate)
  {
   readstat = bc635_read(&taiBC) & 0x07 ;
   if (readstat)
   {
/* If it is not possible to read a time from the NTP server then simply
 * return the error codes from the bc635_read
 */
    if (TSgetUnixTime(&tspec))
      return readstat ;
    else
    {
      taiNTP = tspec.tv_sec + (double)tspec.tv_nsec/1000000000.0 
                            + datlsd * 86400.0 
                            - TS_1900_TO_UNIX_EPOCH ;
      *BCmNTP = taiBC - taiNTP ;
    }
   }
  }
  return readstat ;
} 
