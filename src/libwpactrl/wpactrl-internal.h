/* ISC license. */

#ifndef BCNM_WPACTRL_INTERNAL_H
#define BCNM_WPACTRL_INTERNAL_H

#include <sys/types.h>
#include <stdint.h>
#include <skalibs/gccattributes.h>
#include <skalibs/tai.h>
#include <bcnm/wpactrl.h>

#define WPACTRL_PACKET_MAX 8191
#define WPACTRL_RECV_MAX 32

extern ssize_t wpactrl_fd_recv (int, char *, size_t) ;
extern ssize_t wpactrl_fd_timed_recv (int, char *, size_t, tain const *, tain *) ;
extern size_t wpactrl_filter_exact_search (wpactrl_t const *, char const *) gccattr_pure ;

extern size_t wpactrl_xchgset_find (wpactrl_t const *, uint32_t) ;

#endif
