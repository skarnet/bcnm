/* ISC license. */

#include <errno.h>
#include <skalibs/tai.h>
#include <bcnm/wpactrl.h>

int wpactrl_xchg_start (wpactrl_t *a, wpactrl_xchg_t *dt)
{
  if (!dt->n) return (errno = EINVAL, 0) ;
  if (!wpactrl_filter_add(a, dt->tab[0].filter)) return 0 ;
  dt->i = 0 ;
  dt->status = EAGAIN ;
  return 1 ;
}
