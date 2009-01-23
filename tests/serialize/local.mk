AM_CPPFLAGS +=					\
  $(BOOST_CPPFLAGS)				\
  $(LIBPORT_CPPFLAGS)

AM_LDFLAGS +=                                   \
  $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)

LDADD +=                                        \
  $(top_builddir)/lib/libport/libport.la        \
  $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)

TESTS +=					\
  serialize/serialize.cc

serialize_serialize_SOURCES = serialize/serialize.cc
serialize_serialize_LDFLAGS = $(top_builddir)/lib/serialize/libserialize.la $(AM_LDFLAGS)