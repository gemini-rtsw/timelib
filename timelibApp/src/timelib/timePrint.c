#include <stdio.h>
#include <string.h>
#include "timesys.h"
#include "timeLib.h"

#include <registryFunction.h>
#include <epicsExport.h>

/*+
 *   Function name:
 *   timePrint
 *
 *   Purpose:
 *   Prints the current time (according to timelib) on standard output
 *
 *   Description:
 *   This is a purely diagnostic function used to print out the current
 *   time in order to check that an IOC has the correct time.
 *
 *   Invocation:
 *   timePrint( scale )
 *
 *   Parameters: (">" input, "!" modified, "<" output)
 *      (>)  scale (char *) "UTC","UT1","TT","TDB","GMST","LAST" 
 *
 *-
 */


int timePrint ( char *scale ) 

{
   timescale ts;
   int status;
   int ymdhmsf[7];
   char *month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

   if ( strcmp( scale, "TAI" ) == 0 ) {
      ts = TAI;
   } else if ( strcmp( scale, "UTC" ) == 0 ) {
      ts = UTC;
   } else if ( strcmp( scale, "UT1" ) == 0 ) {
      ts = UT1;
   } else if ( strcmp( scale, "TT" ) == 0 ) {
      ts = TT;
   } else if ( strcmp( scale, "TDB" ) == 0 ) {
      ts = TDB;
   } else if ( strcmp( scale, "GMST" ) == 0 ) {
      ts = GMST;
   } else if ( strcmp( scale, "LAST" ) == 0 ) {
      ts = LAST;
   } else {
      printf( "\"%s\" is not a supported timescale\n", scale );
      return -1;
   }
   if ( (ts != GMST) & (ts != LAST) ) {
      status = timeNowC( ts, 4, ymdhmsf);
      if ( status ) {
         printf( "error code %d from timelib\n", status);
         return status;
      }
      printf( "%d %s %d %02d:%02d:%02d.%04d %s\n", 
         ymdhmsf[0], month[ymdhmsf[1]-1], ymdhmsf[2], ymdhmsf[3], ymdhmsf[4], 
         ymdhmsf[5], ymdhmsf[6], scale);
   } else {
      status = timeNowT( ts, 4, &ymdhmsf[3]);
      if ( status ) {
         printf( "error code %d from timelib\n", status);
         return status;
      }
      printf( "%02d:%02d:%02d.%04d %s\n", 
         ymdhmsf[3], ymdhmsf[4], ymdhmsf[5], ymdhmsf[6], scale);
   }
   return 0;
}

/* Register these symbols for use by IOC code */

epicsRegisterFunction( timePrint );
