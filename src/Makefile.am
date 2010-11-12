## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPROGRAMNAME_LOCALEDIR=\"${PROGRAMNAME_LOCALEDIR}\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(CODEANALYZER_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -Werror\
	 -g

bin_PROGRAMS = codeanalyzer

codeanalyzer_SOURCES =  icon.h \
	FileParser.cc \
	main.cc \
	baobab/baobab-chart.c \
	baobab/baobab-ringschart.c 

codeanalyzer_LDFLAGS =   -lbfd -liberty

codeanalyzer_LDADD = $(CODEANALYZER_LIBS) codeanalyzer.glade.o

main.cc : icon.h

main.cc : codeanalyzer.glade.o


icon.h: icon32.png icon64.png icon16.png
	gdk-pixbuf-csource  --build-list  icon16 icon16.png  icon32 icon32.png  icon64 icon64.png > icon.h


windres.o: windres.rc windres.ico
	@WINDRES@ -I$(srcdir) -I. $(srcdir)/windres.rc windres.o


if USE_WIN32
codeanalyzer_LDADD +=  windres.o
endif





# embedd glade file into Program
CLEANFILES= codeanalyzer.glade.o icon.h
codeanalyzer.glade.o : codeanalyzer.glade
	$(OBJCOPY)  -I binary -B $(OBJCOPYARCH) -O $(OBJCOPYTARGET) codeanalyzer.glade codeanalyzer.glade.o



# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(datadir)/codeanalyzer

