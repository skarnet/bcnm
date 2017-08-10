/* ISC license. */

#include <skalibs/stralloc.h>
#include <skalibs/genalloc.h>
#include <bcnm/wpactrl.h>

void wpactrl_xchg_cbres_free (wpactrl_xchg_cbres_t *cr)
{
  genalloc_free(int, &cr->parsed) ; /* relies on genericity of genalloc_free */
  stralloc_free(&cr->storage) ;
}
