/* ISC license. */

#include <bcnm/wpactrl.h>

char *wpactrl_msg (wpactrl_t *a)
{
  return a->datahead < a->data.len ? a->data.s + a->datahead : 0 ;
}
