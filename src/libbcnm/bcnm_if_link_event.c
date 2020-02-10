/* ISC license. */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <string.h>
#include <errno.h>
#include <net/if.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <bcnm/if.h>

static inline ssize_t fd_recvmsg_nb (int fd, struct msghdr *hdr)
{
  int e = errno ;
  ssize_t r ;
  do r = recvmsg(fd, hdr, MSG_DONTWAIT) ;
  while (r == -1 && errno == EINTR) ;
  if (r == -1 && errno == EWOULDBLOCK) return (errno = e, 0) ;
  if (!r) return (errno = EPIPE, -1) ;
  return r ;
}

static int answer (bcnm_if_link_state_t *tab, size_t n, char const *ifname, unsigned int state)
{
  size_t i = 0 ;
  for (; i < n ; i++) if (!strcmp(ifname, tab[i].ifname)) break ;
  if (i >= n) return 0 ;
  tab[i].state = state ;
  tab[i].changed = 1 ;
  return 1 ;
}

ssize_t bcnm_if_link_event (int fd, bcnm_if_link_state_t *tab, size_t n)
{
  ssize_t e = 0 ;
  struct sockaddr_nl nl ;
  struct nlmsghdr buf[8192 / sizeof(struct nlmsghdr)] ;
  struct iovec v = { .iov_base = buf, .iov_len = sizeof(struct nlmsghdr) * (8192 / sizeof(struct nlmsghdr)) } ;
  struct msghdr msg =
  {
    .msg_name = &nl,
    .msg_namelen = sizeof(struct sockaddr_nl),
    .msg_iov = &v,
    .msg_iovlen = 1,
    .msg_control = 0,
    .msg_controllen = 0,
    .msg_flags = 0
  } ;
  ssize_t r = fd_recvmsg_nb(fd, &msg) ;
  if (r <= 0) return r ;
  if (msg.msg_flags & MSG_TRUNC) return (errno = ENOBUFS, -1) ;
  if (nl.nl_pid) return (errno = EPROTO, -1) ;
  for (struct nlmsghdr *hdr = buf ; NLMSG_OK(hdr, r) ; hdr = NLMSG_NEXT(hdr, r))
  {
    struct ifinfomsg *ifi = NLMSG_DATA(hdr) ;
    char ifname[IF_NAMESIZE] ;
    switch (hdr->nlmsg_type)
    {
      case NLMSG_DONE : goto out ;
      case NLMSG_ERROR : return (errno = EPROTO, -1) ;
      case RTM_DELLINK :
        if_indextoname(ifi->ifi_index, ifname) ;
        e += answer(tab, n, ifname, 0) ;
        break ;
      case RTM_NEWLINK :
        if_indextoname(ifi->ifi_index, ifname) ;
        e += answer(tab, n, ifname, 1 | !!(ifi->ifi_flags & IFF_UP) << 1 | !!(ifi->ifi_flags & IFF_RUNNING) << 2) ;
        break ;
    }
  }
 out:
  return e ;
}
