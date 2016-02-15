#include <rpc/rpc.h>
#include "timeProbe.h"

int xdrTimeProbe( XDR *xdrsp, struct timeProbe *td )
{
  if( !xdr_double(xdrsp, &td->val) )
    return(0);

  if( !xdr_int(xdrsp, &td->reg) )
    return(0);

  if( !xdr_int(xdrsp, &td->cardStat) )
    return(0);

  if( !xdr_int(xdrsp, &td->timeStat) )
    return(0);

  return(1);
}
