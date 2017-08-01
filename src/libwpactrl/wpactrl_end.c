/* ISC license. */

#include <skalibs/stralloc.h>
#include <skalibs/djbunix.h>
#include <bcnm/wpactrl.h>

void wpactrl_end (wpactrl_t *a)
{
  fd_close(a->fda) ;
  fd_close(a->fds) ;
  stralloc_free(&a->filters) ;
  stralloc_free(&a->data) ;
  *a = wpactrl_zero ;
}
