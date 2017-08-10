/* ISC license. */

#include <errno.h>
#include <bcnm/wpactrl.h>

void wpactrl_xchg_init (wpactrl_xchg_t *dt, wpactrl_xchgitem_t const *tab, unsigned int n, tain_t const *limit, void *aux)
{
  dt->tab = tab ;
  dt->n = n ;
  dt->i = 0 ;
  dt->deadline = *limit ;
  dt->status = ECONNABORTED ;
  dt->aux = aux ;
}
