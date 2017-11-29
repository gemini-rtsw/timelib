#include "timesys.h"

/*+
 *   Function name:
 *   timeSetSimFlag
 *
 *   Purpose:
 *   Set the time simulation flag
 *
 *   Description:
 *   This routine sets the simulation flag in the time library 
 *   global memory. The flag says whether the subsystem is 
 *   being simulated internally  by the TCS.
 *
 *   Invocation:
 *   timeSetSimFlag (simulate)
 *
 *   Parameters: (">" input, "<" output, "!" modified)  
 *    (>)  simulate  (int)     simulate flag
 *
 *   External variables:
 *    (>)   tsim  (int)   Time simulation flag
 *-
 */

void timeSetSimFlag ( int simulate)
{
  tsim = simulate ;
}
