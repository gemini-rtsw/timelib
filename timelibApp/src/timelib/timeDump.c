#include <vxWorks.h>
#include <drvTS.h>
#include <stdio.h>
#include "timesys.h"

#include <registryFunction.h>
#include <epicsExport.h>

/*+
 *   Function name:
 *   timeDump
 *
 *   Purpose:
 *   Outputs time library internal variables to screen
 *
 *   Description:
 *   This is a purely diagnostic function used to print out the contents
 *   the the time library's internal variables to the screen.
 *
 *   Invocation:
 *   timeDump()
 *
 *-
 */


void timeDump (void ) 

{

 printf ("Dump of contents of time Library private memory \n\n") ;
 printf ("Init flag = %d\n", initd ) ;
 printf ("Offset bias (days)  = %.3f \n", biass/86400.) ;
 printf ("TT - TAI = %.3f \n", dttd * 86400) ;
 printf ("MJD of next leap sec = %.3f\n", djmls) ;
 printf ("TAI - UTC before that date (secs) = %.3f\n", datlsd * 86400.0) ;
 printf ("UT1 - UTC before that date (secs) = %.3f\n", dutd * 86400.0) ;
 printf ("Current TDB - TT (days) = %.10f\n", deltdbd) ;
 printf ("Current LAST - GMST (rads) = %.10f\n", delstr) ;

}

/* Register these symbols for use by IOC code */

epicsRegisterFunction( timeDump );
