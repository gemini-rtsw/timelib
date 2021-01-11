TOP = ../..

include $(TOP)/configure/CONFIG

# Build the time library

LIBRARY += timelib

DBD += timelib.dbd
DBD += timelibSupp.dbd

timelibSupp_DBD += timelibreg.dbd

timelib_DBD += $(timelibSupp_DBD)
timelib_DBD += base.dbd

# For the build of the libraries
timelib_SRCS += timeCheck.c
timelib_SRCS += timeClockCheck.c
timelib_SRCS += timeClockInit.c
timelib_SRCS += timeClockReport.c
timelib_SRCS += timeDump.c
timelib_SRCS += timeGetMasterIOC.c
timelib_SRCS += timeGetSimFlag.c
timelib_SRCS += timeInit.c
timelib_SRCS += timeNow.c
timelib_SRCS += timeNowC.c
timelib_SRCS += timeNowD.c
timelib_SRCS += timeNowR.c
timelib_SRCS += timeNowT.c
timelib_SRCS += timeOffline.c
timelib_SRCS += timeOffset.c
timelib_SRCS += timePrint.c
timelib_SRCS += timeRaw2tai.c
timelib_SRCS += timeSetDefaults.c
timelib_SRCS += timeSetMasterIOC.c
timelib_SRCS += timeSetSimFlag.c
timelib_SRCS += timeLibRefresh.c
timelib_SRCS += timeTai2raw.c
timelib_SRCS += timeThenC.c
timelib_SRCS += timeThenD.c
timelib_SRCS += timeThenR.c
timelib_SRCS += timeThenT.c
timelib_SRCS += timesys.c
timelib_SRCS += bancomm_weak_aliases.c

LIB_LIBS += slalib $(EPICS_BASE_IOC_LIBS)

timeSeq_SNCFLAGS += +r +d
timelib_SRCS += timeSeq.st
timeSeq_LIBS += seq pv

INC += timeLib.h
INC += timesys.h
INC += bc635Aliases.h

include $(TOP)/configure/RULES
