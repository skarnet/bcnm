/* ISC license. */

#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <skalibs/uint32.h>
#include <skalibs/bytestr.h>
#include <skalibs/error.h>
#include <skalibs/fmtscan.h>
#include <skalibs/stralloc.h>
#include <skalibs/genalloc.h>

#include <bcnm/wpactrl.h>

static int wpactrl_networks_parse_one (char const *s, size_t len, wpactrl_networks_t *thing, stralloc *sa)
{
  wpactrl_networks_t sr ;
  size_t pos = byte_chr(s, len, '\t') ;
  if (pos >= len) goto eproto ;
  if (uint32_scan(s, &sr.id) != pos) goto eproto ;
  s += pos + 1 ; len -= pos + 1 ;

  pos = byte_rchr(s, len, '\t') ;
  if (!pos || pos >= len) goto eproto ;
  sr.flags_start = sa->len ;
  if (wpactrl_flags_scan(s + pos + 1, sa) != len - pos - 1) goto eproto ;
  sr.flags_len = sa->len - sr.flags_start ;
  len = pos ;

  pos = byte_rchr(s, len - 1, '\t') ;
  if (pos >= len - 1) goto eproto ;
  if (wpactrl_bssid_scan(s + pos + 1, sr.bssid) != len - pos - 1) goto eproto ;
  len = pos ;
  if (!len) len = 1 ;
  sr.ssid_start = sa->len ;
  sr.ssid_len = len - 1 ;
  if (!stralloc_catb(sa, s, len - 1) || !stralloc_0(sa)) return 0 ;
  *thing = sr ;
  return 1 ;

 eproto:
  return (errno = EPROTO, 0) ;
}

int wpactrl_networks_parse (char const *s, size_t len, genalloc *ga, stralloc *sa)
{
  int sawasnull = !sa->s ;
  int gawasnull = !genalloc_s(wpactrl_networks_t, ga) ;
  size_t sabase = sa->len ;
  size_t gabase = genalloc_len(wpactrl_networks_t, ga) ;
  size_t start = byte_chr(s, len, '\n') ;
  if (start++ >= len) return (errno = EPROTO, 0) ;
  while (start < len)
  {
    size_t pos = byte_chr(s + start, len - start, '\n') ;
    wpactrl_networks_t thing ;
    if (!wpactrl_networks_parse_one(s + start, pos, &thing, sa)) goto err ;
    if (!genalloc_append(wpactrl_networks_t, ga, &thing)) goto err ;
    start += pos + 1 ;
  }
  return 1 ;

 err:
  if (gawasnull) genalloc_free(wpactrl_networks_t, ga) ;
  else genalloc_setlen(wpactrl_networks_t, ga, gabase) ;
  if (sawasnull) stralloc_free(sa) ;
  else sa->len = sabase ;
  return 0 ;
}
