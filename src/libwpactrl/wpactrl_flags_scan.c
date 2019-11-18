/* ISC license. */

#include <errno.h>
#include <skalibs/bytestr.h>
#include <skalibs/error.h>
#include <skalibs/stralloc.h>
#include <bcnm/wpactrl.h>

size_t wpactrl_flags_scan (char const *t, stralloc *sa)
{
  size_t sabase = sa->len ;
  int wasnull = !sa->s ;
  char const *s = t ;
  while (*s == '[')
  {
    size_t pos ;
    s++;
    pos = str_chr(s, ']') ;
    if (!s[pos]) goto eproto ;
    if (!stralloc_catb(sa, s, pos) || !stralloc_0(sa)) goto err ;
    s += pos + 1 ;
  }
  return s - t ;

 eproto:
  errno = EPROTO ;
 err:
  if (wasnull) stralloc_free(sa) ;
  else sa->len = sabase ;
  return 0 ;
}
