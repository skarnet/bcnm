/* ISC license. */

#include <string.h>
#include <errno.h>
#include <bcnm/wpactrl.h>

#define WPARESPONSE_MAXLEN 28

wparesponse_t wpactrl_command (wpactrl_t *a, char const *s, size_t len, tain_t const *deadline, tain_t *stamp)
{
  static char const *wparesponses[] =
  {
    "OK\n",
    "PONG\n",
    "UNKNOWN COMMAND\n",
    "FAIL\n",
    "FAIL-BUSY\n",
    "FAIL-CHECKSUM\n",
    "FAIL-INVALID-PIN\n",
    "FAIL-CHANNEL-UNAVAILABLE\n",
    "FAIL-CHANNEL-UNSUPPORTED\n",
    "FAIL-Invalid range\n",
    "FAIL-Too long response\n",
    "FAIL-PBC-OVERLAP\n",
    "FAIL-UNKNOWN-UUID\n",
    "FAIL-NO-AP-SETTINGS\n",
    "FAIL-NO-IFNAME-MATCH\n",
    0
  } ;
  char buf[WPARESPONSE_MAXLEN] ;
  ssize_t r = wpactrl_query(a, s, len, buf, WPARESPONSE_MAXLEN, deadline, stamp) ;
  if (r < 0) return WPA_ERROR ;
  if (!r) return (errno = EPIPE, WPA_ERROR) ;
  {
    wparesponse_t i = 0 ;
    for (; wparesponses[i] ; i++)
      if (!strncmp(buf, wparesponses[i], r))
        break ;
    return i ;
  }
}
