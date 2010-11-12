dnl Check for libbfd
dnl Copyright (C) 2004, 2005 Free Software Foundation, Inc.
dnl
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.
dnl
dnl Defines HAVE_LIBBFD to 1 if a working libbfd setup is found, and sets
dnl @LIBBFD@ to the necessary libraries.  HAVE_BFD_GET_BUSSES is set if
dnl bfd_init() exists.

AC_DEFUN([GNUPG_CHECK_LIBBFD],
[
  AC_ARG_WITH(libbfd,
     AC_HELP_STRING([--with-libbfd=DIR],
	[look for the libbfd library in DIR]),
     [_do_libbfd=$withval],[_do_libbfd=yes])

  if test "$_do_libbfd" != "no" ; then
     if test -d "$withval" ; then
        CPPFLAGS="${CPPFLAGS} -I$withval/include"
        LDFLAGS="${LDFLAGS} -L$withval/lib"
        AC_PATH_PROG([_bfd_config],["$_do_libbfd/bin/libbfd-config"])
     else
        AC_PATH_PROG([_bfd_config],[libbfd-config])
     fi

     _libbfd_save_libs=$LIBS
     _libbfd_save_cflags=$CFLAGS

     if test x$_bfd_config != "x" ; then
        _libbfd_try_libs=`$LIBS $_bfd_config --libs`
        _libbfd_try_cflags=`$LIBS $_bfd_config --cflags`
     else
        _libbfd_try_libs="-lbfd"
        _libbfd_try_cflags=""
     fi

     LIBS="$LIBS $_libbfd_try_libs"
     CFLAGS="$CFLAGS $_libbfd_try_cflags"

     AC_MSG_CHECKING([whether libbfd is present and sane])

     AC_LINK_IFELSE(AC_LANG_PROGRAM([#include <bfd.h>],[
bfd_init();
]),_found_libbfd=yes,_found_libbfd=no)

     AC_MSG_RESULT([$_found_libbfd])

     if test $_found_libbfd = yes ; then
        AC_DEFINE(HAVE_LIBBFD,1,
	   [Define to 1 if you have a fully functional libbfd library.])
        AC_SUBST(LIBBFD_CPPFLAGS,$_libbfd_try_cflags)
        AC_SUBST(LIBBFD,$_libbfd_try_libs)
        AC_CHECK_FUNCS(bfd_init)
     fi

     LIBS=$_libbfd_save_libs
     CFLAGS=$_libbfd_save_cflags

     unset _libbfd_save_libs
     unset _libbfd_save_cflags
     unset _libbfd_try_libs
     unset _libbfd_try_cflags
     unset _found_libbfd
  fi
])dnl