/* ISC license. */

#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <skalibs/uint16.h>
#include <skalibs/bytestr.h>
#include <skalibs/error.h>
#include <skalibs/fmtscan.h>
#include <skalibs/stralloc.h>
#include <skalibs/genalloc.h>
#include <bcnm/wpactrl.h>

static size_t bssid_scan (char const *s, char *bssid)
{
  unsigned int i = 0 ;
  char sep[6] = ":::::\t" ;
  for (; i < 6 ; i++)
  {
    if (!ucharn_scan(s, bssid + i, 1)) return 0 ;
    if (s[2] != sep[i]) return 0 ;
    s += 3 ;
  }
  return 18 ;
}

static size_t flags_scan (char const *s, uint32_t *flags)
{
  return 0 ;
}

static int wpactrl_scan_parse_one (char const *s, size_t len, wpactrl_scanres_t *thing, stralloc *sa)
{
  size_t pos = byte_chr(s, len, '\t') ;
  if (pos >= len) return 0 ;
  if (pos != 18) return 0 ;
  if (bssid_scan(s, thing->bssid) != pos) return 0 ;
  s += pos + 1 ; len -= pos + 1 ;
  pos = byte_chr(s, len, '\t') ;
  if (pos >= len) return 0 ;
  if (uint16_scan(s, &thing->frequency) != pos) return 0 ;
  s += pos + 1 ; len -= pos + 1 ;
  pos = byte_chr(s, len, '\t') ;
  if (pos >= len) return 0 ;
  if (flags_scan(s, &thing->flags) != pos) return 0 ;
  s += pos + 1 ; len -= pos + 1 ;
  thing->ssid = sa->len ;
  return stralloc_catb(sa, s, len - 1) && stralloc_0(sa) ;
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
  {
    int e = errno ;
    if (gawasnull) genalloc_free(wpactrl_scanres_t, ga) ;
    else genalloc_setlen(wpactrl_scanres_t, ga, gabase) ;
    if (sawasnull) stralloc_free(sa) ;
    else sa->len = sabase ;
    errno = e ;
  }
  return 0 ;
}
