TOP = ../../..

include $(TOP)/configure/CONFIG

# Build the time library
LIBRARY = timelib



timelib_LIBS_Linux += slalib
timelib_LIBS_DEFAULT += bancomm slalib seq pv

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

SRCS.c  = $(wildcard ../*.c)
timelib_SRCS_DEFAULT += $(strip $(patsubst ../%.c, %.o, $(SRCS.c)))
timelib_SRCS_DEFAULT += timeSeq.st

LIB_SRCS_RTEMS += timeCheck.c


INC += timeLib.h
INC += timesys.h

DBD += timelib.dbd

include $(TOP)/configure/RULES
