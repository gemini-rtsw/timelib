


/* Global variables that had been previously declared in "timesys.h" by redefining the keyword "extern" away.*/

/**    Summary of globals in timesys.h 
**     initd    int       "initialized" flag:  set true
**     biass    double    offset for testing (sec):  zeroed
**     dttd     double    TT-TAI (days)
**     djmls    double    MJD following the next leap second
**     datlsd   double    TAI-UTC before that date (days)
**     dutd     double    UT1-UTC before that date (days)
**     deltdbd  double    current TDB-TT (days)
**     delstr   double    current LAST-GMST (radians)
**/

int initd;
double biass;
double datlsd;
double dttd;
double djmls;
double dutd;
double deltdbd;
double delstr;
double elongt;

int tsim;

/* end of global variables */

