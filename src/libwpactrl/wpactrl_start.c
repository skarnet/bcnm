/* ISC license. */

#include <string.h>
#include <errno.h>
#include <skalibs/djbunix.h>
#include <skalibs/webipc.h>
#include <skalibs/unix-timed.h>
#include <bcnm/wpactrl.h>
#include "wpactrl-internal.h"

int wpactrl_start (wpactrl_t *a, char const *path, unsigned int timeout, tain_t *stamp)
{
  tain_t tto, deadline ;
  int fda, fds ;
  if (timeout) tain_from_millisecs(&tto, timeout) ;
  else tto = tain_infinite_relative ;
  fds = ipc_datagram_nbcoe() ;
  if (fds < 0) goto err ;
  tain_add(&deadline, &tto, stamp) ;
  if (!ipc_timed_connect(fds, path, &deadline, stamp)) goto errs ;
  fda = ipc_datagram_nbcoe() ;
  if (fda < 0) goto errs ;
  if (!ipc_timed_connect(fda, path, &deadline, stamp)) goto erra ;
  if (!ipc_timed_send(fda, "ATTACH", 6, &deadline, stamp)) goto erra ;
  {
    ssize_t r ;
    char answer[3] ;
    r = wpactrl_fd_timed_recv(fda, answer, 3, &deadline, stamp) ;
    if (r != 3 || memcmp(answer, "OK\n", 3)) goto erra ;
  }
  a->fds = fds ;
  a->fda = fda ;
  a->tto = tto ;
  return 1 ;

 erra:
  {
    int e = errno ;
    fd_close(fda) ;
    errno = e ;
  }
 errs:
  {
    int e = errno ;
    fd_close(fds) ;
    errno = e ;
  }
 err:
  return 0 ;
}
