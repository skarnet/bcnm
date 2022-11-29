/* ISC license. */

#include <string.h>
#include <net/if.h>

#include <skalibs/strerr.h>
#include <skalibs/sgetopt.h>
#include <skalibs/types.h>
#include <skalibs/tai.h>
#include <skalibs/iopause.h>

#include <bcnm/if.h>

#define USAGE "bcnm-waitif [ -u | -d ] [ -t timeout ] [ -k kbufsize ] mask interface..."
#define dieusage() strerr_dieusage(100, USAGE)

static inline int check (bcnm_if_link_state_t *tab, size_t n, unsigned int mask, int not)
{
  size_t i = 0 ;
  for (; i < n ; i++)
    if ((tab[i].state & mask) != (not ? 0 : mask)) return 0 ;
  return 1 ;
}

int main (int argc, char const *const *argv)
{
  iopause_fd x = { .events = IOPAUSE_READ } ;
  tain deadline, tto ;
  int not = 0 ;
  unsigned int mask = 1 ;
  unsigned int kbufsize = 131072 ;
  PROG = "bcnm-waitif" ;
  {
    subgetopt l = SUBGETOPT_ZERO ;
    unsigned int t = 0 ;
    for (;;)
    {
      int opt = subgetopt_r(argc, argv, "udt:k:", &l) ;
      if (opt == -1) break ;
      switch (opt)
      {
        case 'u' : not = 0 ; break ;
        case 'd' : not = 1 ; break ;
        case 't' : if (!uint0_scan(l.arg, &t)) dieusage() ; break ;
        case 'k' : if (!uint0_scan(l.arg, &kbufsize)) dieusage() ; break ;
        default : dieusage() ;
      }
    }
    argc -= l.ind ; argv += l.ind ;
    if (t) tain_from_millisecs(&tto, t) ;
    else tto = tain_infinite_relative ;
  }
  if (argc < 2) dieusage() ;
  if (!uint0_scan(*argv++, &mask))
    strerr_dief2x(100, "invalid mask: ", argv[-1]) ;

  x.fd = bcnm_if_link_init(kbufsize) ;
  if (x.fd < 0) strerr_diefu1sys(111, "bcnm_if_link_init") ;

  tain_now_set_stopwatch_g() ;
  tain_add_g(&deadline, &tto) ;

  {
    size_t n = --argc ;
    bcnm_if_link_state_t tab[n] ;
    for (size_t i = 0 ; i < n ; i++)
    {
      int r ;
      size_t len = strlen(argv[i]) ;
      if (len >= IF_NAMESIZE - 1)
        strerr_dief2x(100, "interface name too long: ", argv[i]) ;
      memcpy(tab[i].ifname, argv[i], len+1) ;
      tab[i].changed = 0 ;
      r = bcnm_if_getstate(argv[i]) ;
      if (r < 0) strerr_diefu2sys(111, "get state for interface ", argv[i]) ;
      tab[i].state = r ;
    }

    for (;;)
    {
      int r ;
      if (check(tab, n, mask, not)) return 0 ;
      r = iopause_g(&x, 1, &deadline) ;
      if (r < 0) strerr_diefu1sys(111, "iopause") ;
      if (!r) strerr_dief1x(99, "timed out") ;
      if (bcnm_if_link_event(x.fd, tab, n) < 0)
        strerr_diefu1sys(111, "read rtnetlink event") ;
    }
  }
  return 0 ;
}
