#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#define PORTMAP
#include <rpc/rpc.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include "timeProbe.h"

#define NAME_SIZE   32
#define MAX_SYSTEMS 64
#define TIMEOUT_US  250000              /* Timeout in usec (0.25 seconds) */
#define CONFIG_FILE "/gemini/epics3.13.4/timelib/timelib/data/timeProbe.dat"


int main( int argc, char *argv[] )
{
  FILE             *fd;
  char             vmeSystems[MAX_SYSTEMS][NAME_SIZE];
  CLIENT           *client[MAX_SYSTEMS];
  enum clnt_stat   clientStatus[MAX_SYSTEMS];
  struct timeProbe td[MAX_SYSTEMS];
  struct timeval   connectTimeout;
  int              numSys;
  int              i;

  numSys = 0;
  fd     = fopen( CONFIG_FILE, "r");
  if( fd )
  {
    while( fgets( vmeSystems[numSys], NAME_SIZE, fd ) )
    {
      /* Get rid of the "\n" */
      vmeSystems[numSys][strlen(vmeSystems[numSys])-1] = '\0';
      numSys++;
      if( numSys == MAX_SYSTEMS )
      {
        printf("\nThere are too many systems specified in the configuration file\n");
        printf("The program has only read the first %d entries\n", numSys);
        break;
      }
    }
    fclose(fd);
  }
  else
  {
    printf("Cannot open \"%s\"\n", CONFIG_FILE);
    return(1);
  }

  connectTimeout.tv_sec  = 0;
  connectTimeout.tv_usec = TIMEOUT_US;

#ifdef linux
  for( i=0; i<numSys; i++ )
    client[i] = clnt_create( vmeSystems[i], TIME_PROG, TIME_VERS, "udp");
#else
  for( i=0; i<numSys; i++ )
    client[i] = clnt_create_timed( vmeSystems[i], TIME_PROG, TIME_VERS, "udp", &connectTimeout );
#endif

  for( i=0; i<numSys; i++ )
  {
    if( client[i] )
      clientStatus[i] = clnt_call(client[i], TIME_PROC_NUM, xdr_void, 0, xdrTimeProbe,
                                  (char *)&td[i], connectTimeout);
  }

  /* Now summarize results */

  printf("\n");
  printf("Connection | RPC Success | Bancomm Found | Time Read | Time Status | Time | Offset From TCS | System\n");
  printf("----------------------------------------------------------------------------------------------------\n");
  for( i=0; i<numSys; i++ )
  {
    if( client[i] )
    {
      printf("Yes");

      if( clientStatus[i] == RPC_SUCCESS )
      {
        printf("\t\tYes");
        if( !td[i].cardStat )
        {
          printf("\t\tYes");
          if( !td[i].timeStat )
          {
            printf("\t       Yes");
            printf("\t%d\t%f\t%f   %s\n", td[i].reg, td[i].val, td[i].val-td[0].val, vmeSystems[i]);
          }
          else
            printf("\t       No\tunknown\t   unknown\tunknown\t\t%s\n", vmeSystems[i]);
        }
        else
          printf("\t\tNo\t       No\tunknown\t   unknown\tunknown\t\t%s\n", vmeSystems[i]);
      }
      else
        printf("\t\tNo\t\tNo\t       No\tunknown\t   unknown\tunknown\t\t%s\n", vmeSystems[i]);
    }
    else
      printf("No\t\tNo\t\tNo\t       No\tunknown\t   unknown\tunknown\t\t%s\n", vmeSystems[i]);
  }
  printf("\n");
  printf("NOTE! TO BELIEVE THESE RESULTS, EVERY SYSTEM MUST HAVE BEEN BOOTED TO THE EPICS LEVEL AT LEAST ONCE\n");
  printf("SINCE THE LAST RESET OR POWER DOWN\n");

  for( i=0; i<numSys; i++ )
  {
    if( client[i] )
      clnt_destroy(client[i]);
  }

  return(0);
}
