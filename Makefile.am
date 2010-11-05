## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

codeanalyzerdocdir = ${prefix}/doc/codeanalyzer
codeanalyzerdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(codeanalyzerdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(codeanalyzerdocdir)
