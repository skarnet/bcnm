#
# This file has been generated by tools/gen-deps.sh
#

src/libwpactrl/wpactrl-internal.h: src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_ackmsg.o src/libwpactrl/wpactrl_ackmsg.lo: src/libwpactrl/wpactrl_ackmsg.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_command.o src/libwpactrl/wpactrl_command.lo: src/libwpactrl/wpactrl_command.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_end.o src/libwpactrl/wpactrl_end.lo: src/libwpactrl/wpactrl_end.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_env_parse.o src/libwpactrl/wpactrl_env_parse.lo: src/libwpactrl/wpactrl_env_parse.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_fd_recv.o src/libwpactrl/wpactrl_fd_recv.lo: src/libwpactrl/wpactrl_fd_recv.c src/include/bcnm/wpactrl.h src/libwpactrl/wpactrl-internal.h
src/libwpactrl/wpactrl_fd_timed_recv.o src/libwpactrl/wpactrl_fd_timed_recv.lo: src/libwpactrl/wpactrl_fd_timed_recv.c src/libwpactrl/wpactrl-internal.h
src/libwpactrl/wpactrl_filter_add.o src/libwpactrl/wpactrl_filter_add.lo: src/libwpactrl/wpactrl_filter_add.c src/include/bcnm/wpactrl.h src/libwpactrl/wpactrl-internal.h
src/libwpactrl/wpactrl_filter_exact_search.o src/libwpactrl/wpactrl_filter_exact_search.lo: src/libwpactrl/wpactrl_filter_exact_search.c src/libwpactrl/wpactrl-internal.h
src/libwpactrl/wpactrl_filter_match.o src/libwpactrl/wpactrl_filter_match.lo: src/libwpactrl/wpactrl_filter_match.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_filter_remove.o src/libwpactrl/wpactrl_filter_remove.lo: src/libwpactrl/wpactrl_filter_remove.c src/include/bcnm/wpactrl.h src/libwpactrl/wpactrl-internal.h
src/libwpactrl/wpactrl_msg.o src/libwpactrl/wpactrl_msg.lo: src/libwpactrl/wpactrl_msg.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_query.o src/libwpactrl/wpactrl_query.lo: src/libwpactrl/wpactrl_query.c src/include/bcnm/wpactrl.h src/libwpactrl/wpactrl-internal.h
src/libwpactrl/wpactrl_querysa.o src/libwpactrl/wpactrl_querysa.lo: src/libwpactrl/wpactrl_querysa.c src/include/bcnm/wpactrl.h src/libwpactrl/wpactrl-internal.h
src/libwpactrl/wpactrl_scan_parse.o src/libwpactrl/wpactrl_scan_parse.lo: src/libwpactrl/wpactrl_scan_parse.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_start.o src/libwpactrl/wpactrl_start.lo: src/libwpactrl/wpactrl_start.c src/include/bcnm/wpactrl.h src/libwpactrl/wpactrl-internal.h
src/libwpactrl/wpactrl_update.o src/libwpactrl/wpactrl_update.lo: src/libwpactrl/wpactrl_update.c src/include/bcnm/wpactrl.h src/libwpactrl/wpactrl-internal.h
src/libwpactrl/wpactrl_xchg_computedeadline.o src/libwpactrl/wpactrl_xchg_computedeadline.lo: src/libwpactrl/wpactrl_xchg_computedeadline.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_xchg_event.o src/libwpactrl/wpactrl_xchg_event.lo: src/libwpactrl/wpactrl_xchg_event.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_xchg_free.o src/libwpactrl/wpactrl_xchg_free.lo: src/libwpactrl/wpactrl_xchg_free.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_xchg_init.o src/libwpactrl/wpactrl_xchg_init.lo: src/libwpactrl/wpactrl_xchg_init.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_xchg_start.o src/libwpactrl/wpactrl_xchg_start.lo: src/libwpactrl/wpactrl_xchg_start.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_xchg_timeout.o src/libwpactrl/wpactrl_xchg_timeout.lo: src/libwpactrl/wpactrl_xchg_timeout.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_xchg_zero.o src/libwpactrl/wpactrl_xchg_zero.lo: src/libwpactrl/wpactrl_xchg_zero.c src/include/bcnm/wpactrl.h
src/libwpactrl/wpactrl_zero.o src/libwpactrl/wpactrl_zero.lo: src/libwpactrl/wpactrl_zero.c src/include/bcnm/wpactrl.h

libwpactrl.a.xyzzy: src/libwpactrl/wpactrl_ackmsg.o src/libwpactrl/wpactrl_command.o src/libwpactrl/wpactrl_end.o src/libwpactrl/wpactrl_env_parse.o src/libwpactrl/wpactrl_fd_recv.o src/libwpactrl/wpactrl_fd_timed_recv.o src/libwpactrl/wpactrl_filter_add.o src/libwpactrl/wpactrl_filter_exact_search.o src/libwpactrl/wpactrl_filter_match.o src/libwpactrl/wpactrl_filter_remove.o src/libwpactrl/wpactrl_msg.o src/libwpactrl/wpactrl_query.o src/libwpactrl/wpactrl_querysa.o src/libwpactrl/wpactrl_scan_parse.o src/libwpactrl/wpactrl_start.o src/libwpactrl/wpactrl_update.o src/libwpactrl/wpactrl_xchg_computedeadline.o src/libwpactrl/wpactrl_xchg_event.o src/libwpactrl/wpactrl_xchg_free.o src/libwpactrl/wpactrl_xchg_init.o src/libwpactrl/wpactrl_xchg_start.o src/libwpactrl/wpactrl_xchg_timeout.o src/libwpactrl/wpactrl_xchg_zero.o src/libwpactrl/wpactrl_zero.o
libwpactrl.so.xyzzy: EXTRA_LIBS :=
libwpactrl.so.xyzzy: src/libwpactrl/wpactrl_ackmsg.lo src/libwpactrl/wpactrl_command.lo src/libwpactrl/wpactrl_end.lo src/libwpactrl/wpactrl_env_parse.lo src/libwpactrl/wpactrl_fd_recv.lo src/libwpactrl/wpactrl_fd_timed_recv.lo src/libwpactrl/wpactrl_filter_add.lo src/libwpactrl/wpactrl_filter_exact_search.lo src/libwpactrl/wpactrl_filter_match.lo src/libwpactrl/wpactrl_filter_remove.lo src/libwpactrl/wpactrl_msg.lo src/libwpactrl/wpactrl_query.lo src/libwpactrl/wpactrl_querysa.lo src/libwpactrl/wpactrl_scan_parse.lo src/libwpactrl/wpactrl_start.lo src/libwpactrl/wpactrl_update.lo src/libwpactrl/wpactrl_xchg_computedeadline.lo src/libwpactrl/wpactrl_xchg_event.lo src/libwpactrl/wpactrl_xchg_free.lo src/libwpactrl/wpactrl_xchg_init.lo src/libwpactrl/wpactrl_xchg_start.lo src/libwpactrl/wpactrl_xchg_timeout.lo src/libwpactrl/wpactrl_xchg_zero.lo src/libwpactrl/wpactrl_zero.lo
