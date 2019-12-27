#include <slalib.h>
#include <slamac.h>
#include "timesys.h"

/*+
 *   Function name:
 *   timeSetDefaults
 *
 *   Purpose:
 *   Set defaults in the time library private memory
 *
 *   Description:
 *   Simply copy data passed in as parameters into local memory after 
 *   performing any unit conversions.
 *
 *   Invocation:
 *   timeSetDefaults(tlongm, tlatm, xpm, ypm, ttmtai, mjdls, delat, delut,
 *                   deltdb, bias)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)   tlongm   (double)  Site mean longitude (degrees)
 *      (>)   tlatm    (double)  Site mena latitude (degrees)
 *      (>)   xpm      (double)  X component of polar motion (arcsec)
 *      (>)   ypm      (double)  Y component of polar motion (arcsec)
 *      (>)   ttmtai   (double)  TT - TAI  (secs)
 *      (>)   mjdls    (double)  MJD following next leap second
 *      (>)   delat    (double)  TAI - UTC before that date (secs) 
 *      (>)   delut    (double)  UT1 - UTC before that date (secs)
 *      (>)   deltdb   (double)  current TDB - TT (days)
 *      (>)   bias     (double)  offset for testing (days)
 *
 *   External functions:
 *   slaPolmo   (slalib)  Compute components of polar motion
 *
 *-
 */

void timeSetDefaults (double tlongm, double tlatm, double xpm, double ypm,
                      double ttmtai, double mjdls, double delat, 
                      double delut, double deltdb, double bias)

{

  double phit ;             /* latitude corrected for polar motion */
  double daz ;              /* Azimuth correction due to polar motion */

  xpm   = xpm * DAS2R ;
  ypm   = ypm * DAS2R ;
  tlongm = tlongm * DD2R ;
  tlatm  = tlatm * DD2R ;

  slaPolmo ( tlongm, tlatm, xpm, ypm, &elongt, &phit, &daz ) ;

  dttd = ttmtai / 86400. ;
  datlsd = delat / 86400. ;
  dutd = delut / 86400. ;
  biass = bias * 86400. ;
  deltdbd = deltdb ;
  djmls = mjdls ;

#if defined(DEBUG)
  printf ("TIME DEFAULTS\n") ;
  printf ("xpm %-+25.15g\n", xpm);
  printf ("ypm %-+25.15g\n", ypm);
  printf ("tlongm %-+25.15g\n", tlongm);
  printf ("tlatm %-+25.15g\n", tlatm);
  printf ("elongt %-+25.15g\n", elongt);
  printf ("dttd %-+25.15g\n", dttd);
  printf ("datlsd %-+25.15g\n", datlsd);
  printf ("dutd %-+25.15g\n", dutd);
  printf ("biass %-+25.15g\n", biass);
  printf ("deltdbd %-+25.15g\n", deltdbd);
  printf ("djmls %-+25.15g\n", djmls);
#endif
}
