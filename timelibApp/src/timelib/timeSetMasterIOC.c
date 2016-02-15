#include "timesys.h"

/*+
 *   Function name:
 *   timeSetMasterIOC
 *
 *   Purpose:
 *   Set flag to show if this is the timebus master
 *
 *   Description:
 *   Set the master IOC flag in the time library global memory 
 *
 *   Invocation:
 *   timeSetMasterIOC (master)
 *
 *   Parameters: (">" input, "!" modified, ">" output)  
 *    (>)  master  (int)    1 = masterIOC
 *
 *   External variables:
 *    (>) masterIOC (int)  Flag to indicate if this is a master IOC
 *-
 */

void timeSetMasterIOC ( int master) 
{

  masterIOC = master ;

}
