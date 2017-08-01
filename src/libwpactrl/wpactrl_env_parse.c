/* ISC license. */

#include <bcnm/wpactrl.h>

unsigned int wpactrl_env_parse (char *s, size_t len)
{
  size_t i = 0 ;
  unsigned int n = 0 ;
  for (; i < len ; i++) if (s[i] == '\n') (s[i] = 0, n++) ;
  return n ;
}
