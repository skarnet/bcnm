/* ISC license. */

#include <skalibs/unix-timed.h>
#include <bcnm/wpactrl.h>
#include "wpactrl-internal.h"

ssize_t wpactrl_query (wpactrl_t *a, char const *q, size_t qlen, char *ans, size_t ansmax, tain_t *stamp)
{
  tain_t deadline ;
  tain_add(&deadline, stamp, &a->tto) ;
  if (!ipc_timed_send(a->fds, q, qlen, &deadline, stamp)) return WPA_ERROR ;
  return wpactrl_fd_timed_recv(a->fds, ans, ansmax, &deadline, stamp) ;
}
