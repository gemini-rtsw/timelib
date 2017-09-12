
#include <epicsTime.h>
#include <bc635.h>
#include "timesys.h"

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
  epicsTimeStamp now;
  unsigned long pNSecs;


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
    if (epicsTimeGetCurrent(&now) != 0)
        return -1;

    if (epicsTimeToTimespec(&tspec, &now) != 0)
        return -1;

    /* Commented out the lines that convert from UNIX time.
     * The legacy code was using TSgetUnixTime; the new code
     * calls epicsTimeGetCurrent. Substracting the leap seconds
     * is not needed either.
      taiNTP = tspec.tv_sec + (double)tspec.tv_nsec/1000000000.0 
                            + datlsd * 86400.0 
                            - TS_1900_TO_UNIX_EPOCH ;
     */
      taiNTP = tspec.tv_sec + (double)tspec.tv_nsec/1000000000.0;
      *BCmNTP = taiBC - taiNTP ;
      /* printf("Bancomm Time: %f , NTP Time %f , difference: %f, NTP Time human: %s \n", taiBC, taiNTP, *BCmNTP, now ); */
   }
  }
  return readstat ;
} 
