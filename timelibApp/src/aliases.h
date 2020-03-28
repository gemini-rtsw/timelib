#ifndef BANCOMM_WEAK_ALIASES
#define BANCOMM_WEAK_ALIASES

void __attribute__((weak)) BCconfigure(const int, const int, const int, const int,
				       const int);
int __attribute__((weak))  bc635_read(double *);
int __attribute__((weak))  bcSendTfp(char *);
int __attribute__((weak))  bcTestCard( void );
int __attribute__((weak))  bcGetGpsLeap( void );
int __attribute__((weak))  clock_rate_set( int );
int __attribute__((weak))  bc635IntEnable(const unsigned short, const char *);
int __attribute__((weak))  bc635_write(const unsigned short, const double);
void __attribute__((weak)) bcIntConnect(void (*isrproc)(const int n));
void __attribute__((weak)) bcIntDisconnect(void);
void __attribute__((weak)) bcSetRTC(void);
int __attribute__((weak))  bcSetEpoch (const int);
int __attribute__((weak))  bcRegsToTime (double *, unsigned char *);
long __attribute__((weak)) bc635_report (int);
long __attribute__((weak)) bc635_init (void);

#endif
