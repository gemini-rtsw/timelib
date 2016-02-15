TOP = ../../..

include $(TOP)/configure/CONFIG

# Build the time library

LIBRARY_Linux += timelib

LIB_LIBS += slalib

# For the build of the libraries
LIB_SRCS_Linux += timeInit.c
LIB_SRCS_Linux += timeNow.c
LIB_SRCS_Linux += timeNowC.c
LIB_SRCS_Linux += timeNowD.c
LIB_SRCS_Linux += timeNowR.c
LIB_SRCS_Linux += timeNowT.c
LIB_SRCS_Linux += timeOffline.c
LIB_SRCS_Linux += timeOffset.c
LIB_SRCS_Linux += timeTai2raw.c
LIB_SRCS_Linux += timeThenC.c
LIB_SRCS_Linux += timeThenD.c
LIB_SRCS_Linux += timeThenR.c
LIB_SRCS_Linux += timeThenT.c
LIB_SRCS_Linux += timeRaw2tai.c
LIB_SRCS_Linux += timePrint.c

LIBRARY_RTEMS += timelib

LIB_SRCS_RTEMS += timeOffline.c
LIB_SRCS_RTEMS += timeNow.c
LIB_SRCS_RTEMS += timeThenT.c
LIB_SRCS_RTEMS += timeThenD.c
LIB_SRCS_RTEMS += timeThenR.c
LIB_SRCS_RTEMS += timeRaw2tai.c
LIB_SRCS_RTEMS += timeInit.c

INC += timeLib.h
INC += timesys.h

DBD += timelib.dbd

include $(TOP)/configure/RULES
