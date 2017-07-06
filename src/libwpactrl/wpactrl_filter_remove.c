/* ISC license. */

#include <string.h>
#include <bcnm/wpactrl.h>
#include "wpactrl-internal.h"

void wpactrl_filter_remove (wpactrl_t *a, char const *s)
{
  size_t pos = wpactrl_filter_exact_search(a, s) ;
  if (pos >= a->filters.len)
  {
    size_t after = pos + strlen(a->filters.s + pos) + 1 ;
    memmove(a->filters.s + pos, a->filters.s + after, a->filters.len - after) ;
    a->filters.len -= after - pos ;
  }
}
