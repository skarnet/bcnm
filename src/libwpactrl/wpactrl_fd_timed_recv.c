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

ssize_t wpactrl_fd_timed_recv (int fd, char *s, size_t len, tain const *deadline, tain *stamp)
{
  struct blah_s blah = { .fd = fd, .s = s, .len = len } ;
  return timed_get(&blah, (init_func_ref)&getfd, (get_func_ref)&get, deadline, stamp) ;
}
