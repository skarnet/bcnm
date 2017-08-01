/* ISC license. */

#include <string.h>
#include <skalibs/stralloc.h>
#include <bcnm/wpactrl.h>

static inline size_t wpactrl_findmsg (wpactrl_t *a, char const *filter)
{
  size_t filterlen = strlen(filter) ;
  size_t i = 0 ;
  while (i < a->data.len)
  {
    if (!strncmp(a->data.s + i, filter, filterlen)) break ;
    i += strlen(a->data.s + i) + 1 ;
  }
  return i ;
}

int wpactrl_xchg_event (wpactrl_t *a, wpactrl_xchg_t *dt, tain_t *stamp)
{
  size_t pos, len ;
  if (dt->i >= dt->n) return 2 ;
  pos = wpactrl_findmsg(a, dt->tab[dt->i].filter) ;
  if (pos >= a->data.len) return 0 ;
  dt->sa.len = 0 ;
  len = strlen(a->data.s + pos) + 1 ;
  if (dt->i == dt->n - 1)
  {
    if (!stralloc_catb(&dt->sa, a->data.s + pos, len)) return -1 ;
    memmove(a->data.s + pos, a->data.s + pos + len, a->data.len - len) ;
    a->data.len -= len ;
    dt->status = 0 ;
    wpactrl_filter_remove(a, dt->tab[dt->i].filter) ;
    return 1 ;
  }
  if (!(*dt->tab[dt->i].f)(a, a->data.s + pos, len - 1, dt->aux, stamp)) return -1 ;
  memmove(a->data.s + pos, a->data.s + pos + len, a->data.len - len) ;
  a->data.len -= len ;
  wpactrl_filter_remove(a, dt->tab[dt->i].filter) ;
  if (!wpactrl_filter_add(a, dt->tab[++dt->i].filter)) return -1 ;
  return 0 ;
}
