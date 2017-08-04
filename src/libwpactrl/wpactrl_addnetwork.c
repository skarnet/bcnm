/* ISC license. */

#include <string.h>
#include <errno.h>
#include <skalibs/uint32.h>
#include <skalibs/error.h>
#include <bcnm/wpactrl.h>

int wpactrl_addnetwork (wpactrl_t *a, uint32_t *id, tain_t *stamp)
{
  char buf[UINT32_FMT] ;
  if (wpactrl_query(a, "ADD_NETWORK", buf, UINT32_FMT, stamp) < 0) return 0 ;
  if (uint32_scan(buf, id)) return 1 ;
  return (errno = EPROTO, 0) ;
}
