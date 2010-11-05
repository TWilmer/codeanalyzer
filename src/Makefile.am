## Process this file with automake to produce Makefile.in

## Created by Anjuta

uidir = $(datadir)/codeanalyzer/ui
ui_DATA = codeanalyzer.ui

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(CODEANALYZER_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = codeanalyzer

codeanalyzer_SOURCES = \
	main.cc

codeanalyzer_LDFLAGS = 

codeanalyzer_LDADD = $(CODEANALYZER_LIBS)

EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(datadir)/codeanalyzer
