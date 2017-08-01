/* ISC license. */

#include <string.h>
#include <errno.h>
#include <skalibs/stralloc.h>
#include <bcnm/wpactrl.h>
#include "wpactrl-internal.h"

int wpactrl_filter_add (wpactrl_t *a, char const *s)
{
  if (wpactrl_filter_exact_search(a, s) < a->filters.len) return (errno = EBUSY, 0) ;
  return stralloc_catb(&a->filters, s, strlen(s)) ;
}
