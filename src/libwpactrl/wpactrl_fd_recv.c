/* ISC license. */

#include <skalibs/sysdeps.h>
#include <skalibs/nonposix.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <errno.h>
#include <bcnm/wpactrl.h>
#include "wpactrl-internal.h"

ssize_t wpactrl_fd_recv (int fd, char *s, size_t len)
{
  static int const bsd_braindeadness_workaround_flags =
#ifdef SKALIBS_HASMSGDONTWAIT
    MSG_DONTWAIT
#else
    0
#endif
    |
#ifdef SKALIBS_HASCMSGCLOEXEC
    MSG_CMSG_CLOEXEC
#else
    0
#endif
    ;
  struct iovec iov = { .iov_base = s, .iov_len = len } ;
  struct msghdr msghdr =
  {
    .msg_name = 0,
    .msg_namelen = 0,
    .msg_iov = &iov,
    .msg_iovlen = 1,
    .msg_flags = 0,
    .msg_control = 0,
    .msg_controllen = 0
  } ;
  ssize_t r ;
  do r = recvmsg(fd, &msghdr, bsd_braindeadness_workaround_flags) ;
  while (r == -1 && errno == EINTR) ;
  return r > 0 && msghdr.msg_flags | MSG_TRUNC ? (errno = EMSGSIZE, -1) : r ;
}
