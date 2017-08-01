/* ISC license. */

#include <skalibs/stralloc.h>
#include <bcnm/wpactrl.h>

void wpactrl_xchg_free (wpactrl_xchg_t *dt)
{
  stralloc_free(&dt->sa) ;
  *dt = wpactrl_xchg_zero ;
}
