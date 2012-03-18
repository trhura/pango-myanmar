## Process this file with automake to create Makefile.in.

myanmar_fc_sources = myanmar-fc.c 

myanmar_lang_sources =	myanmar.h \
			myctype.h \
			myctype.c \
			wbrk.h	  \
			wbrk.c	  \
			map.h	\
		    	myanmar-lang.c 

pangolibs = $(PANGO_LIBS) $(DATRIE_LIBS)
pangoft2libs = $(PANGOFT2_LIBS) $(FREETYPE_LIBS) $(pangolibs)

moddefine = $(FT2_PREFIX)
moduledir = $(libdir)/pango/$(PANGO_MODULE_VERSION)/modules

if HAVE_FREETYPE
PANGO_FC_INCLUDES = $(FREETYPE_CFLAGS)
PANGO_FC_MODULES = pango-myanmar-fc.la
endif

INCLUDES =			\
	-std=c99	    \
	-DPANGO_ENABLE_ENGINE	\
	-DPANGO_ENABLE_BACKEND  \
	-DG_DISABLE_DEPRECATED	\
	$(PANGO_CFLAGS)		\
	$(DATRIE_CFLAGS)	\
	$(moddefine)		\
	$(GLIB_CFLAGS)		\
	$(FREETYPE_CFLAGS)

module_LTLIBRARIES = $(PANGO_FC_MODULES) pango-myanmar-lang.la

pango_myanmar_fc_la_LDFLAGS = -export-dynamic -avoid-version -module
pango_myanmar_fc_la_LIBADD = $(pangoft2libs)
pango_myanmar_fc_la_SOURCES = $(myanmar_fc_sources)

pango_myanmar_lang_la_LDFLAGS = -export-dynamic -avoid-version -module
pango_myanmar_lang_la_LIBADD = $(pangolibs)
pango_myanmar_lang_la_SOURCES = $(myanmar_lang_sources)

included-modules: $(noinst_LTLIBRARIES)

.PHONY: included-modules
