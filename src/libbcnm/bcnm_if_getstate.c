/* ISC license. */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <bcnm/if.h>

int bcnm_if_getstate (char const *ifname)
{
  struct ifreq blah ;
  int sfd ;
  int e = errno ;
  size_t len = strlen(ifname) ;
  if (len >= IFNAMSIZ) return (errno = ENAMETOOLONG, -1) ;
  memcpy(blah.ifr_name, ifname, len+1) ;
  sfd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0) ;
  if (sfd < 0) return -1 ;
  if (ioctl(sfd, SIOCGIFFLAGS, &blah) < 0)
    return errno == ENODEV ? (errno = e, 0) : -1 ;
  close(sfd) ;
  return 1 | !!(blah.ifr_flags & IFF_UP) << 1 | !!(blah.ifr_flags & IFF_RUNNING) << 2 ;
}
