/* ISC license. */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <bcnm/if.h>

int bcnm_if_link_init (unsigned int kbufsz)
{
  struct sockaddr_nl nl =
  {
    .nl_family = AF_NETLINK,
    .nl_pad = 0,
    .nl_groups = RTNLGRP_LINK,
    .nl_pid = 0
  } ;
  int fd = socket(AF_NETLINK, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, NETLINK_ROUTE) ;
  if (fd < 0) return -1 ;
  if (bind(fd, (struct sockaddr *)&nl, sizeof(struct sockaddr_nl)) < 0) goto err ;
  if (setsockopt(fd, SOL_SOCKET, SO_RCVBUFFORCE, &kbufsz, sizeof(unsigned int)) < 0)
  {
    if (errno != EPERM
     || setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &kbufsz, sizeof(unsigned int)) < 0) goto err ;
  }
  return fd ;

 err:
  close(fd) ;
  return -1 ;
}

