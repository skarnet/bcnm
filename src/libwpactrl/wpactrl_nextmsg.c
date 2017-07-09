/* ISC license. */

#include <string.h>
#include <bcnm/wpactrl.h>

void wpactrl_nextmsg (wpactrl_t *a)
{
  if (a->datahead < a->data.len) a->datahead += strlen(a->data.s + a->datahead) + 1 ;
  if (a->datahead >= a->data.len)
  {
    a->data.len = 0 ;
    a->datahead = 0 ;
  }
}
