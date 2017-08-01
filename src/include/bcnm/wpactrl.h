/* ISC license. */

#ifndef BCNM_WPACTRL_H
#define BCNM_WPACTRL_H

#include <sys/types.h>
#include <stdint.h>
#include <errno.h>
#include <skalibs/gccattributes.h>
#include <skalibs/tai.h>
#include <skalibs/stralloc.h>
#include <skalibs/genalloc.h>

typedef enum wparesponse_e wparesponse_t, *wparesponse_t_ref ;
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
#define WPACTRL_ZERO { .fds = -1, .fda = -1, .options = 0, .tto = TAIN_ZERO, .datahead = 0, .data = STRALLOC_ZERO, .filters = STRALLOC_ZERO }

#define WPACTRL_OPTION_NOFILTER 0x0001U

extern wpactrl_t const wpactrl_zero ;

extern int wpactrl_start (wpactrl_t *, char const *, unsigned int, tain_t *) ;
#define wpactrl_start_g(a, path, timeout) wpactrl_start(a, path, (timeout), &STAMP)
extern void wpactrl_end (wpactrl_t *) ;

extern wparesponse_t wpactrl_command (wpactrl_t *, char const *, size_t, tain_t *) ;
#define wpactrl_command_g(a, q, qlen) wpactrl_command(a, q, (qlen), &STAMP)
extern ssize_t wpactrl_query (wpactrl_t *, char const *, size_t, char *, size_t, tain_t *) ;
#define wpactrl_query_g(a, q, qlen, ans, ansmax) wpactrl_query(a, q, qlen, ans, (ansmax), &STAMP)
extern int wpactrl_querysa (wpactrl_t *, char const *, size_t, stralloc *, tain_t *) ;
#define wpactrl_querysa_g(a, q, qlen, sa) wpactrl_querysa(a, q, qlen, (sa), &STAMP)

extern int wpactrl_filter_add (wpactrl_t *, char const *) ;
extern void wpactrl_filter_remove (wpactrl_t *, char const *) ;
extern int wpactrl_filter_match (wpactrl_t const *, char const *, size_t) ;

#define wpactrl_filter_activate(a) ((a)->options &= ~(uint32_t)WPACTRL_OPTION_NOFILTER)
#define wpactrl_filter_deactivate(a) ((a)->options |= WPACTRL_OPTION_NOFILTER)

extern int wpactrl_update (wpactrl_t *) ;
extern char *wpactrl_msg (wpactrl_t *) gccattr_pure ;
extern void wpactrl_ackmsg (wpactrl_t *) ;


 /* Helper functions for parsing answers from wpa_supplicant */

typedef struct wpactrl_scanres_s wpactrl_scanres_t, *wpactrl_scanres_t_ref ;
struct wpactrl_scanres_s
{
  char bssid[6] ;
  uint16_t frequency ;
  uint16_t signal_level ;
  size_t flags_start ;
  size_t flags_len ;
  size_t ssid_start ;
  size_t ssid_len ;
} ;
#define WPACTRL_SCANRES_ZERO { .bssid = "\0\0\0\0\0", .frequency = 0, .signal_level = 0, .flags_start = 0, .flags_len = 0, .ssid_start = 0, .ssid_len = 0 }

extern int wpactrl_scan_parse (char const *, size_t, genalloc * /* wpactrl_scanres_t */, stralloc *) ;
extern unsigned int wpactrl_env_parse (char *, size_t) ;


 /* Higher-level functions for easy iopause */

typedef int wpactrl_xchg_func_t (wpactrl_t *, char const *, size_t, void *, tain_t *) ;
typedef wpactrl_xchg_func_t *wpactrl_xchg_func_t_ref ;

typedef struct wpactrl_xchgitem_s wpactrl_xchgitem_t, *wpactrl_xchgitem_t_ref ;
struct wpactrl_xchgitem_s
{
  char const *filter ;
  wpactrl_xchg_func_t_ref f ;
} ;

typedef struct wpactrl_xchg_s wpactrl_xchg_t, *wpactrl_xchg_t_ref ;
struct wpactrl_xchg_s
{
  stralloc sa ;
  wpactrl_xchgitem_t const *tab ; 
  unsigned int n ;
  unsigned int i ;
  tain_t deadline ;
  int status ;
  void *aux ;
} ;
#define WPACTRL_XCHG_ZERO { .sa = STRALLOC_ZERO, .tab = 0, .n = 0, .i = 0, .deadline = TAIN_ZERO, .status = ECONNABORTED, .aux = 0 }
#define WPACTRL_XCHG_INIT(array, size, limit, extra) { .sa = STRALLOC_ZERO, .tab = array, .n = size, .i = 0, .deadline = limit, .status = ECONNABORTED, .aux = extra }

extern wpactrl_xchg_t const wpactrl_xchg_zero ;
extern void wpactrl_xchg_free (wpactrl_xchg_t *) ;
extern int wpactrl_xchg_init (wpactrl_xchg_t *, wpactrl_xchgitem_t const *, unsigned int, tain_t const *, void *) ;
extern int wpactrl_xchg_start (wpactrl_t *, wpactrl_xchg_t *) ;

extern void wpactrl_xchg_computedeadline (wpactrl_xchg_t const *, tain_t *) ;
extern int wpactrl_xchg_timeout (wpactrl_xchg_t *, tain_t const *) ;
#define wpactrl_xchg_timeout_g(dt) wpactrl_xchg_timeout((dt), &STAMP)
extern int wpactrl_xchg_event (wpactrl_t *, wpactrl_xchg_t *, tain_t *) ;
#define wpactrl_xchg_event_g(a, dt) wpactrl_xchg_event(a, (dt), &STAMP)


 /*
    The following is taken from wpa_supplicant's wpa_ctrl.h.
 */

#define WPA_CTRL_REQ "CTRL-REQ-"
#define WPA_CTRL_RSP "CTRL-RSP-"
#define WPA_EVENT_CONNECTED "CTRL-EVENT-CONNECTED "
#define WPA_EVENT_DISCONNECTED "CTRL-EVENT-DISCONNECTED "
#define WPA_EVENT_ASSOC_REJECT "CTRL-EVENT-ASSOC-REJECT "
#define WPA_EVENT_AUTH_REJECT "CTRL-EVENT-AUTH-REJECT "
#define WPA_EVENT_TERMINATING "CTRL-EVENT-TERMINATING "
#define WPA_EVENT_PASSWORD_CHANGED "CTRL-EVENT-PASSWORD-CHANGED "
#define WPA_EVENT_EAP_NOTIFICATION "CTRL-EVENT-EAP-NOTIFICATION "
#define WPA_EVENT_EAP_STARTED "CTRL-EVENT-EAP-STARTED "
#define WPA_EVENT_EAP_PROPOSED_METHOD "CTRL-EVENT-EAP-PROPOSED-METHOD "
#define WPA_EVENT_EAP_METHOD "CTRL-EVENT-EAP-METHOD "
#define WPA_EVENT_EAP_PEER_CERT "CTRL-EVENT-EAP-PEER-CERT "
#define WPA_EVENT_EAP_PEER_ALT "CTRL-EVENT-EAP-PEER-ALT "
#define WPA_EVENT_EAP_TLS_CERT_ERROR "CTRL-EVENT-EAP-TLS-CERT-ERROR "
#define WPA_EVENT_EAP_STATUS "CTRL-EVENT-EAP-STATUS "
#define WPA_EVENT_EAP_SUCCESS "CTRL-EVENT-EAP-SUCCESS "
#define WPA_EVENT_EAP_FAILURE "CTRL-EVENT-EAP-FAILURE "
#define WPA_EVENT_TEMP_DISABLED "CTRL-EVENT-SSID-TEMP-DISABLED "
#define WPA_EVENT_REENABLED "CTRL-EVENT-SSID-REENABLED "
#define WPA_EVENT_SCAN_STARTED "CTRL-EVENT-SCAN-STARTED "
#define WPA_EVENT_SCAN_RESULTS "CTRL-EVENT-SCAN-RESULTS "
#define WPA_EVENT_SCAN_FAILED "CTRL-EVENT-SCAN-FAILED "
#define WPA_EVENT_STATE_CHANGE "CTRL-EVENT-STATE-CHANGE "
#define WPA_EVENT_BSS_ADDED "CTRL-EVENT-BSS-ADDED "
#define WPA_EVENT_BSS_REMOVED "CTRL-EVENT-BSS-REMOVED "
#define WPA_EVENT_NETWORK_NOT_FOUND "CTRL-EVENT-NETWORK-NOT-FOUND "
#define WPA_EVENT_SIGNAL_CHANGE "CTRL-EVENT-SIGNAL-CHANGE "
#define WPA_EVENT_BEACON_LOSS "CTRL-EVENT-BEACON-LOSS "
#define WPA_EVENT_REGDOM_CHANGE "CTRL-EVENT-REGDOM-CHANGE "
#define WPA_EVENT_CHANNEL_SWITCH "CTRL-EVENT-CHANNEL-SWITCH "
#define WPA_EVENT_SUBNET_STATUS_UPDATE "CTRL-EVENT-SUBNET-STATUS-UPDATE "
#define IBSS_RSN_COMPLETED "IBSS-RSN-COMPLETED "
#define WPA_EVENT_FREQ_CONFLICT "CTRL-EVENT-FREQ-CONFLICT "
#define WPA_EVENT_AVOID_FREQ "CTRL-EVENT-AVOID-FREQ "
#define WPS_EVENT_OVERLAP "WPS-OVERLAP-DETECTED "
#define WPS_EVENT_AP_AVAILABLE_PBC "WPS-AP-AVAILABLE-PBC "
#define WPS_EVENT_AP_AVAILABLE_AUTH "WPS-AP-AVAILABLE-AUTH "
#define WPS_EVENT_AP_AVAILABLE_PIN "WPS-AP-AVAILABLE-PIN "
#define WPS_EVENT_AP_AVAILABLE "WPS-AP-AVAILABLE "
#define WPS_EVENT_CRED_RECEIVED "WPS-CRED-RECEIVED "
#define WPS_EVENT_M2D "WPS-M2D "
#define WPS_EVENT_FAIL "WPS-FAIL "
#define WPS_EVENT_SUCCESS "WPS-SUCCESS "
#define WPS_EVENT_TIMEOUT "WPS-TIMEOUT "
#define WPS_EVENT_ACTIVE "WPS-PBC-ACTIVE "
#define WPS_EVENT_DISABLE "WPS-PBC-DISABLE "
#define WPS_EVENT_ENROLLEE_SEEN "WPS-ENROLLEE-SEEN "
#define WPS_EVENT_OPEN_NETWORK "WPS-OPEN-NETWORK "
#define WPS_EVENT_ER_AP_ADD "WPS-ER-AP-ADD "
#define WPS_EVENT_ER_AP_REMOVE "WPS-ER-AP-REMOVE "
#define WPS_EVENT_ER_ENROLLEE_ADD "WPS-ER-ENROLLEE-ADD "
#define WPS_EVENT_ER_ENROLLEE_REMOVE "WPS-ER-ENROLLEE-REMOVE "
#define WPS_EVENT_ER_AP_SETTINGS "WPS-ER-AP-SETTINGS "
#define WPS_EVENT_ER_SET_SEL_REG "WPS-ER-AP-SET-SEL-REG "
#define DPP_EVENT_AUTH_SUCCESS "DPP-AUTH-SUCCESS "
#define DPP_EVENT_NOT_COMPATIBLE "DPP-NOT-COMPATIBLE "
#define DPP_EVENT_RESPONSE_PENDING "DPP-RESPONSE-PENDING "
#define DPP_EVENT_SCAN_PEER_QR_CODE "DPP-SCAN-PEER-QR-CODE "
#define DPP_EVENT_CONF_RECEIVED "DPP-CONF-RECEIVED "
#define DPP_EVENT_CONF_SENT "DPP-CONF-SENT "
#define DPP_EVENT_CONF_FAILED "DPP-CONF-FAILED "
#define DPP_EVENT_CONFOBJ_SSID "DPP-CONFOBJ-SSID "
#define DPP_EVENT_CONNECTOR "DPP-CONNECTOR "
#define DPP_EVENT_C_SIGN_KEY "DPP-C-SIGN-KEY "
#define DPP_EVENT_NET_ACCESS_KEY "DPP-NET-ACCESS-KEY "
#define DPP_EVENT_MISSING_CONNECTOR "DPP-MISSING-CONNECTOR "
#define DPP_EVENT_NETWORK_ID "DPP-NETWORK-ID "
#define MESH_GROUP_STARTED "MESH-GROUP-STARTED "
#define MESH_GROUP_REMOVED "MESH-GROUP-REMOVED "
#define MESH_PEER_CONNECTED "MESH-PEER-CONNECTED "
#define MESH_PEER_DISCONNECTED "MESH-PEER-DISCONNECTED "
#define MESH_SAE_AUTH_FAILURE "MESH-SAE-AUTH-FAILURE "
#define MESH_SAE_AUTH_BLOCKED "MESH-SAE-AUTH-BLOCKED "
#define WMM_AC_EVENT_TSPEC_ADDED "TSPEC-ADDED "
#define WMM_AC_EVENT_TSPEC_REMOVED "TSPEC-REMOVED "
#define WMM_AC_EVENT_TSPEC_REQ_FAILED "TSPEC-REQ-FAILED "
#define P2P_EVENT_DEVICE_FOUND "P2P-DEVICE-FOUND "
#define P2P_EVENT_DEVICE_LOST "P2P-DEVICE-LOST "
#define P2P_EVENT_GO_NEG_REQUEST "P2P-GO-NEG-REQUEST "
#define P2P_EVENT_GO_NEG_SUCCESS "P2P-GO-NEG-SUCCESS "
#define P2P_EVENT_GO_NEG_FAILURE "P2P-GO-NEG-FAILURE "
#define P2P_EVENT_GROUP_FORMATION_SUCCESS "P2P-GROUP-FORMATION-SUCCESS "
#define P2P_EVENT_GROUP_FORMATION_FAILURE "P2P-GROUP-FORMATION-FAILURE "
#define P2P_EVENT_GROUP_STARTED "P2P-GROUP-STARTED "
#define P2P_EVENT_GROUP_REMOVED "P2P-GROUP-REMOVED "
#define P2P_EVENT_CROSS_CONNECT_ENABLE "P2P-CROSS-CONNECT-ENABLE "
#define P2P_EVENT_CROSS_CONNECT_DISABLE "P2P-CROSS-CONNECT-DISABLE "
#define P2P_EVENT_PROV_DISC_SHOW_PIN "P2P-PROV-DISC-SHOW-PIN "
#define P2P_EVENT_PROV_DISC_ENTER_PIN "P2P-PROV-DISC-ENTER-PIN "
#define P2P_EVENT_PROV_DISC_PBC_REQ "P2P-PROV-DISC-PBC-REQ "
#define P2P_EVENT_PROV_DISC_PBC_RESP "P2P-PROV-DISC-PBC-RESP "
#define P2P_EVENT_PROV_DISC_FAILURE "P2P-PROV-DISC-FAILURE"
#define P2P_EVENT_SERV_DISC_REQ "P2P-SERV-DISC-REQ "
#define P2P_EVENT_SERV_DISC_RESP "P2P-SERV-DISC-RESP "
#define P2P_EVENT_SERV_ASP_RESP "P2P-SERV-ASP-RESP "
#define P2P_EVENT_INVITATION_RECEIVED "P2P-INVITATION-RECEIVED "
#define P2P_EVENT_INVITATION_RESULT "P2P-INVITATION-RESULT "
#define P2P_EVENT_INVITATION_ACCEPTED "P2P-INVITATION-ACCEPTED "
#define P2P_EVENT_FIND_STOPPED "P2P-FIND-STOPPED "
#define P2P_EVENT_PERSISTENT_PSK_FAIL "P2P-PERSISTENT-PSK-FAIL id="
#define P2P_EVENT_PRESENCE_RESPONSE "P2P-PRESENCE-RESPONSE "
#define P2P_EVENT_NFC_BOTH_GO "P2P-NFC-BOTH-GO "
#define P2P_EVENT_NFC_PEER_CLIENT "P2P-NFC-PEER-CLIENT "
#define P2P_EVENT_NFC_WHILE_CLIENT "P2P-NFC-WHILE-CLIENT "
#define P2P_EVENT_FALLBACK_TO_GO_NEG "P2P-FALLBACK-TO-GO-NEG "
#define P2P_EVENT_FALLBACK_TO_GO_NEG_ENABLED "P2P-FALLBACK-TO-GO-NEG-ENABLED "
#define ESS_DISASSOC_IMMINENT "ESS-DISASSOC-IMMINENT "
#define P2P_EVENT_REMOVE_AND_REFORM_GROUP "P2P-REMOVE-AND-REFORM-GROUP "
#define P2P_EVENT_P2PS_PROVISION_START "P2PS-PROV-START "
#define P2P_EVENT_P2PS_PROVISION_DONE "P2PS-PROV-DONE "
#define INTERWORKING_AP "INTERWORKING-AP "
#define INTERWORKING_BLACKLISTED "INTERWORKING-BLACKLISTED "
#define INTERWORKING_NO_MATCH "INTERWORKING-NO-MATCH "
#define INTERWORKING_ALREADY_CONNECTED "INTERWORKING-ALREADY-CONNECTED "
#define INTERWORKING_SELECTED "INTERWORKING-SELECTED "
#define CRED_ADDED "CRED-ADDED "
#define CRED_MODIFIED "CRED-MODIFIED "
#define CRED_REMOVED "CRED-REMOVED "
#define GAS_RESPONSE_INFO "GAS-RESPONSE-INFO "
#define GAS_QUERY_START "GAS-QUERY-START "
#define GAS_QUERY_DONE "GAS-QUERY-DONE "
#define ANQP_QUERY_DONE "ANQP-QUERY-DONE "
#define RX_ANQP "RX-ANQP "
#define RX_HS20_ANQP "RX-HS20-ANQP "
#define RX_HS20_ANQP_ICON "RX-HS20-ANQP-ICON "
#define RX_HS20_ICON "RX-HS20-ICON "
#define RX_MBO_ANQP "RX-MBO-ANQP "
#define HS20_SUBSCRIPTION_REMEDIATION "HS20-SUBSCRIPTION-REMEDIATION "
#define HS20_DEAUTH_IMMINENT_NOTICE "HS20-DEAUTH-IMMINENT-NOTICE "
#define EXT_RADIO_WORK_START "EXT-RADIO-WORK-START "
#define EXT_RADIO_WORK_TIMEOUT "EXT-RADIO-WORK-TIMEOUT "
#define RRM_EVENT_NEIGHBOR_REP_RXED "RRM-NEIGHBOR-REP-RECEIVED "
#define RRM_EVENT_NEIGHBOR_REP_FAILED "RRM-NEIGHBOR-REP-REQUEST-FAILED "

#endif