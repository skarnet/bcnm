/* ISC license. */

#include <errno.h>
#include <skalibs/stralloc.h>
#include <bcnm/wpactrl.h>
#include "wpactrl-internal.h"

int wpactrl_querysa (wpactrl_t *a, char const *s, size_t len, stralloc *sa, tain_t *stamp)
{
  char buf[WPACTRL_PACKET_MAX] ;
  ssize_t r = wpactrl_query(a, s, len, buf, WPACTRL_PACKET_MAX, stamp) ;
  if (r < 0) return 0 ;
  if (!r) return (errno = EPIPE, 0) ;
  return stralloc_catb(sa, buf, r) ;
}
