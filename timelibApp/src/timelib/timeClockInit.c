#include <vxWorks.h>
#include <drvTS.h>
#include <bc635.h>
#include <sysLib.h>
#include "timesys.h"

#include <registryFunction.h>
#include <epicsExport.h>


/*+
 *   Function name:
 *   timeClockInit
 *
 *   Purpose:
 *   Initialise the clock used by the Gemini time system
 *   This routine must be called prior to starting EPICS.
 *
 *   Description:
 *   This routine is essential to the correct startup of the Gemini time
 *   system. It must be called prior to the startup of EPICS since it 
 *   calls BCconfigure. Since EPICS is not running when this routine is
 *   called it uses private memory to store whether the time system is
 *   to be simulated and whether this IOC is the time bus master. These
 *   stored values can be used later by EPICS code.
 *   The master flag is set to 1 if the IOC in which this code is running is 
 *   to be the time bus master i.e. it has a bc637 card installed.
 *   The simulate flag is set to 1 if time is to be simulated i.e time
 *   is to be driven by an onboard real time clock rather than a Bancomm
 *   card.
 *
 *   Invocation:
 *   timeClockInit(master, simulate, intPerSecond, intPerTick, timeOffset)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)    master   (int)      1 if timebus master 0 otherwise
 *      (>)    simulate (int)      1 if time is simulated 0 therwise
 *      (>)    intPerSecond (int)  Frequency of interrupts (Hz)
 *      (>)    intPerTick (int)    No. of interrupts per clock tick
 *      (>)    timeOffset (int)    Compensation offset (microsec)
 *
 *   Function value:
 *   (<) status   (int)  0 = OK
 * 
 *   External functions:
 *    BCconfigure (Bancomm library)  Configure Bancomm time card
 *
 *-
 */


int timeClockInit ( int master, int simulate, int intPerSecond,
                    int intPerTick, int timeOffset )
/*
**  - - - - - - - - - - - - - -
**   t i m e C l o c k I n i t
**  - - - - - - - - - - - - - -
**
**  Initialise the clock used by the Gemini time system. This routine
**  must be called prior to starting EPICS.
**
**  Given :
**     master   int         TRUE if this IOC has the Bancomm master (bc637)
**     simulate int         TRUE if the real time clock should be used
**                          instead of the bancomm
**
**  Returned (function value):
**              int         status:  0 = OK
**
**  Called:  BCconfigure
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Last revision:   28 March 1996
**
**  Copyright 1996 RAL.  All rights reserved.
*/
{
   int j;
   int tickint;

/* Save the simulate flag  and master flags in the time library global memory */

   timeSetSimFlag(simulate) ;
   timeSetMasterIOC (master) ;

   if (!simulate)
   {

/*  Configure the Bancomm hardware to GPS time, frequency 60Hz, 1 interrupt
    per clock tick, 0 ms offset. */

      BCconfigure (master, 1, intPerSecond, intPerTick, timeOffset);

/* always return 0 at the moment as there is no return value from BCconfigure
 */
      return 0;
   }
   else
   {

/* set the system clock rate */

      tickint = intPerTick;
      if (tickint < 2) tickint = 1;
      j = sysClkRateSet(intPerSecond/tickint);
      return j;
   }
}

/* Register these symbols for use by IOC code */

epicsRegisterFunction( timeClockInit );
