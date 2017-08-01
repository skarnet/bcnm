/* ISC license. */

#include <skalibs/tai.h>
#include <bcnm/wpactrl.h>

void wpactrl_xchg_computedeadline (wpactrl_xchg_t const *dt, tain_t *deadline)
{
  if (tain_less(&dt->deadline, deadline)) *deadline = dt->deadline ;
}
