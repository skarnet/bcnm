/* ISC license. */

#include <string.h>
#include <skalibs/uint32.h>
#include <bcnm/wpactrl.h>

wparesponse_t wpactrl_setnetworkoption (wpactrl_t *a, uint32_t id, char const *var, char const *val, tain *stamp)
{
  size_t varlen = strlen(var) ;
  size_t vallen = strlen(val) ;
  size_t idlen ;
  char buf[15 + UINT32_FMT + varlen + vallen] ;
  memcpy(buf, "SET_NETWORK ", 12) ;
  idlen = uint32_fmt(buf + 12, id) ;
  buf[12 + idlen] = ' ' ;
  memcpy(buf + 13 + idlen, var, varlen) ;
  buf[13 + idlen + varlen] = ' ' ;
  memcpy(buf + 14 + idlen + varlen, val, vallen) ;
  buf[14 + idlen + varlen + vallen] = 0 ;
  return wpactrl_command(a, buf, stamp) ;
}
