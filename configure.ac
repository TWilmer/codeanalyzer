dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_CONFIG_MACRO_DIR([m4])

AC_INIT(codeanalyzer, 0.1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CXX
AM_PROG_CC_STDC
AC_HEADER_STDC




AC_CHECK_DECLS(basename)

AM_PROG_LIBTOOL


AC_CHECK_LIB([bfd], [bfd_init], , [ac_found_bfd=no])


PKG_CHECK_MODULES(CODEANALYZER, [gtkmm-2.4 >= 2.16    ])

AC_MSG_CHECKING([for native Win32])
LIB_EXE_MACHINE_FLAG=X86
case "$host" in
  *-mingw*)
    os_win32=yes
    gio_can_sniff=no
    MATH_LIB=
    case "$host" in
    x86_64-*-*)
      LIB_EXE_MACHINE_FLAG=X64
      ;;
    esac
    ;;
  *)
    os_win32=no
    ;;
esac
AC_MSG_RESULT([$os_win32])

AC_SUBST(LIB_EXE_MACHINE_FLAG)

case $host in
  *-*-linux*)
    os_linux=yes
    ;;
esac


AM_CONDITIONAL(OS_WIN32, test "$os_win32" = "yes")
AM_CONDITIONAL(USE_WIN32, test "$os_win32" = "yes")
AM_CONDITIONAL(OS_UNIX, test "$os_win32" != "yes")
AM_CONDITIONAL(OS_LINUX, test "$os_linux" = "yes")


GNUPG_CHECK_LIBUSB

AC_CHECK_TOOL(OBJCOPY, objcopy)
if test "x$OBJCOPY" = "x"; then
        AC_MSG_ERROR("objcopy from GNU binutils >= 2.11.90 not found")
fi


AC_CHECK_TOOL(OBJDUMP, objdump)
dnl extract target and architecture if objdump was found
if test "x$OBJDUMP" = "x"; then :; else
  AC_MSG_CHECKING(for object target)
  octarget=`$OBJDUMP --info | sed -ne '2p'` # extract first target
  AC_MSG_RESULT($octarget)
  AC_MSG_CHECKING(for object architecture)
  ocarch=`$OBJDUMP --info | sed -ne '4p'` # extract corresponding arch
  AC_MSG_RESULT($ocarch)
fi
AC_SUBST(OBJCOPYTARGET, $octarget)
AC_SUBST(OBJCOPYARCH, $ocarch)


if test "$os_win32" = "yes"; then
 AC_CHECK_TOOL(WINDRES, windres, no)
  if test "$WINDRES" = no; then
    AC_MSG_ERROR([*** Could not find an implementation of windres in your PATH.])
  fi
fi


AC_CHECK_TOOL(OBJCOPY, objcopy)


LT_PROG_RC
 
AC_OUTPUT([
Makefile
src/Makefile

])
