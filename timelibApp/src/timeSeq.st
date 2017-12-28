/*
*   FILENAME
*   timeSeq.st
*
*   FUNCTION NAME(S)
*   timeSeq - the sequence program that controls the monitoring of time
*
*/
/* *INDENT-OFF* */
/*
 * $Log: timeSeq.st,v $
 * Revision 1.4  2012/06/25 21:12:21  gemvx
 * Changed delat to 35 to take care if the June 2012 leap second
 *
 * Revision 1.3  2008/12/31 16:16:30  gemvx
 * Changed delat to 34 to account for a new leap second
 *
 * Revision 1.2  2006/01/03 11:29:13  cjm
 * Change default leap seconds to 33
 *
 * Revision 1.1  2002/01/09 16:53:45  ajf
 * Rearrangement of timelib source directories for inclusion of timeProbe.
 *
 * Revision 1.6  2000/03/21 21:45:07  cjm
 * Write to IMSS field of the time health record
 *
 * Revision 1.5  2000/01/25 01:21:52  cjm
 * Monitor time health during state waitConnect
 *
 * Revision 1.4  1999/05/30 10:42:38  dlt
 * Check tcs time variables even when simulating time
 *
 * Revision 1.3  1999/01/17 01:27:24  cjm
 * Change delat from 31 to 32 for leap second at end of 1998
 *
 * Revision 1.2  1998/11/20 02:56:49  dlt
 * Monitor changes in polar motion etc.
 *
 * Revision 1.1.1.1  1998/11/07 23:08:14  epics
 * Version 1-2 of timelib as transferred from DRAL
 *
 * Revision 1.9  1998/10/20 07:51:09  dlt
 * Recompute eqn of equinoxes when deltdb changes
 *
 * Revision 1.8  1998/06/29 20:11:57  tcs
 * Increment default number of leap seconds to 31
 *
 * Revision 1.7  1998/01/20 14:19:26  tcs
 * Correct health in simulation mode
 *
 * Revision 1.6  1998/01/20 08:52:25  tcs
 * Make initial state BAD and supress log message when unchanged
 *
 * Revision 1.5  1997/12/03 10:12:52  pbt
 * Correct variable names xpmr/ypmr to xpm/ypm
 *
 * Revision 1.4  1997/07/09 13:59:09  pbt
 * Health status now a string
 *
 * Revision 1.3  1997/06/02 15:54:30  cjm
 * Support new health values i.e. 0, 100, 200
 *
 * Revision 1.2  1996/06/19 08:34:01  cjm
 * First release version
 *
 * Revision 1.1  1996/05/13 08:45:38  tcs
 * *** empty log message ***
 *
 */
/* *INDENT-ON* */

/*+
 * Module:
 * timeSeq.st
 *
 * Purpose:
 * Monitor time system on behalf of application
 *
 * Description:
 * This sequence checks and monitors the time system. It first verifies
 * that the parameters used to boot the time system are consistent with
 * the hardware that is available. It then checks that the TCS is in a
 * state where it is ready to provide time initialisation parameters
 * If it is, it will initialise the time system. It then enters a state 
 * where it monitors the time system and reports any errors or significant
 * events.
 *
 * The sequence should be started with the command
 *   seq &timeSeq "sys=<system prefix>"
 *
 * where <system prefix> is the standard Gemini prefix for the system
 * e.g. on the Mount Control System the command would be
 *   seq &timeSeq "sys=MC:"
 *
 * whereas on the TCS it would be
 *   seq &timeSeq "sys=TCS:"
 *
 * The sequence requires that the EPICS system running the code provides
 * a health record for the time system as well as a log record. These are 
 * currently a mbbi and stringin record respectively and have the names
 * {sys}TIME:health and {sys}logrecord.
 *
 * For test purposes it may be neccessary to over ride the default locations
 * of the TCS process variables. e.g. if all the TCS records were in a
 * database whose prefix was tcs4 then the sequence should be started
 * as follows on the TCS
 *
 *   seq &timeSeq "sys=tcs4:, top=tcs4:"
 *
 * and as follows on the mount
 *
 *   seq &timeSeq "sys=MC:, top=tcs4:"
 *
 *-
 */

program timeSeq ("top = tcs:")    

option -c ;                   /* allow start prior to all connections */

/* put includes here */

%%#include "timeLib.h"
%%#include <epicsString.h>

/* Put defines here */

%{
/* #define NULL 0 */
#define BOOTING 0
#define INITIALISING 1
#define RUNNING 2
#define GOODHEALTH 0
#define WARNHEALTH 100
#define BADHEALTH 200
}%

/* SNC assignments */

int tcsState ; assign tcsState to "{top}state" ;
monitor tcsState ;
double tcsBias ; assign tcsBias to "{top}ak:biass" ;
monitor tcsBias ;
double tcsDeltdb ; assign tcsDeltdb to "{top}ak:deltdb" ;
monitor tcsDeltdb ;
double tcsXpm ;   assign tcsXpm to "{top}ak:xpm" ;
monitor tcsXpm ;
double tcsYpm ;   assign tcsYpm to "{top}ak:ypm" ;
monitor tcsYpm ;
double tcsDjmls ;  assign tcsDjmls to "{top}ak:djmls" ;
monitor tcsDjmls ;
double tcsDelat ;  assign tcsDelat to "{top}ak:delat" ;
monitor tcsDelat ;
double tcsDelut ;  assign tcsDelut to "{top}ak:delut" ;
monitor tcsDelut ;

double tcsTlong ; assign tcsTlong to "{top}ak:tlongm" ;
double tcsTlat ;  assign tcsTlat to "{top}ak:tlatm" ;
double tcsTtmtai ; assign tcsTtmtai to "{top}ak:ttmtai" ;

string timeHealth ;    assign timeHealth to "{sys}TIME:health" ;
string timeMess ;  assign timeMess to "{sys}TIME:health.IMSS" ;
string tcsLog ;    assign tcsLog to "{sys}logrecord" ;
int timeSimFlag ;  assign timeSimFlag to "{sys}TIME:intSimulate" ;      


evflag changedBias ;         /* Event flag to detect changes in time offset */
evflag changedTdb ;          /* Event flag for changes in deltdb */
evflag changedConfig ;       /* Event flag for changes in time configuration */

sync tcsDeltdb changedTdb ;
sync tcsBias   changedBias ;
sync tcsXpm    changedConfig ;
sync tcsYpm    changedConfig ;
sync tcsDjmls  changedConfig ;
sync tcsDelat  changedConfig ;
sync tcsDelut  changedConfig ;

/* Local variables */

float loopTime ;              /* Interval between monitors */
int readstat ;                /* status from checking time */
int prevstat ;                /* status from previous check */
double BCmNTP ;               /* Bancomm - NTP (secs) */
%%char  *timemac;             /* Pointer to macros */

/* These variables are defaults for the time library global memory.
 * They should agree with the numbers in the relevant GemNorth.default
 * file.
 */

double tlong  ;          /* Site longitude (radians)*/
double tlat   ;          /* site latitude (radians) */
double xpm    ;          /* X component of polar motion (arcsec) */
double ypm    ;          /* Y component of polar motion (arcsec) */
double ttmtai ;          /* TT - TAI (seconds) */
double djmls  ;          /* MJD of next leap second */
double delat  ;          /* TAI - UTC before next leap second (seconds) */
double delut  ;          /* UT1 - UTC (seconds) */
double deltdb ;          /* delta TDB (days) */
double biass ;           /* time offset (days) */

/* Code for state set timeMon */

ss timeMon
{

  state init
  {
   when()
   {
/* The purpose of this code is to initialise the time library with 
 * reasonable values as soon as possible. Once the TCS is up and running
 * these initial values will get updated with more accurate numbers. In
 * the meantime, the system running this code will have access to all
 * possible time scales albeit with somewhat reduced accuracy
 */
    tlong = -155.471667 ;
    tlat  = 19.826667 ;
    xpm    = 0.05 ;     
    ypm    = 0.4 ;     
    ttmtai = 32.184 ; 
    djmls  = 999999.9 ;    
    delat  = 35.0 ;      
    delut  = 0.338 ;    
    deltdb = 0.0 ;     
    biass = 0.0 ;     
    timeClockCheck() ;
    timeSetDefaults (tlong, tlat, xpm, ypm, ttmtai, djmls, delat,
                     delut, deltdb, biass) ;
    timeInit() ;
    timeGetSimFlag(&timeSimFlag) ;
    loopTime = 1.0 ;
    strcpy(timeMess, " ") ;
    strcpy(timeHealth, "GOOD") ;
    pvPut(timeMess) ;
    pvPut(timeHealth) ;
    pvPut(timeSimFlag) ;
    timemac = macValueGet("sys") ;
    if (timemac == NULL )
      printf ("timeSeq: Sequence started without assigning sys macro\n") ;
   } state connecting
  }

  state connecting
  {
   when (timeSimFlag == TRUE)
   {
     strcpy(timeMess, " ") ;
     strcpy(timeHealth, "GOOD") ;
     pvPut(timeMess) ;
     pvPut(timeHealth) ;
   } state simulating

   when (pvConnectCount() == pvChannelCount())
   {
   } state checkState

   when (pvConnectCount() < pvChannelCount())
   {
    printf ("timeSeq: Awaiting channel connections\n") ;
   }  state waitConnect
  }

  state checkState
  {
   when (tcsState == RUNNING)
   {
/* At this point, the TCS should be fully initialised and have valid data
 * in all its records. So grab all the latest data and update the time
 * library global memory
 */
   pvGet(tcsTlong) ;
   pvGet(tcsTlat) ;
   pvGet(tcsXpm) ;
   pvGet(tcsYpm) ;
   pvGet(tcsTtmtai);
   pvGet(tcsDjmls) ;
   pvGet(tcsDelat) ;
   pvGet(tcsDelut) ;
   timeSetDefaults(tcsTlong, tcsTlat, tcsXpm, tcsYpm, tcsTtmtai,
                   tcsDjmls, tcsDelat, tcsDelut, tcsDeltdb, tcsBias) ;
   timeInit() ;
   } state monitoring
  }

  state waitConnect
  {
   when (pvConnectCount() == pvChannelCount())
   {
   } state checkState

   when (delay(loopTime))
   {
/* This code periodically reads the time from the Bancomm, and sets the 
 * time system health on the basis of the error returned
 */
    readstat = timeCheck(&BCmNTP) & 0x05; /* ignore offset >5 us */
    if ((BCmNTP > 2.0 || BCmNTP < -2.0) && readstat)
    {
       if (strcmp(timeHealth,"BAD") || prevstat != readstat)
       {
           strcpy(timeMess, "Bancomm time out by > 2s");
           strcpy(timeHealth, "BAD") ;
           prevstat = readstat ;
           pvPut(timeMess);
           pvPut(timeHealth) ;
           sprintf(tcsLog, "Time: Error, status = %d", readstat) ;
           pvPut(tcsLog) ;
       }
    }
    else if ((BCmNTP < 2.0 && BCmNTP > -2.0) && readstat)
    {
       if (strcmp(timeHealth,"WARNING") || prevstat != readstat)
       {
           sprintf(timeMess, "Bancomm status %d", readstat) ;
           strcpy(timeHealth, "WARNING") ;
           prevstat = readstat ;
           pvPut(timeMess);
           pvPut(timeHealth) ;
           sprintf(tcsLog, "Time: Warning, status = %d", readstat) ;
           pvPut(tcsLog) ;
       }
    }
    else
    {
       if (strcmp(timeHealth,"WARNING") || prevstat != readstat)
       {
           strcpy(timeMess, "No TCS connection");
           strcpy(timeHealth, "WARNING") ;
           prevstat = readstat ;
           pvPut(timeMess);
           pvPut(timeHealth) ;
           sprintf (tcsLog, "Time: No TCS connection") ;
           pvPut(tcsLog) ;
       }
    }
   } state waitConnect
  }

  state monitoring
  {
   when (delay(loopTime))
   {
/* This code periodically reads the time from the Bancomm, and sets the 
 * time system health on the basis of the error returned
 */
    readstat = timeCheck(&BCmNTP) & 0x05; /* ignore offset >5 us */
    if ((BCmNTP > 2.0 || BCmNTP < -2.0) && readstat)
    {
       if (strcmp(timeHealth,"BAD") || prevstat != readstat)
       {
           strcpy(timeMess, "Bancomm time out by > 2s");
           strcpy(timeHealth, "BAD") ;
           prevstat = readstat ;
           pvPut(timeMess) ;
           pvPut(timeHealth) ;
           sprintf(tcsLog, "Time: Health bad, status = %d", readstat) ;
           pvPut(tcsLog) ;
       }
    }
    else if ((BCmNTP < 2.0 && BCmNTP > -2.0) && readstat)
    {
       if (strcmp(timeHealth,"WARNING") || prevstat != readstat)
       {
           sprintf(timeMess, "Bancomm status %d", readstat) ;
           strcpy(timeHealth, "WARNING") ;
           prevstat = readstat ;
           pvPut(timeMess) ;
           pvPut(timeHealth) ;
           sprintf(tcsLog, "Time: Health warning, status = %d", readstat) ;
           pvPut(tcsLog) ;
       }
    }
    else
    {
       if (strcmp(timeHealth,"GOOD") || prevstat != readstat)
       {
           strcpy(timeMess," ");
           strcpy(timeHealth, "GOOD") ;
           prevstat = readstat ;
           pvPut(timeMess) ;
           pvPut(timeHealth) ;
           sprintf (tcsLog, "Time: Health good") ;
           pvPut(tcsLog) ;
       }
    }

   } state monitoring

   when (pvConnectCount() < pvChannelCount())
   {
    tcsState = BOOTING ;
   }  state waitConnect

   when (efTestAndClear(changedBias))
   {
    timeOffset(tcsBias) ;
   } state monitoring

   when (efTestAndClear(changedTdb))
   {
    timeLibRefresh(tcsDeltdb) ;
   } state monitoring

   when (efTestAndClear(changedConfig))
   {
    pvGet(tcsTlong) ;
    pvGet(tcsTlat) ;
    pvGet(tcsTtmtai);
    timeSetDefaults(tcsTlong, tcsTlat, tcsXpm, tcsYpm, tcsTtmtai,
                    tcsDjmls, tcsDelat, tcsDelut, tcsDeltdb, tcsBias) ;
   } state monitoring

  }

/* 
 * This state monitors the tcs time variables but does not check the health
 * of the Bancomm card.
 */

  state simulating
  {
   when (delay(30.0))
   {
   }  state simulating

   when (efTestAndClear(changedBias))
   {
    timeOffset(tcsBias) ;
   } state simulating

   when (efTestAndClear(changedTdb))
   {
    timeLibRefresh(tcsDeltdb) ;
   } state simulating

   when (efTestAndClear(changedConfig))
   {
    pvGet(tcsTlong) ;
    pvGet(tcsTlat) ;
    pvGet(tcsTtmtai);
    timeSetDefaults(tcsTlong, tcsTlat, tcsXpm, tcsYpm, tcsTtmtai,
                    tcsDjmls, tcsDelat, tcsDelut, tcsDeltdb, tcsBias) ;
   } state simulating

  }
 
}

