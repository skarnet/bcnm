/* ISC license. */

#include <skalibs/uint32.h>
#include <bcnm/wpactrl.h>

wparesponse_t wpactrl_selectnetwork (wpactrl_t *a, uint32_t id, tain *stamp)
{
  char buf[15 + UINT32_FMT] = "SELECT_NETWORK " ;
  buf[15 + uint32_fmt(buf + 15, id)] = 0 ;
  return wpactrl_command(a, buf, stamp) ;
}
