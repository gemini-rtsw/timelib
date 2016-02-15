#ifndef TIMELIBHDEF
#define TIMELIBHDEF

/*
**  - - - - - - - - - -
**   t i m e L i b . h
**  - - - - - - - - - -
**
**  Macros and prototype function declarations for timelib library.
**
**  Last revision:   20 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/
/* *INDENT-OFF* */
/*
 * $Log: timeLib.h,v $
 * Revision 1.1.1.1  2005/06/14 14:43:29  ajf
 * Initial creation of the Gemini TIMELIB 3-14 repository
 *
 * Revision 1.1.1.1  2005/06/13 12:16:50  ajf
 * Initial creation of the Gemini TIMELIB 3-14 repository
 *
 * Revision 1.1.1.1  2005/05/25 12:35:03  ajf
 * Initial creation of the Gemini timelib3-14 repository
 *
 * Revision 1.1  2002/01/09 16:53:44  ajf
 * Rearrangement of timelib source directories for inclusion of timeProbe.
 *
 * Revision 1.1.1.1  1998/11/07 23:08:13  epics
 * Version 1-2 of timelib as transferred from DRAL
 *
 * Revision 1.5  1998/10/20 07:51:07  dlt
 * Recompute eqn of equinoxes when deltdb changes
 *
 * Revision 1.4  1998/06/30 14:01:21  dlt
 * Change timescale argument to char
 *
 * Revision 1.3  1997/03/24 09:36:39  tcs
 * Enable use of time library on Unix
 *
 * Revision 1.2  1996/06/19 08:33:54  cjm
 * First release version
 *
 * Revision 1.1  1996/05/13 08:45:33  tcs
 * *** empty log message ***
 *
 */
/* *INDENT-ON* */


   typedef enum {
                  TAI,      /* International Atomic Time    */
                  UTC,      /* Coordinated Universal Time   */
                  UT1,      /* Universal Time               */
                  TT,       /* Terrestrial Time             */
                  TDB,      /* Barycentric Dynamical Time   */
                  GMST,     /* Greenwich Mean Sidereal Time */
                  LAST      /* Local Apparent Sidereal Time */
                } timescale;

int timeInit ( void );
int timeNow ( double* );
int timeNowC ( timescale, int, int* );
int timeNowD ( timescale, double* );
int timeNowR ( timescale, double* );
int timeNowT ( timescale, int, int* );
int timeOffset ( double );
double timeRaw2tai ( double );
double timeTai2raw ( double );
int timeThenC ( double, timescale, int, int* );
int timeThenD ( double, timescale, double* );
int timeThenR ( double, timescale, double* );
int timeThenT ( double, timescale, int, int* );
int timeOffline ( double, double, double, double, double, double, double );
int timePrint( char* );

#if defined (vxWorks)
void timeClockCheck ( void );
int timeClockInit (int master, int simulate, int intPerSecond,
                   int intPerTick, int timeOffset );
void timeClockReport ( void );
void timeSetSimFlag ( int simulate );
void timeGetSimFlag ( int *simulate );
void timeSetMasterIOC ( int master );
void timeGetMasterIOC ( int *master );
void timeDump( void );
void timeSetDefaults ( double tlongm, double tlatm, double xpmr, double ypmr,
                       double ttmtai, double mjdls, double delat,
                       double delut, double deltdb, double bias );
int  timeCheck( double *timeDiff );
void timeLibRefresh (double deltdb);
#endif

#endif
