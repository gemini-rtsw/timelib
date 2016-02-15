TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#=============================
# Build a support module called <APPNAME>

LIBRARY_IOC += timelib

# <APPNAME>.dbd will be created and installed
DBD += timelib.dbd

# timelib.dbd will be made up from these files:
timelib_DBD += base.dbd
# List local .dbd files here
# timelib_DBD += xxx.dbd

# Include dbd files for external support modules:
# timelib_DBD += genSubRecord.dbd

# specify all source files to be compiled
# including sequencer (.st) source files
#timelib_SRCS += xxx.c
#timelib_SRCS += xxx.st

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

