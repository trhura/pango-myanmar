MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
	-I$(top_builddir)/src/	\
	-std=c99		\
	-DPANGO_ENABLE_ENGINE	\
	-DPANGO_ENABLE_BACKEND  \
	-DG_DISABLE_DEPRECATED	\
	$(PANGO_CFLAGS)		\
	$(DATRIE_CFLAGS)	\
	$(GLIB_CFLAGS)

pangolibs = $(PANGO_LIBS) $(DATRIE_LIBS) $(GLIB_LIBS)
#EXTRA_DIST = sorttest.txt sorted.txt test-thcoll.sh test-thbrk.sh test-thwbrk.sh

TESTS_ENVIRONMENT = top_builddir=$(top_builddir)

TESTS = \
	test_mybrk

check_PROGRAMS = \
	test_mybrk

test_mybrk_SOURCES = \
	test_mybrk.c \
	$(top_builddir)/src/myctype.h \
	$(top_builddir)/src/myctype.c \
	$(top_builddir)/src/wbrk.h \
	$(top_builddir)/src/wbrk.c

test_mybrk_LDADD = $(pangolibs)