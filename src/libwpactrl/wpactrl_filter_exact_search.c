/* ISC license. */

#include <string.h>
#include "wpactrl-internal.h"

size_t wpactrl_filter_exact_search (wpactrl_t const *a, char const *s)
{
  size_t pos = 0 ;
  for ( ; pos < a->filters.len ; pos += strlen(a->filters.s + pos) + 1)
    if (!strcmp(s, a->filters.s + pos)) break ;
  return pos ;
}
