include $(top_srcdir)/build-aux/m4sh.mk

EXTRA_DIST +=					\
  msvc/cl.exe					\
  msvc/cppoutput				\
  msvc/cygpath					\
  msvc/lib					\
  msvc/lib.exe					\
  msvc/link.exe					\
  msvc/makedepend

# compilation
EXTRA_DIST += msvc/compilation.m4sh
nodist_check_SCRIPTS += msvc/compilation
m4sh_scripts += msvc/compilation

msvc/compilation.log: msvc/compilation
	@$(am__check_pre) $${dir}$< $(am__check_post)

# depcomp
EXTRA_DIST += msvc/depcomp.m4sh
nodist_check_SCRIPTS += msvc/depcomp
m4sh_scripts += msvc/depcomp

msvc/depcomp.log: msvc/depcomp
	@$(am__check_pre) $${dir}$< $(am__check_post)

MSVC_TESTS = msvc/compilation msvc/depcomp
TESTS += $(MSVC_TESTS)
