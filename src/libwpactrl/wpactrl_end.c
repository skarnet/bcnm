/* ISC license. */

#include <skalibs/posixplz.h>
#include <skalibs/stralloc.h>
#include <skalibs/djbunix.h>

#include <bcnm/wpactrl.h>

void wpactrl_end (wpactrl_t *a)
{
  a->clientpath.s[a->clientpath.len - 2] = 's' ;
  unlink_void(a->clientpath.s) ;
  a->clientpath.s[a->clientpath.len - 2] = 'a' ;
  unlink_void(a->clientpath.s) ;
  fd_close(a->fda) ;
  fd_close(a->fds) ;
  stralloc_free(&a->filters) ;
  stralloc_free(&a->data) ;
  stralloc_free(&a->clientpath) ;
  *a = wpactrl_zero ;
}
