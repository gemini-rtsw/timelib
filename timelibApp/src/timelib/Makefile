TOP = ../../..

include $(TOP)/configure/CONFIG

# Build the time library

LIBRARY += timelib

# For the build of the libraries
timelib_SRCS_Linux += timeInit.c
timelib_SRCS_Linux += timeNow.c
timelib_SRCS_Linux += timeNowC.c
timelib_SRCS_Linux += timeNowD.c
timelib_SRCS_Linux += timeNowR.c
timelib_SRCS_Linux += timeNowT.c
timelib_SRCS_Linux += timeOffline.c
timelib_SRCS_Linux += timeOffset.c
timelib_SRCS_Linux += timeTai2raw.c
timelib_SRCS_Linux += timeThenC.c
timelib_SRCS_Linux += timeThenD.c
timelib_SRCS_Linux += timeThenR.c
timelib_SRCS_Linux += timeThenT.c
timelib_SRCS_Linux += timeRaw2tai.c
timelib_SRCS_Linux += timePrint.c
timelib_SRCS_Linux += timesys.c

LIB_LIBS += slalib $(EPICS_BASE_IOC_LIBS)

SRCS.c  = $(wildcard ../*.c)
timelib_SRCS_DEFAULT += $(strip $(patsubst ../%.c, %.o, $(SRCS.c)))

timeSeq_SNCFLAGS += +r +d
timelib_DBD += timeSeq.dbd
timelib_SRCS += timeSeq.st
timeSeq_LIBS += seq pv

INC += timeLib.h
INC += timesys.h

DBD += timelib.dbd

include $(TOP)/configure/RULES
