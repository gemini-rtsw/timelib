/****************************************************************************
 *
 * 2020-01-29 (rcardenes)
 *
 * Fake Bancomm support functions that behave like there's no installed
 * Bancomm card. The symbols would clash with those from the actual Bancomm
 * support module, and thus they're exported as *weak* symbols, which are
 * replaced by the actual ones *if* Bancomm support is included by the IOC.
 *
 * The registrars are needed so that we get some key symbols exported as DBD
 * and collected by the IOC. The IOC building process will produce functions
 * that link against those, "pulling" along the actual symbols that we wanted
 * to bring in.
 ****************************************************************************/
#include "aliases.h"
#include <iocsh.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define epicsExportSharedSymbols
#include <shareLib.h>

	typedef void (*REGISTRAR)(void);

#define EPICS_EXPORT_POBJ(typ,obj) pvar_ ## typ ## _ ## obj
#define EPICS_EXPORT_PFUNC(obj) pvar_func_ ## obj

#define epicsExportAddress(typ,obj) \
	epicsShareExtern typ *EPICS_EXPORT_POBJ(typ,obj) __attribute__((weak)); \
	epicsShareDef typ *EPICS_EXPORT_POBJ(typ,obj) __attribute__((weak)) = (typ *)(char *)&obj

#define epicsExportRegistrar(func) \
	epicsShareFunc REGISTRAR EPICS_EXPORT_PFUNC(func) __attribute__((weak)) = (REGISTRAR)(void*)&func

#define epicsRegisterFunction(func) \
	static void __attribute__(("weak")) register_func_ ## func(void) { \
		registryFunctionAdd(#func,(REGISTRYFUNCTION)func);} \
	epicsExportRegistrar(register_func_ ## func)

#ifdef __cplusplus
}
#endif

#ifndef OK
#define OK 0
#endif

#ifndef ERROR
#define ERROR (-1)
#endif

/*
 * These functions are provided as weak symbols for the case when the
 * Bancomm driver is not linked in (eg. building for Linux)
 *
 * The functions will do what's expected for an absent Bancomm (mostly
 * return error)
 */

void  BCconfigure
(
 const int MasterIOC,
 const int NoLeapSecs,
 const int intPerSecond,
 const int intPerTick,
 const int Offset
 ) {
	printf("FAKE BCconfigure\n");
}

void  bcIntConnect(void (*isrproc)(const int n)) {
}

void  bcIntDisconnect(void) {
}

void  bcSetRTC(void) {
}

int   bc635IntEnable(const unsigned short signal, const char *parm) {
	return ERROR;
}

int   bc635_read(double *prval) {
	return ERROR;
}

/***********************************************************************
 * bc635_write - Set time coincidence strobe
 *
 * For signal #3 and valid inputs, simulate success. Otherwise,
 * return error
 ***********************************************************************/
int   bc635_write(const unsigned short signal, const double value) {
	if ((signal == 3) && (value >= 0.0))
		return OK;
	return ERROR;
}


int bcSendTfp(char *charptr) {
	return ERROR;
}

int   bcSetEpoch (const int year) {
	return 0;
}

int   bcRegsToTime (double *prval, unsigned char *stime) {
	return ERROR;
}

long  bc635_report (int level) {
	return OK;
}

long  bc635_init (void) {
	return ERROR;
}

int bcTestCard( void ) {
	printf("FAKE bcTestCard: Returning ERROR\n");
	return ERROR;
}

/***********************************************************************
 * bcGetGpsLeap - Return the current value of GPS leap seconds.
 *
 * For bc635 cards (non-GPS) the result is always 0
 ***********************************************************************/
int   bcGetGpsLeap( void ) {
	return 0;
}

int clock_rate_set( int rate ) {
	return 0;
}

#include <stddef.h>


static int bcConfiguredOK = 0;        // Whether have a Bancomm at all
static int bcYearEpoch = 0;           // The epoch (wrt 1970) of Jan 1, Oh UTC of current year
static int bcDebug = 0;               // The epoch (wrt 1970) of Jan 1, Oh UTC of current year
static int bc_usec_ignore = 0;        // IGNORE MicroSeconds BUG JIRA TASK (RTUPG-521). Default DON'T Ignore!
static int bcIntCounter = 0;          // Count all interrupts
static int bcTickCnt = 0;             // Count all interrupts
static int altIntCounter1 = 0;        // Count alternate interrupts
static int altIntCounter2 = 0;        // Count alternate interrupts
static int bcReadCounter = 0;         // Count alternate interrupts
static int bcEventCounter = 0;        // Count alternate interrupts

// Register these symbols for use by IOC code
// Information needed by iocsh
static const iocshArg     bc635_reportArg0 = {"interest_level", iocshArgInt};
static const iocshArg    *bc635_reportArgs[] = { &bc635_reportArg0 };
static const iocshFuncDef bc635_reportFuncDef = {"bc635_report", 1, bc635_reportArgs};

// Wrapper called by iocsh, selects the argument types that bc635_report needs
static void bc635_reportCallFunc(const iocshArgBuf *args) {
    bc635_report(args[0].ival);
}

// Registration routine, runs at startup
static void bc635_reportRegister(void) {
    iocshRegister(&bc635_reportFuncDef, bc635_reportCallFunc);
}

// Register these symbols for use by IOC code
// Information needed by iocsh
static const iocshArg     BCconfigureArg0 = {"master", iocshArgInt};  // TRUE for Master IOC with bc637 GPS receiver
static const iocshArg     BCconfigureArg1 = {"useleap", iocshArgInt};  // FALSE = use UTC; TRUE = GPS time, no leap secs
static const iocshArg     BCconfigureArg2 = {"intPerSecond", iocshArgInt};  // Bancomm Periodic Frequency in Hz
static const iocshArg     BCconfigureArg3 = {"intPerTick", iocshArgInt};  // Number of periodic interrupts per VxWorks system clock tick
static const iocshArg     BCconfigureArg4 = {"Offset", iocshArgInt};  // Offset in microseconds relative to input reference, +ve = correction for delay

static const iocshArg    *BCconfigureArgs[] = {
	&BCconfigureArg0,
	&BCconfigureArg1,
	&BCconfigureArg2,
	&BCconfigureArg3,
	&BCconfigureArg4,
};

static const iocshFuncDef BCconfigureFuncDef = {"BCconfigure", 5, BCconfigureArgs};
// Wrapper called by iocsh, selects the argument types that bc635_report needs
static void BCconfigureCallFunc(const iocshArgBuf *args) {
    BCconfigure(args[0].ival, args[1].ival, args[2].ival, args[3].ival, args[4].ival );
}

// Registration routine, runs at startup
static void BCconfigureRegister(void) {
    iocshRegister(&BCconfigureFuncDef, BCconfigureCallFunc);
}

// Set up to  export the time provider priority set function to the IOC shell
static const iocshArg bc635TimeSetTpPrioArg0 = {"BC635 Time Provider Priority", iocshArgInt};
static const iocshArg * const bc635TimeSetTpPrioArgs[1] = { &bc635TimeSetTpPrioArg0 };
static const iocshFuncDef bc635TimeSetTpPrioFuncDef = {"bc635TimeSetTpPrio", 1, bc635TimeSetTpPrioArgs};
static void bc635TimeSetTpPrioCallFunc(const iocshArgBuf *args)
{
    printf("FAKE call to bc635TimeSetTpPrio\n");
}

// now register and export the shell functions
static void bc635TimeRegister(void) {
   iocshRegister(&bc635TimeSetTpPrioFuncDef, bc635TimeSetTpPrioCallFunc);
}

// Test Functions
static const iocshFuncDef bcSetRTCFuncDef ={"bcUseRTCMode", 0, NULL};
static void bcSetRTCCallFunc(const iocshArgBuf *args)
{
    //Call this on the shell before BCConfigure()
    printf("FAKE call to bcUseRTCMode\n");
}

static void BCTestFunctions(void) {
   iocshRegister(&bcSetRTCFuncDef, bcSetRTCCallFunc);
}

epicsExportRegistrar(bc635TimeRegister);
epicsExportRegistrar(bc635_reportRegister);
epicsExportRegistrar(BCconfigureRegister);
epicsExportRegistrar(BCTestFunctions);
epicsExportAddress(int, altIntCounter1);
epicsExportAddress(int, altIntCounter2);
epicsExportAddress(int, bcReadCounter);
epicsExportAddress(int, bcEventCounter);
epicsExportAddress(int, bcIntCounter);
epicsExportAddress(int, bcTickCnt);
epicsExportAddress(int, bcConfiguredOK );
epicsExportAddress(int, bcYearEpoch);
epicsExportAddress(int, bcDebug);
epicsExportAddress(int, bc_usec_ignore);
