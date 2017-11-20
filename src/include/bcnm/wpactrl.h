/* ISC license. */

#ifndef BCNM_WPACTRL_H
#define BCNM_WPACTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <stdint.h>
#include <errno.h>
#include <skalibs/gccattributes.h>
#include <skalibs/tai.h>
#include <skalibs/stralloc.h>
#include <skalibs/genalloc.h>

enum wparesponse_e
{
  WPA_OK = 0,
  WPA_PONG = 1,
  WPA_ERROR = -1,
  WPA_UNKNOWNCOMMAND = -2,
  WPA_FAIL = -3,
  WPA_FAILBUSY = -4,
  WPA_FAILCHECKSUM = -5,
  WPA_FAILINVALIDPIN = -6,
  WPA_FAILCHANNELUNAVAILABLE = -7,
  WPA_FAILCHANNELUNSUPPORTED = -8,
  WPA_FAILINVALIDRANGE = -9,
  WPA_FAILTOOLONGRESPONSE = -10,
  WPA_FAILPBCOVERLAP = -11,
  WPA_FAILUNKNOWNUUID = -12,
  WPA_FAILNOAPSETTINGS = -13,
  WPA_FAILNOIFNAMEMATCH = -14,
  WPA_UNKNOWNRESPONSE = -999
} ;
typedef enum wparesponse_e wparesponse_t, *wparesponse_t_ref ;

typedef struct wpactrl_s wpactrl_t, *wpactrl_t_ref ;
struct wpactrl_s
{
  int fds ;
  int fda ;
  uint32_t options ;
  tain_t tto ;
  size_t datahead ;
  stralloc data ;
  stralloc filters ;
} ;
#define WPACTRL_ZERO { -1, -1, 0, TAIN_ZERO, 0, STRALLOC_ZERO, STRALLOC_ZERO }

#define WPACTRL_OPTION_NOFILTER 0x0001U

extern wpactrl_t const wpactrl_zero ;

extern int wpactrl_start (wpactrl_t *, char const *, unsigned int, tain_t *) ;
#define wpactrl_start_g(a, path, timeout) wpactrl_start(a, path, (timeout), &STAMP)
extern void wpactrl_end (wpactrl_t *) ;

extern wparesponse_t wpactrl_command (wpactrl_t *, char const *, tain_t *) ;
#define wpactrl_command_g(a, q) wpactrl_command(a, (q), &STAMP)
extern ssize_t wpactrl_query (wpactrl_t *, char const *, char *, size_t, tain_t *) ;
#define wpactrl_query_g(a, q, ans, ansmax) wpactrl_query(a, q, ans, (ansmax), &STAMP)
extern int wpactrl_querysa (wpactrl_t *, char const *, stralloc *, tain_t *) ;
#define wpactrl_querysa_g(a, q, sa) wpactrl_querysa(a, q, (sa), &STAMP)

extern int wpactrl_filter_add (wpactrl_t *, char const *) ;
extern void wpactrl_filter_remove (wpactrl_t *, char const *) ;
extern int wpactrl_filter_match (wpactrl_t const *, char const *, size_t) ;

#define wpactrl_filter_activate(a) ((a)->options &= ~(uint32_t)WPACTRL_OPTION_NOFILTER)
#define wpactrl_filter_deactivate(a) ((a)->options |= WPACTRL_OPTION_NOFILTER)

#define wpactrl_fd(a) ((a)->fda)
extern int wpactrl_update (wpactrl_t *) ;
extern char *wpactrl_msg (wpactrl_t *) gccattr_pure ;
extern void wpactrl_ackmsg (wpactrl_t *) ;


 /* Helper functions for parsing answers from wpa_supplicant */

extern size_t wpactrl_bssid_scan (char const *, char *) ;
extern size_t wpactrl_flags_scan (char const *, stralloc *) ;

extern unsigned int wpactrl_env_parse (char *, size_t) ;

typedef struct wpactrl_scanres_s wpactrl_scanres_t, *wpactrl_scanres_t_ref ;
struct wpactrl_scanres_s
{
  char bssid[6] ;
  uint32_t frequency ;
  uint32_t signal_level ;
  size_t flags_start ;
  size_t flags_len ;
  size_t ssid_start ;
  size_t ssid_len ;
} ;
#define WPACTRL_SCANRES_ZERO { "\0\0\0\0\0", 0, 0, 0, 0, 0, 0 }

extern int wpactrl_scan_parse (char const *, size_t, genalloc * /* wpactrl_scanres_t */, stralloc *) ;

typedef struct wpactrl_networks_s wpactrl_networks_t, *wpactrl_networks_t_ref ;
struct wpactrl_networks_s
{
  uint32_t id ;
  size_t ssid_start ;
  size_t ssid_len ;
  char bssid[6] ;
  size_t flags_start ;
  size_t flags_len ;
} ;

extern int wpactrl_networks_parse (char const *, size_t, genalloc * /* wpactrl_networks_t */, stralloc *) ;


 /* Functions for easy iopause around async commands */

typedef int wpactrl_xchg_func_t (wpactrl_t *, char const *, size_t, void *, tain_t *) ;
typedef wpactrl_xchg_func_t *wpactrl_xchg_func_t_ref ;

typedef struct wpactrl_xchgitem_s wpactrl_xchgitem_t, *wpactrl_xchgitem_t_ref ;
struct wpactrl_xchgitem_s
{
  char const *filter ;
  wpactrl_xchg_func_t_ref cb ;
} ;

typedef struct wpactrl_xchg_s wpactrl_xchg_t, *wpactrl_xchg_t_ref ;
struct wpactrl_xchg_s
{
  wpactrl_xchgitem_t const *tab ; 
  unsigned int n ;
  unsigned int i ;
  tain_t deadline ;
  int status ;
  void *aux ;
} ;
#define WPACTRL_XCHG_ZERO { 0, 0, 0, TAIN_ZERO, ECONNABORTED, 0 }
#define WPACTRL_XCHG_INIT(array, size, limit, extra) { array, size, 0, limit, ECONNABORTED, extra }

extern wpactrl_xchg_t const wpactrl_xchg_zero ;
extern void wpactrl_xchg_init (wpactrl_xchg_t *, wpactrl_xchgitem_t const *, unsigned int, tain_t const *, void *) ;
extern int wpactrl_xchg_start (wpactrl_t *, wpactrl_xchg_t *) ;

extern void wpactrl_xchg_computedeadline (wpactrl_xchg_t const *, tain_t *) ;
extern int wpactrl_xchg_timeout (wpactrl_t *, wpactrl_xchg_t *, tain_t const *) ;
#define wpactrl_xchg_timeout_g(a, dt) wpactrl_xchg_timeout(a, (dt), &STAMP)
extern int wpactrl_xchg_event (wpactrl_t *, wpactrl_xchg_t *, tain_t *) ;
#define wpactrl_xchg_event_g(a, dt) wpactrl_xchg_event(a, (dt), &STAMP)

typedef struct wpactrl_xchg_cbres_s wpactrl_xchg_cbres_t, *wpactrl_xchg_cbres_t_ref ;
struct wpactrl_xchg_cbres_s
{
  genalloc parsed ;
  stralloc storage ;
} ;
#define WPACTRL_XCHG_CBRES_ZERO { GENALLOC_ZERO, STRALLOC_ZERO }

extern wpactrl_xchg_cbres_t const wpactrl_xchg_cbres_zero ;
extern void wpactrl_xchg_cbres_free (wpactrl_xchg_cbres_t *) ;


 /* High-level functions for common calls to wpa_supplicant */

extern int wpactrl_addnetwork (wpactrl_t *, uint32_t *, tain_t *) ;
#define wpactrl_addnetwork_g(a, idp) wpactrl_addnetwork(a, (idp), &STAMP)

extern wparesponse_t wpactrl_removenetwork (wpactrl_t *, uint32_t, tain_t *) ;
#define wpactrl_removenetwork_g(a, id) wpactrl_removenetwork(a, (id), &STAMP)

extern int wpactrl_findnetwork (wpactrl_t *, char const *, uint32_t *, tain_t *) ;
#define wpactrl_findnetwork_g(a, ssid, idp) wpactrl(a, ssid, (idp), &STAMP)

extern wparesponse_t wpactrl_setnetworkoption (wpactrl_t *, uint32_t, char const *, char const *, tain_t *) ;
#define wpactrl_setnetworkoption_g(a, id, var, val) wpactrl_setnetworkoption(a, id, var, (val), &STAMP)

extern wparesponse_t wpactrl_selectnetwork (wpactrl_t *, uint32_t, tain_t *) ;
#define wpactrl_selectnetwork_g(a, id) wpactrl_selectnetwork(a, (id), &STAMP)

extern int wpactrl_associate (wpactrl_t *, char const *, char const *, tain_t *) ;
#define wpactrl_associate_g(a, ssid, psk) wpactrl_associate(a, ssid, (psk), &STAMP)

extern int wpactrl_startscan (wpactrl_t *, wpactrl_xchg_t *, wpactrl_xchg_cbres_t *, tain_t const *, tain_t *) ;
#define wpactrl_startscan_g(a, xchg, res, limit) wpactrl_startscan(a, xchg, res, (limit), &STAMP)

#ifdef __cplusplus
}
#endif

#endif
