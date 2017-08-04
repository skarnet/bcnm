/* ISC license. */

#include <string.h>
#include <errno.h>
#include <skalibs/error.h>
#include <skalibs/fmtscan.h>
#include <bcnm/wpactrl.h>

size_t wpactrl_bssid_scan (char const *s, char *bssid)
{
  unsigned int i = 0 ;
  if (!strncmp(s, "any", 3))
  {
    memset(bssid, 0, 6) ;
    return 3 ;
  }
  for (; i < 5 ; i++)
  {
    if (!ucharn_scan(s, bssid + i, 1)) goto eproto ;
    if (s[2] != ':') goto eproto ;
    s += 3 ;
  }
  if (!ucharn_scan(s, bssid + 5, 1)) goto eproto ;
  return 17 ;

 eproto:
  return (errno = EPROTO, 0) ;
}

