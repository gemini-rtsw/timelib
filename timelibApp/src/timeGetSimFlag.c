#include "timesys.h"

/*+
 *   Function name:
 *   timeGetSimFlag
 *
 *   Purpose:
 *   Fetch time simulation flag 
 *
 *   Description:
 *   This routine fetches the simulation flag from the time library  
 *   global memory . The flag says whether the subsystem is 
 *   being simulated internally  by the TCS.
 *
 *   Invocation:
 *   timeGetSimFlag (&simulate)
 *
 *   Parameters: (">" input, "<" output, "!" modified)  
 *     (<) simulate  (int)     simulate flag
 *
 *   External variables:
 *     (>) tsim  (int)  simulation flag
 *
 *- 
 */


void timeGetSimFlag (int *simulate)
{
  *simulate = tsim;

}
