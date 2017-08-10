/* ISC license. */

#include <string.h>
#include <bcnm/wpactrl.h>

int wpactrl_filter_match (wpactrl_t const *a, char const *s, size_t len)
{
  size_t filterlen = a->filters.len ;
  char const *filters = a->filters.s ;
  if (len < 4) return 0 ;
  s += 3 ; len -= 3 ;
  while (filterlen)
  {
    size_t flen = strlen(filters) ;
    if (len >= flen && !strncmp(filters, s, flen)) return 1 ;
    filters += flen+1 ;
    filterlen -= flen+1 ;
  }
  return 0 ;
}
