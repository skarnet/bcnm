/* ISC license. */

#include <string.h>
#include <errno.h>

#include <skalibs/posixplz.h>
#include <skalibs/stralloc.h>
#include <skalibs/djbunix.h>
#include <skalibs/socket.h>
#include <skalibs/skamisc.h>
#include <skalibs/unix-timed.h>

#include <bcnm/config.h>
#include <bcnm/wpactrl.h>
#include "wpactrl-internal.h"

int wpactrl_start (wpactrl_t *a, char const *path, unsigned int timeout, tain_t *stamp)
{
  tain_t tto, deadline ;
  int fda, fds ;
  stralloc sa = STRALLOC_ZERO ;
  if (timeout) tain_from_millisecs(&tto, timeout) ;
  else tto = tain_infinite_relative ;
  if (!stralloc_cats(&sa, BCNM_TMPDIR)) return 0 ;
  if (!stralloc_cats(&sa, "/libwpactrl")
   || !sauniquename(&sa)
   || !stralloc_catb(&sa, ":s", 3)) goto err ;
  fds = ipc_datagram_nbcoe() ;
  if (fds < 0) goto err ;
  unlink_void(sa.s) ;
  tain_add(&deadline, &tto, stamp) ;
  if (ipc_bind(fds, sa.s) == -1
   || !ipc_timed_connect(fds, path, &deadline, stamp)) goto errs ;
  fda = ipc_datagram_nbcoe() ;
  if (fda < 0) goto errs ;
  sa.s[sa.len - 2] = 'a' ;
  unlink_void(sa.s) ;
  if (ipc_bind(fda, sa.s) == -1
   || !ipc_timed_connect(fda, path, &deadline, stamp)
   || !ipc_timed_send(fda, "ATTACH", 6, &deadline, stamp)) goto erra ;
  {
    ssize_t r ;
    char answer[3] ;
    r = wpactrl_fd_timed_recv(fda, answer, 3, &deadline, stamp) ;
    if (r != 3 || memcmp(answer, "OK\n", 3)) goto erra ;
  }
  if (!stralloc_shrink(&sa)) goto erra ;

  a->clientpath = sa ;
  a->fds = fds ;
  a->fda = fda ;
  a->tto = tto ;
  return 1 ;

 erra:
  fd_close(fda) ;
 errs:
  fd_close(fds) ;
 err:
  stralloc_free(&sa) ;
  return 0 ;
}
