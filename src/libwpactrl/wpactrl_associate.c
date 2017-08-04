/* ISC license. */

#include <stdint.h>
#include <errno.h>
#include <bcnm/wpactrl.h>

int wpactrl_associate (wpactrl_t *a, char const *ssid, char const *psk, tain_t *stamp)
{
  uint32_t id ;
  int r = wpactrl_findnetwork(a, ssid, &id, stamp) ;
  if (r < 0) return 0 ;
  if (!r)
  {
    if (!wpactrl_addnetwork(a, &id, stamp)) goto err ;
  }

  if (psk)
  {
    if (wpactrl_setnetworkoption(a, id, "key_mgmt", "WPA-PSK WPA-PSK-SHA256", stamp) != WPA_OK
     || wpactrl_setnetworkoption(a, id, "mem_only_psk", "1", stamp) != WPA_OK
     || wpactrl_setnetworkoption(a, id, "psk", psk, stamp) != WPA_OK) goto err ;
  }
  else
  {
    if (wpactrl_setnetworkoption(a, id, "key_mgmt", "NONE", stamp) != WPA_OK)
      goto err ;
  }

  if (wpactrl_selectnetwork(a, id, stamp) != WPA_OK) goto err ;
  return 1 ;

 err:
  return (errno = EIO, 0) ;
}
