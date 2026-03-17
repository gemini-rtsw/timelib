# Leap Seconds



-   [Introduction](#LeapSeconds-Introduction)
-   [Changes to the Software](#LeapSeconds-ChangestotheSoftware)
    -   [TCS](#LeapSeconds-TCS)
    -   [Timelib](#LeapSeconds-Timelib)
    -   [Slalib](#LeapSeconds-Slalib)
    -   [Flamingos 2](#LeapSeconds-Flamingos2)
    -   [GPI](#LeapSeconds-GPI)
        -   [GMP](#LeapSeconds-GMP)
        -   [Timelib / Slalib](#LeapSeconds-Timelib/Slalib)
            -   [EPICS](#LeapSeconds-EPICS)
            -   [Slalib](#LeapSeconds-Slalib.1)
            -   [Timelib](#LeapSeconds-Timelib.1)
        -   [TLC](#LeapSeconds-TLC)
-   [Emails](#LeapSeconds-Emails)
-   [References](#LeapSeconds-References)


# Introduction

A leap second is a one-second adjustment that is occasionally applied to
Coordinated Universal Time (UTC) in order to keep its time of day close
to the mean solar time. Insertion of each UTC leap second is usually
decided **about six months in advance** by the International Earth
Rotation and Reference Systems Service (IERS), when needed to ensure
that the difference between the UTC and UT1 readings will never exceed
0.9 second.

# Changes to the Software

There are changes that must be done between the time the leap second is
announced and before it is applied. The other set of changes should be
done and after the leap second is applied. It is not critical to do the
after-leap-second changes on July or January first, but it is important
that all the after-leap-second changes are done at the same time.

Adding (or subtracting) a leap second involves changes in the following
software components:

-   TCS

-   timelib

-   slalib

-   GPI

In the case of slalib and timelib, both the VxWorks, RTEMS and Linux
versions should be updated. However, the change on the telops
workstations is not critical and can wait until the next scheduled
software release.

All the examples in this page correspond to the leap second announced
for December 31st 2017. The values should be updated for the new leap
second.

## TCS

When the leap second is announced and **before** it's applied, change
the value of *djmls* in *GemNorth.default* and *GemSouth.default* to the
day the leap second will be applied:

In the following section:



```
#
# Quasi-fixed parameters (TCS/PTW/4.4)
#
double $(top)ak:delut = 0.0;           # UT1 - UTC (seconds) 
double $(top)ak:delat = 36.0;          # TAI - UTC (seconds)  <--
double $(top)ak:ttmtai = 32.184;       # TT - TAI (seconds)
double $(top)ak:xpm    = 0.0;          # X polar motion (arcsec) 
double $(top)ak:ypm   = 0.0;           # Y polar motion (arcsec) 
double $(top)ak:slow.A = 278.16;       # Initial air temperature (deg K)
double $(top)ak:slow.B = 615.0;        # Initial pressure (mbars)
double $(top)ak:slow.C =  0.2;         # Initial relative humidity
double $(top)ak:tempMax = 50.0;        # Max air temperature (deg C)
double $(top)ak:tempMin = -40.0;       # Min air temparature (deg C)
double $(top)ak:tempDefault =  5.0;    # Default temperature
double $(top)ak:pressMax = 1100.0;     # Max air pressure (millibars)
double $(top)ak:pressMin = 300.0;      # Min air pressure (millibars)
double $(top)ak:pressDefault =  615.0; # Default air pressure (millibars)
double $(top)ak:humidMax = 100.0;      # Max relative humidity (% RH)
double $(top)ak:humidMin = 0.0;        # Min  relative humidity (% RH)
double $(top)ak:humidDefault =  20.0;  # Default relative humidity (% RH)
double $(top)ak:tlr   =  0.0065;       # Tropospheric lapse rate (deg K / m)
double $(top)ak:wavel =  0.5;          # Default wavelength (microns)
double $(top)ak:djmls = 999999.9;      # MJD of next leap second   <--
double $(top)ak:biass = 0.0;           # Time offset for testing (seconds)

double $(top)ak:xim   =  0.0;          # X pointing origin (mm)
double $(top)ak:yim   =  0.0;          # Y pointing origin (mm)
```



The line containing the Modified Julian Date for the next leap second:



```
double $(top)ak:djmls = 999999.9;      # MJD of next leap second
```



should change to the MJD of the next leap second:



```
double $(top)ak:djmls = 57754.0;      # MJD of next leap second (2017/01/01)
```



Then any time **after** the leap second is applied, change the following
lines from:



```
double $(top)ak:delat = 36.0;          # TAI - UTC (seconds)
```





```
double $(top)ak:djmls = 57754.0;      # MJD of next leap second (2017/01/01)
```



to the following values, to be consistent with the new leap second
settings:



```
double $(top)ak:delat = 37.0;          # TAI - UTC (seconds)
```





```
double $(top)ak:djmls = 99999.0;      # MJD of next leap second
```



## Timelib

After the leap second is applied, add (or subtract) one second the
***delat*** value in *timeSeq.st*.  
For December 2016 the value changes from 36 to 37.



```
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
    delat  = 37.0 ;    <---    
    delut  = 0.338 ;    
    deltdb = 0.0 ;     
    biass = 0.0 ;    
```



## Slalib

After the leap second is announced, the function **slaDat** should be
updated to add a check for the new leap second so that if any systems
out there call slaDat they will get the correct leap second value.
Modify **dat.c** and add the corresponding line for the new leap second:



```
/* - - - - - - - - - - - - - - - - - - - - - */
/* Add new code here on each occasion that a */
/* leap second is announced, and also update */
/* the preamble comments appropriately.      */
/* - - - - - - - - - - - - - - - - - - - - - */

/* 2017 January 1 */
   if ( utc >= 57754.0 ) return 37.0;

/* 2015 July 1 */
   if ( utc >= 57204.0 ) return 36.0;

/* 2012 June 1 */
   if ( utc >= 56109.0 ) return 35.0;
```



## Flamingos 2

No changes are required as far as we can tell.

F2 has copies of *timelib* and *slalib* in the */gemini* folder on
*flam2sparc*, but these libraries were intended for the original VME
software that was later replaced by the portable channel access server
interface (PCASI), and for the OIWFS software used during the acceptance
tests.

## GPI

The GPI changes are no longer needed, but are documented here for
reference.

### GMP

The GMP needs to take into account the leap seconds when updating the
PCS.  
There is a single configuration file that needs to be updated, in the
case of GPI it is located at

/gemsoft/opt/gpi/gmp-server/conf/services/edu.gemini.aspen.gmp.pcs.model.PcsUpdaterComponent-default.cfg

And it contains an entry called:



```
# TAI difference
 taiDiff=37
```



After the file is changed it is recommended to restart the GMP

### Timelib / Slalib

*Timelib* and *slalib* should be compiled in the TLC to update the leap
second. Unfortunately, these two libraries are designed to compile with
EPICS installed on the same machine, which is not the case in the TLC.
The Linux rpm cannot be installed in this machine since it's an old
RedHat 5.3 machine. The following instructions describe the steps that
were used in 2016 to recompile timelib and slalib. All the instructions
in this section assume that the code was checked out and compiled in the
''$HOME/gemini'' directory.

#### EPICS

-   Check out the EPICS code from the repository



```
% cd $HOME/gemini
% svn co http://source.gemini.edu/software/epics-base/trunk/ epics
```



-   Compile the code

This will fail because of unresolved dependencies with packages that are
not available in the TLC, but it will get far enough for what we need.



```
% cd epics/base-3.14.12
% make
...
*** [src.install] Error 2
```



#### Slalib

Slalib should be compiled before timelib.

-   Checkout the code from the repository



```
% cd $HOME/gemini
% svn co http://source.gemini.edu/software/epics_module-slalib/trunk slalib
```



-   Edit configure/RELEASE to set EPICS_BASE



```
EPICS_BASE=/home/gpi/gemini/epics/base
```



-   Make sure the leap second is added in slalibApp/src/dat.c



```
/* 2017 January 1 */
   if ( utc >= 57754.0 ) return 37.0;
```



-   Compile the code



```
% cd $HOME/gemini/slalib
% make
```



-   Copy the library to the directory expected by the TLC



```
% cd $HOME/gemini/slalib/lib/linux-x86_64
% cp libsla.a <to be completed by Carlos>
```



#### Timelib

The leap second changes are located in the IOC specific code
([timeSeq.st](http://timeSeq.st)) that's not used in GPI, but the
library was compiled anyway.

-   Check out the code from the repository



```
% cd $HOME/gemini
% svn co http://source.gemini.edu/software/epics_module-timelib/trunk timelib
```



-   Edit configure/RELEASE to set EPICS_BASE



```
EPICS_BASE=/home/gpi/gemini/epics/base
```



-   Edit configure/RELEASE.gemini to set SLALIB



```
SLALIB=/home/gpi/gemini/slalib
```



-   Comment out IOC specific code timelibApp/src/timelib/Makefile



```
#LIB_SRCS_Linux += timePrint.c
#LIB_SRCS_solaris += timePrint.c
#LIB_LIBS += $(EPICS_BASE_IOC_LIBS)
#LIB_SRCS_DEFAULT += timeSeq.st
```



-   Compile the code



```
% cd $HOME/gemini/timelib/
% make
```



-   Copy the library to the directory expected by the TLC



```
% cd $HOME/gemini/timelib/lib/linux-x86_64/
% cp libtime.a <to be completed by Carlos>
```



### TLC

Additionally the TLC is compiled against timelib to calculate the WCS
values. These values go into the FITS headers.

The newly compiled libraries timelib and slalib should be copied to the
dir



```
$TLC_ROOT/external/gemini
```



Under the directories:



```
include/
 lib/
```



Only one process *gpCmdEvent* uses timelib so in principle it is only
necessary to compile and restart gpCmdEvent, as follows

As user *gpi*



```
 cd $TLC_ROOT/src
 make gpCmdEvent
 exit
```



As user *software*, restart the *gmp*



```
 /gemsoft/opt/gpi/gmp-server/bin/gmp-server-ctl.sh stop
 /gemsoft/opt/gpi/gmp-server/bin/gmp-server-ctl.sh start
```



# Emails

This section contains the email exchanges with Chris Mayer to get advice
on how to proceed with the leap second changes for the 2012 June 30th
leap second.

-   From Pedro Gigoux to Chris Mayer



```
Hola Chris,

As you probably know, the IERS announced a +1 leap second for June 30th.
The timelib source code has the following comments regarding how to cope with this change:

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

Could you confirm that this procedure is all we need to do in order to get things right?
My understanding is that only the TCS needs to be rebooted in order to change the leap second settings.
Can these changes be made on the fly or we need to compile timelib and reboot the TCS every time?
Is it too critical if we don't make the after-leap-second changes on July 1st?

Thank you,
```



-   From Chris Mayer to Pedro Gigoux



```
Hi Pedro,

  yes, this procedure is still correct and it is not time critical that you make the after-leap-second changes on
July 1st. However, I am a bit puzzled by comment 1 for after the leap second has occurred where it say sanity check
that dutd is +ve and decrement by 1s - the instruction to decrement by 1s seems wrong. The terms dutd and datlsd are
used in the combination dutd - datlsd. At the leap second both datlsd and dutd increment by 1s so the difference
stays the same.

  There may be a coordination issue depending on who generally updates the Gemxxxx.current files for the polar
motion and UT1-UTC (i.e.dutd). After the leap second has occurred the one thing to avoid is updating UT1-UTC without
simultaneously updating datlsd and djmls. So, if its an SSA job to update polar motion and UT1-UTC before observing
starts then they must also update Gemxxxxx.default with datlsd and djmls at the start of the shift after the leap
second.

  Currently it is still necessary to boot reboot the TCS to get the new data but then any system that is using
timeSeq.st should pick up the data from the TCS via channel access. I'll do a quick check tomorrow to make sure this
is still working - it is a long time since it has been tested.

  Longer term a couple of other changes can be made: slaDat should be updated to add the new leap second so that if
any systems out there call slaDat they will get the correct leap second value and timeSeq.st should be updated so that
the default value of leap seconds is set to 35.

  All the best

    Chris
```



-   From Chris Mayer to Pedro Gigoux



```
Hi Pedro,

  I did a check here using the TCS and A&G in simulation. I started the TCS with its default value
of djmls (999999.99) and the then checked what timeDump gave on both the TCS and A&G

> tc1>timeDump
> Dump of contents of time Library private memory
>
> Init flag = 1
> Offset bias (days)  = 0.000
> TT - TAI = 32.184
> MJD of next leap sec = 999999.900
> TAI - UTC before that date (secs) = 34.000
> UT1 - UTC before that date (secs) = 0.360
> Current TDB - TT (days) = 0.0000000029
> Current LAST - GMST (rads) = 3.5698076356

> ag->timeDump
> Dump of contents of time Library private memory
>
> Init flag = 1
> Offset bias (days)  = 0.000
> TT - TAI = 32.184
> MJD of next leap sec = 999999.900
> TAI - UTC before that date (secs) = 34.000
> UT1 - UTC before that date (secs) = 0.360
> Current TDB - TT (days) = 0.0000000029
> Current LAST - GMST (rads) = 3.5698076356

I then set the djmls to a time a few minutes in the future
(UTC 10 36 00) and rebooted the TCS.  I now got

> ag->timeDump
> Dump of contents of time Library private memory
>
> Init flag = 1
> Offset bias (days)  = 0.000
> TT - TAI = 32.184
> MJD of next leap sec = 56104.442
> TAI - UTC before that date (secs) = 34.000
> UT1 - UTC before that date (secs) = 0.360
> Current TDB - TT (days) = 0.0000000029
> Current LAST - GMST (rads) = 3.5698076349

 Showing that the A&G had picked up the new value without being rebooted.

 I also ran a log on the TCS to check what it reported for UTC
as the artificial leap second at 10 36 occurred

>              tc1:UTC 11:36:31.400762081   10:35:57.4
>              tc1:UTC 11:36:31.900896072   10:35:57.9
>              tc1:UTC 11:36:32.400773048   10:35:58.4
>              tc1:UTC 11:36:32.900892972   10:35:58.9
>              tc1:UTC 11:36:33.400774002   10:35:59.4
>              tc1:UTC 11:36:33.900907039   10:35:59.9
>              tc1:UTC 11:36:34.400785923   10:35:59.4
>              tc1:UTC 11:36:34.900975942   10:35:59.9
>              tc1:UTC 11:36:35.400784969   10:36:00.4
>              tc1:UTC 11:36:35.900922060   10:36:00.9
>              tc1:UTC 11:36:36.400795936   10:36:01.4
>              tc1:UTC 11:36:36.900921106   10:36:01.9
>

The epics timestamps (TAI) are smooth and increment normally but you can see that the UTC as
calculated by timelib steps back by exactly 1 s at 10 36 so that that second is repeated and
UTC is given correctly

    All the best

      Chris
```



# References

-   [Wikipedia leap second
    page](https://en.wikipedia.org/wiki/Leap_second)

-   [IERS Bulletin C: leap second
    announcements](https://datacenter.iers.org/data/latestVersion/bulletinC.txt)

