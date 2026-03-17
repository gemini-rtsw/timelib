
#include <registryFunction.h>
#include <epicsExport.h>
#include <dbDefs.h>
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
 *   @deprecatedThe routine timeClockInit should have been called first
 *
 *   Deficiencies:
 *   The main problem is trying to identify whether a Bancomm 637 or 635
 *   card is present. This has to be done by trying to fetch the number
 *   of GPS leap seconds. If the number comes back as 0 it is a 635, if the
 *   number is positive then it is a 637. 
 *
 *-
 */

void timeClockCheck()

{

     int simulate;       /* TRUE if time system is to be simulated */

     /* Do first consistency check. Has system been started as time bus master
      * but there is no hardware to support this mode ?
      */
     simulate = TRUE;
     timeSetSimFlag(simulate);

     printf("2025 NOTICE!: Time System Default to NTP. Bancomm Hardware and Libraries Require Base Build < stable/2025-R7.01\n");
}

/* Register these symbols for use by IOC code */
epicsRegisterFunction( timeClockCheck );
