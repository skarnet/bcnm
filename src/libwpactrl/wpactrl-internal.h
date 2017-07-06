/* ISC license. */

#ifndef BCNM_WPACTRL_INTERNAL_H
#define BCNM_WPACTRL_INTERNAL_H

#include <sys/types.h>
#include <skalibs/gccattributes.h>
#include <skalibs/tai.h>
#include <bcnm/wpactrl.h>

#define WPACTRL_PACKET_MAX 8192
#define WPACTRL_RECV_MAX 32

extern ssize_t wpactrl_fd_recv (int, char *, size_t) ;
extern ssize_t wpactrl_fd_timed_recv (int, char *, size_t, tain_t const *, tain_t *) ;
extern size_t wpactrl_filter_exact_search (wpactrl_t const *, char const *) gccattr_pure ;

#endif
