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

static int wpactrl_scan_parse_one (char const *s, size_t len, wpactrl_scanres_t *thing, stralloc *sa)
{
  wpactrl_scanres_t sr ;
  size_t pos = byte_chr(s, len, '\t') ;
  if (pos >= len) goto eproto ;
  if (wpactrl_bssid_scan(s, sr.bssid) != pos) goto eproto ;
  s += pos + 1 ; len -= pos + 1 ;
  pos = byte_chr(s, len, '\t') ;
  if (pos >= len) goto eproto ;
  if (uint32_scan(s, &sr.frequency) != pos) goto eproto ;
  s += pos + 1 ; len -= pos + 1 ;
  pos = byte_chr(s, len, '\t') ;
  if (pos >= len) goto eproto ;
  if (int32_scan(s, &sr.signal_level) != pos) goto eproto ;
  s += pos + 1 ; len -= pos + 1 ;
  pos = byte_chr(s, len, '\t') ;
  if (pos >= len) goto eproto ;
  sr.flags_start = sa->len ;
  if (wpactrl_flags_scan(s, sa) != pos) goto eproto ;
  s += pos + 1 ; len -= pos + 1 ;
  sr.flags_len = sa->len - sr.flags_start ;
  sr.ssid_start = sa->len ;
  sr.ssid_len = len ;
  if (!stralloc_catb(sa, s, len) || !stralloc_0(sa)) return 0 ;
  *thing = sr ;
  return 1 ;

 eproto:
  return (errno = EPROTO, 0) ;
}

int wpactrl_scan_parse (char const *s, size_t len, genalloc *ga, stralloc *sa)
{
  int sawasnull = !sa->s ;
  int gawasnull = !genalloc_s(wpactrl_scanres_t, ga) ;
  size_t sabase = sa->len ;
  size_t gabase = genalloc_len(wpactrl_scanres_t, ga) ;
  size_t start = byte_chr(s, len, '\n') ;
  if (start++ >= len) return (errno = EPROTO, 0) ;
  while (start < len)
  {
    size_t pos = byte_chr(s + start, len - start, '\n') ;
    wpactrl_scanres_t thing ;
    if (!wpactrl_scan_parse_one(s + start, pos, &thing, sa)) goto err ;
    if (!genalloc_append(wpactrl_scanres_t, ga, &thing)) goto err ;
    start += pos + 1 ;
  }
  return 1 ;

 err:
  if (gawasnull) genalloc_free(wpactrl_scanres_t, ga) ;
  else genalloc_setlen(wpactrl_scanres_t, ga, gabase) ;
  if (sawasnull) stralloc_free(sa) ;
  else sa->len = sabase ;
  return 0 ;
}
