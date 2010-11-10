## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src  po

codeanalyzerdocdir = ${prefix}/doc/codeanalyzer
codeanalyzerdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(codeanalyzerdoc_DATA)

INTLTOOL_FILES = intltool-extract.in \
                 intltool-merge.in \
                 intltool-update.in intltool-extract \
                 intltool-merge \
                 intltool-update \
                 po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(codeanalyzerdocdir)
