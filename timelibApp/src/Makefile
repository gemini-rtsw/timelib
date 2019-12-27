TOP = ../..

include $(TOP)/configure/CONFIG

# Build the time library

LIBRARY += timelib

# For the build of the libraries
timelib_SRCS += timeDump.c
timelib_SRCS += timeNow.c
timelib_SRCS += timeNowC.c
timelib_SRCS += timeNowD.c
timelib_SRCS += timeNowR.c
timelib_SRCS += timeNowT.c
timelib_SRCS += timeOffline.c
timelib_SRCS += timeOffset.c
timelib_SRCS += timeTai2raw.c
timelib_SRCS += timeThenC.c
timelib_SRCS += timeThenD.c
timelib_SRCS += timeThenR.c
timelib_SRCS += timeThenT.c
timelib_SRCS += timeRaw2tai.c
timelib_SRCS += timePrint.c
timelib_SRCS += timesys.c

LIB_LIBS += slalib $(EPICS_BASE_IOC_LIBS)


timeSeq_SNCFLAGS += +r +d
timelib_DBD += timeSeq.dbd
timelib_SRCS += timeSeq.st
timeSeq_LIBS += seq pv

INC += timeLib.h
INC += timesys.h

DBD += timelib.dbd

include $(TOP)/configure/RULES
