/* ISC license. */

#include <string.h>
#include <errno.h>

#include <skalibs/posixishard.h>
#include <skalibs/error.h>
#include <skalibs/allreadwrite.h>
#include <skalibs/stralloc.h>

#include <bcnm/wpactrl.h>
#include "wpactrl-internal.h"

static inline int validate (char const *s, size_t len)
{
  if (len < 4) return 0 ;
  if (s[0] != '<') return 0 ;
  if (!memchr("123456789", s[1], 9)) return 0 ;
  if (s[2] != '>') return 0 ;
  if (strnlen(s, len) < len) return 0 ;
  return 1 ;
}

int wpactrl_update (wpactrl_t *a)
{
  unsigned int n = WPACTRL_RECV_MAX ;
  unsigned int count = 0 ;
  char buf[WPACTRL_PACKET_MAX+1] ;
  while (n--)
  {
    ssize_t r = sanitize_read(wpactrl_fd_recv(a->fda, buf, WPACTRL_PACKET_MAX)) ;
    if (r < 0) return -1 ;
    if (!r) break ;
    if (a->options & WPACTRL_OPTION_NOFILTER
     || (validate(buf, r) && wpactrl_filter_match(a, buf, r)))
    {
      buf[r] = 0 ;
      if (!stralloc_catb(&a->data, buf, r+1)) return -1 ;
      count++ ;
    }
  }
  return (int)count ;
}
