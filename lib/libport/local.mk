lib_LTLIBRARIES += lib/libport/libport.la
lib_libport_libport_la_LDFLAGS = -avoid-version -no-undefined
lib_libport_libport_la_CPPFLAGS =		\
  $(AM_CPPFLAGS)				\
  $(LIBPORT_CPPFLAGS)				\
  -I$(srcdir)/lib				\
  -DBUILDING_LIBPORT				\
  $(BOOST_CPPFLAGS)

# Sources to compile to use libport.
dist_lib_libport_libport_la_SOURCES = 		\
  lib/libport/base64.cc				\
  lib/libport/buffer-stream.cc			\
  lib/libport/cli.cc				\
  lib/libport/compiler.cc			\
  lib/libport/contract.cc			\
  lib/libport/csignal.cc			\
  lib/libport/cstdio.cc				\
  lib/libport/cstdlib.cc			\
  lib/libport/debug.cc				\
  lib/libport/deref.cc				\
  lib/libport/dirent.cc				\
  lib/libport/escape.cc				\
  lib/libport/fd-stream.cc			\
  lib/libport/path.cc				\
  lib/libport/file-library.cc			\
  lib/libport/file-system.cc			\
  lib/libport/indent.cc				\
  lib/libport/package-info.cc			\
  lib/libport/pid-file.cc			\
  lib/libport/program-name.cc			\
  lib/libport/read-stdin.cc			\
  lib/libport/sched.cc				\
  lib/libport/semaphore.cc			\
  lib/libport/symbol.cc				\
  lib/libport/sysexits.cc			\
  lib/libport/timer.cc				\
  lib/libport/tokenizer.cc			\
  lib/libport/ufloat.cc				\
  lib/libport/unique-pointer.cc			\
  lib/libport/unistd.cc				\
  lib/libport/utime.cc

# These are broken and someone will have to fix them...
# libport_sources += 				\
#   $(libport_srcdir)/uffloat.cc		\
#   $(libport_srcdir)/ull-fixed-point.cc


# Make sure nobody uses config.h instead of libport/config.h.
#maintainer-check:
#	cd $(srcdir) && ! grep -E '[<"]config.h[">]' $(nobase_libport_HEADERS) $(libport_HEADERS)
# GNU Make 3.80 does not recognize that `!' is a shell builtin, and
# tries to call it via exec.  So help GNU Make understand it should
# call the shell.
#	: && ! grep -E '[<"]config.h[">]' $(dist_lib_libport_libport_la_SOURCES)