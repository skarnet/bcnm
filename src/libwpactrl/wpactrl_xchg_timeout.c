/* ISC license. */

#include <errno.h>
#include <skalibs/tai.h>
#include <bcnm/wpactrl.h>

int wpactrl_xchg_timeout (wpactrl_t *a, wpactrl_xchg_t *dt, tain_t const *stamp)
{
  if (!tain_less(stamp, &dt->deadline))
  {
    dt->status = ETIMEDOUT ;
    wpactrl_filter_remove(a, dt->tab[dt->i].filter) ;
    return 1 ;
  }
  else return 0 ;
}
