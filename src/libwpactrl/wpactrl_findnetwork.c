/* ISC license. */

#include <skalibs/stralloc.h>
#include <skalibs/genalloc.h>
#include <bcnm/wpactrl.h>
#include "wpactrl-internal.h"

int wpactrl_findnetwork (wpactrl_t *a, char const *ssid, uint32_t *id, tain *stamp)
{
  char buf[WPACTRL_PACKET_MAX] ;
  stralloc sa = STRALLOC_ZERO ;
  genalloc ga = GENALLOC_ZERO ; /* wpactrl_networks_t */
  size_t i = 0 ;
  wpactrl_networks_t *p ;
  size_t n ;
  ssize_t r = wpactrl_query(a, "LIST_NETWORKS", buf, WPACTRL_PACKET_MAX, stamp) ;
  if (r < 0) return -1 ;
  if (!wpactrl_networks_parse(buf, r, &ga, &sa)) return -1 ;
  n = genalloc_len(wpactrl_networks_t, &ga) ;
  p = genalloc_s(wpactrl_networks_t, &ga) ;
  for (; i < n ; i++) if (!strcmp(ssid, sa.s + p[i].ssid_start)) break ;
  if (i < n) *id = p[i].id ;
  genalloc_free(wpactrl_networks_t, &ga) ;
  stralloc_free(&sa) ;
  return i < n ;
}
