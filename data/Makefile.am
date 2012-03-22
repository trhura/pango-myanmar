MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = mybrk.tri mydict.txt map.sed map.h

dictdatadir = $(datadir)/libburmese

if ENABLE_DICT
dictdata_DATA = mybrk.tri
else
dictdata_DATA =
endif

mydict_SRC = \
	$(srcdir)/mydict-common-words.txt \
	$(srcdir)/mydict-mmnlp-words.txt \
	$(srcdir)/mydict-syllables.txt

EXTRA_DIST = mybrk.abm $(mydict_SRC)

mydict.txt: $(mydict_SRC)
	cat $(mydict_SRC) | LC_ALL=C sort -u > mydict.txt
	python generator.py map
	sed -i -f map.sed mydict.txt
	sed -i -e "/^\s*$$/d" mydict.txt
	cp map.h ../src/

mybrk.tri: mydict.txt
	if test ! -e mybrk.abm; then \
	  ln -s $(top_srcdir)/data/mybrk.abm . ; \
	fi
	rm -f mybrk.tri
	$(TRIETOOL) mybrk add-list -e utf-8 mydict.txt

