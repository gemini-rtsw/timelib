/*+
 *   Function name:
 *   timeOffline
 *
 *   Purpose:
 *   Simulate Gemini TCS time service
 *
 *   Description:
 *   Simulate the Gemini TCS time service in order to be able to run
 *   the time library functions offline.
 *
 *   Invocation:
 *   status = timeOffline (tai, etlong, phi, hm, dleap, dat, dut)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>) tai      (double)    TAI (MJD)
 *      (>) elong    (double)    east longitude (true, radians)
 *      (>) phi      (double)    latitude (true geodetic, radians)
 *      (>) hm       (double)    height above reference spheroid (metres)
 *      (>) dleap    (double)    MJD following the latest known leap second
 *      (>) dat      (double)    TAI-UTC before that date (seconds)
 *      (>) dut      (double)    UT1-UTC before that date (seconds)
 *
 *   Function value:
 *   (<)  status  (int) Return status, 0 = OK
 *-
 */

/***************************************************************/
/*                                                             */
/*  Please note that there are two different versions of this  */
/*  function, for online use (under VxWorks) and offline use   */
/*  (under Unix) respectively.  The alternative sets of code   */
/*  are selected through the vxWorks environment variable.     */
/*                                                             */
/***************************************************************/

#include <slalib.h>
#include "timesys.h"

#if defined (vxWorks) || defined (__rtems__)

/*
**  --------------
**  ONLINE VERSION
**  --------------
*/

int timeOffline ( double tai,
                  double elong, double phi, double hm,
                  double dleap, double dat, double dut )
/*
**  - - - - - - - - - - - -
**   t i m e O f f l i n e
**  - - - - - - - - - - - -
**
**  Simulate the Gemini TCS time service in order to be able to run
**  the time library functions offline.
**
**  THIS FUNCTION IS NOT INTENDED FOR ONLINE USE.  THE PRESENT
**  VERSION ALWAYS RETURNS AND ERROR STATUS.
**
**  Given (arguments) but not used in this version:
**     tai      double    TAI (MJD)
**     elong    double    east longitude (true, radians)
**     phi      double    latitude (true geodetic, radians)
**     hm       double    height above reference spheroid (metres)
**     dleap    double    MJD following the latest known leap second
**     dat      double    TAI-UTC before that date (seconds)
**     dut      double    UT1-UTC before that date (seconds)
**
**  Returned (function value):
**              int       status:  0 = OK
**
**  Note:  This version of the function exists only to support the
**  case where an attempt is made to use the routine online.  An
**  error status is returned.
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Last revision:   18 March 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/
{
   return -1;
}

#else

/*
**  ---------------
**  OFFLINE VERSION
**  ---------------
*/

int timeOffline ( double tai,
                  double elong, double phi, double hm,
                  double dleap, double dat, double dut )
/*
**  - - - - - - - - - - - -
**   t i m e O f f l i n e
**  - - - - - - - - - - - -
**
**  Simulate the Gemini TCS time service in order to be able to run
**  the time library functions offline.
**
**  Given (arguments):
**     tai      double    TAI (MJD)
**     elong    double    east longitude (true, radians)
**     phi      double    latitude (true geodetic, radians)
**     hm       double    height above reference spheroid (metres)
**     dleap    double    MJD following the latest known leap second
**     dat      double    TAI-UTC before that date (seconds)
**     dut      double    UT1-UTC before that date (seconds)
**
**  Returned (global data):
**     initd    int       "initialized" flag:  set true
**     biass    double    offset for testing (sec):  zeroed
**     dttd     double    TT-TAI (days)
**     djmls    double    MJD following the next leap second
**     datlsd   double    TAI-UTC before that date (days)
**     dutd     double    UT1-UTC before that date (days)
**     deltdbd  double    current TDB-TT (days)
**     delstr   double    current LAST-GMST (radians)
**
**  Returned (function value):
**              int       status:  0 = OK
**
**  Called:  slaGeoc, slaRcc, slaDranrm
**
**  Notes:
**
**  1)  This version of the function may only be used offline.
**
**  2)  No validation of the input arguments is performed.  It is up
**      to the application to provide sensible values.
**
**  3)  The offset-for-testing is set to zero.  A non-zero value can
**      be applied by calling the timeOffset routine.
**
**  4)  The function is capable of simulating to very high accuracy
**      the time context of the online system for the given set of
**      circumstances.  However, for many purposes, a reduced level of
**      accuracy is acceptable, and in such cases various liberties
**      can be taken with the input arguments, as follows:
**
**      tai        This is used only to calculate TDB-TT and the
**                 equation of the equinoxes.  Depending on the
**                 application, errors of several days may well be
**                 harmless!  Using UTC instead of TAI will never
**                 make a practical difference.
**
**      elong      The longitude enters into the prediction of
**                 sidereal time and hence telescope pointing.  The
**                 mean longitude (i.e. neglecting polar motion) is
**                 acceptable for most purposes, producing only a
**                 sub-arcsecond effect on pointing.
**
**      phi        The latitude affects only the TDB-TT calculation.
**                 Mean latitude (i.e. neglecting polar motion) is always
*                  acceptable.
**
**      hm         The observatory height microscopically affects
**                 TDB-TT.  Zero can always be used without significant
*                  loss of accuracy.
**
**      dleap      Use either zero or a large number in order to apply
**                 the given dat and dut.
**
**      dat        At around 30 seconds currently, this is a fairly
**                 important argument for most applications.
**
**      dut        For many purposes, zero will do.  Errors here affect
**                 pointing calculations;  using zero produces up to
**                 about 13 arcseconds of error.
**
**  Reference:  Gemini TCS/PTW/6.
**
**  Last revision:   22 April 1997
**
**  Copyright 1997 RAL.  All rights reserved.
*/
#define AUKM  1.4959787066e8   /* AU to km */
{
   double tt, r, z;


/* Offset (zeroed). */
   biass = 0.0;

/* TT-TAI (fixed). */
   dttd = 32.184 / 86400.0;          /* ttmtai from TCS */

/* MJD following latest known UTC leap second (given). */
   djmls = dleap <= 0.0 ? 1e10 : dleap;

/* TAI-UTC before latest known UTC leap second (given). */
   datlsd = dat / 86400.0;

/* UT1-UTC before latest known UTC leap second (given). */
   dutd = dut / 86400.0;

/* Current TDB-TT (computed from TAI and site location). */
   tt = tai + dttd;
   slaGeoc ( phi, hm, &r, &z );
   deltdbd = slaRcc ( tt, fmod ( tai - dat + dut, 1.0 ), -elong,
                      r * AUKM, z * AUKM ) / 86400.0;

/* LAST minus GMST. */
   delstr = slaDranrm ( elong + slaEqeqx ( tt + deltdbd ) );

/* Set the "initialized" flag. */
   initd = 1;

/* Return the status. */
   return 0;
}

#endif
