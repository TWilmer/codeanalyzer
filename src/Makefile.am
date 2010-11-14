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
	slice.h baobab/baobab-chart.h baobab/baobab-ringschart.h \
	icon16.png  icon32.png  icon64.png  icon.h  icon.svg \
	FileParser.hpp \
	main.cc \
	elf-bfd.h \
	elf/alpha.h   elf/cris.h      elf/frv.h   elf/internal.h  elf/mcore.h       elf/msp430.h        elf/s390.h     elf/x86-64.h elf/arc.h     elf/crx.h       elf/h8.h    elf/ip2k.h      elf/mep.h         elf/mt.h            elf/score.h    elf/xc16x.h elf/arm.h     elf/d10v.h      elf/hppa.h  elf/iq2000.h    elf/microblaze.h  elf/openrisc.h      elf/sh.h       elf/xstormy16.h elf/avr.h     elf/d30v.h      elf/i370.h  elf/lm32.h      elf/mips.h        elf/or32.h          elf/sparc.h    elf/xtensa.h elf/bfin.h    elf/dlx.h       elf/i386.h  elf/m32c.h      elf/mmix.h        elf/pj.h            elf/spu.h elf/common.h  elf/dwarf.h     elf/i860.h  elf/m32r.h      elf/mn10200.h     elf/ppc64.h         elf/v850.h elf/cr16c.h   elf/external.h  elf/i960.h  elf/m68hc11.h   elf/mn10300.h     elf/ppc.h           elf/vax.h elf/cr16.h    elf/fr30.h      elf/ia64.h  elf/m68k.h      elf/moxie.h       elf/reloc-macros.h  elf/vxworks.h \
	baobab/baobab-chart.c \
	baobab/baobab-ringschart.c  codeanalyzer.glade


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

