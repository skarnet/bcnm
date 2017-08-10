/* ISC license. */

#include <errno.h>
#include <bcnm/wpactrl.h>
#include "wpactrl-internal.h"

static int wpactrl_scan_cb (wpactrl_t *a, char const *s, size_t len, void *aux, tain_t *stamp)
{
  wpactrl_xchg_cbres_t *res = aux ;
  char buf[WPACTRL_PACKET_MAX] ;
  ssize_t r = wpactrl_query(a, "SCAN_RESULTS", buf, WPACTRL_PACKET_MAX, stamp) ;
  if (r <= 0) return 0 ;
  (void)s ;
  (void)len ;
  return wpactrl_scan_parse(buf, r, &res->parsed, &res->storage) ;
}

static wpactrl_xchgitem_t wpactrl_xchgitem_scan =
{
  .filter = "CTRL-EVENT-SCAN-RESULTS",
  .cb = &wpactrl_scan_cb
} ;

int wpactrl_startscan (wpactrl_t *a, wpactrl_xchg_t *xchg, wpactrl_xchg_cbres_t *res, tain_t const *deadline, tain_t *stamp)
{
  wparesponse_t r = wpactrl_command(a, "SCAN", stamp) ;
  if (r != WPA_OK && r != WPA_FAILBUSY) return (errno = EIO, 0) ;
  wpactrl_xchg_init(xchg, &wpactrl_xchgitem_scan, 1, deadline, res) ;
  return wpactrl_xchg_start(a, xchg) ;
}
