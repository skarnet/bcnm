/* ISC license. */

#include <string.h>
#include <errno.h>
#include <skalibs/error.h>
#include <skalibs/stralloc.h>
#include <bcnm/wpactrl.h>

static inline size_t wpactrl_findmsg (wpactrl_t *a, char const *filter)
{
  size_t filterlen = strlen(filter) ;
  size_t i = 0 ;
  while (i < a->data.len)
  {
    if (!strncmp(a->data.s + i + 3, filter, filterlen)) break ;
    i += strlen(a->data.s + i) + 1 ;
  }
  return i ;
}

int wpactrl_xchg_event (wpactrl_t *a, wpactrl_xchg_t *dt, tain_t *stamp)
{
  size_t pos, len ;
  if (dt->i >= dt->n) return 2 ;
  if (!error_isagain(dt->status)) return (errno = EINVAL, -1) ;
  pos = wpactrl_findmsg(a, dt->tab[dt->i].filter) ;
  if (pos >= a->data.len) return 0 ;
  len = strlen(a->data.s + pos) + 1 ;
  if (!(*dt->tab[dt->i].cb)(a, a->data.s + pos, len - 1, dt->aux, stamp)) return -2 ;
  memmove(a->data.s + pos, a->data.s + pos + len, a->data.len - len) ;
  a->data.len -= len ;
  wpactrl_filter_remove(a, dt->tab[dt->i].filter) ;
  if (++dt->i == dt->n)
  {
    dt->status = 0 ;
    return 1 ;
  }
  if (!wpactrl_filter_add(a, dt->tab[dt->i].filter)) return -1 ;
  return 0 ;
}
