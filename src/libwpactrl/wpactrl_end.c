/* ISC license. */

#include <skalibs/djbunix.h>
#include <bcnm/wpactrl.h>

void wpactrl_free (wpactrl_t *a)
{
  fd_close(a->fda) ;
  fd_close(a->fds) ;
  *a = wpactrl_zero ;
}
