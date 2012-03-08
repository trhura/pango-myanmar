## Process this file with automake to create Makefile.in.

myanmar_lang_sources = myanmar.h \
		       myanmar-lang.c 

pangolibs = $(PANGO_LIBS)
pangoft2libs = $(PANGOFT2_LIBS) $(FREETYPE_LIBS) $(pangolibs)

moddefine = $(FT2_PREFIX)
moduledir = $(libdir)/pango/$(PANGO_MODULE_VERSION)/modules

INCLUDES =			\
	-DPANGO_ENABLE_ENGINE	\
	-DPANGO_ENABLE_BACKEND  \
	-DG_DISABLE_DEPRECATED	\
	$(PANGO_CFLAGS)		\
	$(moddefine)		\
	$(GLIB_CFLAGS)		\
	$(FREETYPE_CFLAGS)

module_LTLIBRARIES = $(PANGO_FC_MODULES) pango-myanmar-lang.la

pango_myanmar_lang_la_LDFLAGS = -export-dynamic -avoid-version -module
pango_myanmar_lang_la_LIBADD = $(pangolibs)
pango_myanmar_lang_la_SOURCES = $(myanmar_lang_sources)

included-modules: $(noinst_LTLIBRARIES)

.PHONY: included-modules
