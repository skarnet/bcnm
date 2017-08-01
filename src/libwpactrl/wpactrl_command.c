/* ISC license. */

#include <string.h>
#include <errno.h>
#include <bcnm/wpactrl.h>

#define WPARESPONSE_MAXLEN 28

struct wparesponse_map_s
{
  char const *s ;
  wparesponse_t r ;
} ;

wparesponse_t wpactrl_command (wpactrl_t *a, char const *s, size_t len, tain_t *stamp)
{
  static struct wparesponse_map_s const wparesponses[] =
  {
    {  "OK\n", WPA_OK },
    { "PONG\n", WPA_PONG },
    { "UNKNOWN COMMAND\n", WPA_UNKNOWNCOMMAND },
    { "FAIL\n", WPA_FAIL },
    { "FAIL-BUSY\n", WPA_FAILBUSY },
    { "FAIL-CHECKSUM\n", WPA_FAILCHECKSUM },
    { "FAIL-INVALID-PIN\n", WPA_FAILINVALIDPIN },
    { "FAIL-CHANNEL-UNAVAILABLE\n", WPA_FAILCHANNELUNAVAILABLE },
    { "FAIL-CHANNEL-UNSUPPORTED\n", WPA_FAILCHANNELUNSUPPORTED },
    { "FAIL-Invalid range\n", WPA_FAILINVALIDRANGE },
    { "FAIL-Too long response\n", WPA_FAILTOOLONGRESPONSE },
    { "FAIL-PBC-OVERLAP\n", WPA_FAILPBCOVERLAP },
    { "FAIL-UNKNOWN-UUID\n", WPA_FAILUNKNOWNUUID },
    { "FAIL-NO-AP-SETTINGS\n", WPA_FAILNOAPSETTINGS },
    { "FAIL-NO-IFNAME-MATCH\n", WPA_FAILNOIFNAMEMATCH },
    { 0, WPA_UNKNOWNRESPONSE }
  } ;
  char buf[WPARESPONSE_MAXLEN] ;
  ssize_t r = wpactrl_query(a, s, len, buf, WPARESPONSE_MAXLEN, stamp) ;
  if (r < 0) return WPA_ERROR ;
  if (!r) return (errno = EPIPE, WPA_ERROR) ;
  {
    unsigned int i = 0 ;
    for (; wparesponses[i].s ; i++)
      if (!strncmp(buf, wparesponses[i].s, r))
        break ;
    return wparesponses[i].r ;
  }
}
