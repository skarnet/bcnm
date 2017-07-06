/* ISC license. */

#include <skalibs/functypes.h>
#include <skalibs/allreadwrite.h>
#include <skalibs/unix-timed.h>
#include "wpactrl-internal.h"

struct blah_s
{
  int fd ;
  char *s ;
  size_t len ;
} ;

static int getfd (struct blah_s *blah)
{
  return blah->fd ;
}

static ssize_t get (struct blah_s *blah)
{
  return sanitize_read(wpactrl_fd_recv(blah->fd, blah->s, blah->len)) ;
}

ssize_t wpactrl_fd_timed_recv (int fd, char *s, size_t len, tain_t const *deadline, tain_t *stamp)
{
  struct blah_s blah = { .fd = fd, .s = s, .len = len } ;
  return timed_get(&blah, (initfunc_t_ref)&getfd, (getfunc_t_ref)&get, deadline, stamp) ;
}
