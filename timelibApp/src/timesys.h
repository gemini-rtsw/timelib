#ifndef TIMESYSIC
#define TIMESYSIC

/*
**  - - - - - - - - - -
**   t i m e s y s . h
**  - - - - - - - - - -
**
**  Time functions, macros and global data include file.
**
**  Last revision:   19 March 1997
**                   03 Jul 2019 mdw Added TS_EPICS_TO_UNIX_EPOCH
**
**  Copyright RAL.  All rights reserved.
*/

#include "timeLib.h"

/*
**  ---------
**  CONSTANTS
**  ---------
*/

/* MJD for raw time epoch zero: 1970 January 1.0 (= Unix zero time). */
#define TRAW_EPOCH 40587.0

/* TS_1900_TO_UNIX_TIME is 70 * no. of secs in year + 17 * no. of secs in day
*  (17 being the no. of leap years from 1900 to 1970) It is defined this
*  way to prevent warnings over integer overflows
*/
#define TS_1900_TO_UNIX_EPOCH 2208988800UL 

/* TS_EPICS_TO_UNIX_EPOCH is [20 years * 365 days/year + 5 days] * 86400 seconds/per day.
 *  [5 days is the number of leap years between 1 Jan 1970 (Unix Epoch) and 1 Jan 1990 
 *  (EPICS Epoch)
 */

#define TS_EPICS_TO_UNIX_EPOCH 631152000UL

/*
**  ----
**  TIME  -  latest time parameters
**  ----
*/

/* "Initialized" flag */
   extern int initd;

/* Offset to be added to clock reading for test purposes (sec) */
   extern double biass;

/* Quasi-perpetual TT-TAI (days) */
   extern double dttd;

/* MJD following the next UTC leap second */
   extern double djmls;

/* Current TAI-UTC (days) */
   extern double datlsd;

/* Current UT1-UTC (days) */
   extern double dutd;

/* n.b. Both the TAI-UTC and UT1-UTC are for BEFORE */
/* the specified leap second.  See the note, below. */

/* Current TDB-TT (days) */
   extern double deltdbd;

/* Current LAST-GMST (radians) */
   extern double delstr;




/*
** ------------
** LEAP SECONDS
** ------------
**
** Actions when a new leap second is announced:
**
**   1) Set djmls to the date of the next leap-second.  This will either
**      be a July 1 or a January 1.
**
**   2) Sanity-check that the datlsd currently in service is 1 second
**      less than the TAI-UTC which the announcement says will come
**      into force at the leap second.
**
** Actions after a leap second has occurred (say next day):
**
**   1) Sanity-check that dutd is +ve and decrement by 1 second.
**
**   2) Increment datlsd by 1 second and sanity-check that it is
**      now equal to the post-leap-second TAI-UTC given in the
**      leap second announcement.
**
**   3) Set djmls to an arbitrary large number (1e6 suggested).
*/

#endif
