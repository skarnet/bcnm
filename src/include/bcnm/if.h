/* ISC license. */

#ifndef BCNM_IF_H
#define BCNM_IF_H

#include <sys/types.h>
#include <net/if.h>

typedef struct bcnm_if_link_state_s bcnm_if_link_state_t, *bcnm_if_link_state_t_ref ;
struct bcnm_if_link_state_s
{
  char ifname[IF_NAMESIZE] ;
  unsigned int state : 15 ;
  unsigned int changed : 1 ;
} ;
#define BCNM_IF_LINK_STATE_ZERO { "", 0, 0 }

extern int bcnm_if_getstate (char const *) ;
extern int bcnm_if_link_init (unsigned int) ;
extern ssize_t bcnm_if_link_event (int, bcnm_if_link_state_t *, size_t) ;

#endif
