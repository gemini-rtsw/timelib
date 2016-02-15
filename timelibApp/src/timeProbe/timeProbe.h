/*
 *  Define the RPC Program number used to fetch the time
 *  from the Bancomm time registers.
 */


#define TIME_PROG     0x20000000
#define TIME_VERS     1
#define TIME_PROC_NUM 1

struct timeProbe
{
  double val;
  int    reg;
  int    cardStat;
  int    timeStat;
};

int xdrTimeProbe( XDR *, struct timeProbe * );
