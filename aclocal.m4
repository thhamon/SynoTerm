dnl aclocal.m4 generated automatically by aclocal 1.4a

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

# Like AC_CONFIG_HEADER, but automatically create stamp file.

AC_DEFUN(AM_CONFIG_HEADER,
[AC_PREREQ([2.12])
AC_CONFIG_HEADER([$1])
dnl When config.status generates a header, we must update the stamp-h file.
dnl This file resides in the same directory as the config header
dnl that is generated.  We must strip everything past the first ":",
dnl and everything past the last "/".
AC_OUTPUT_COMMANDS(changequote(<<,>>)dnl
ifelse(patsubst(<<$1>>, <<[^ ]>>, <<>>), <<>>,
<<test -z "<<$>>CONFIG_HEADERS" || echo timestamp > patsubst(<<$1>>, <<^\([^:]*/\)?.*>>, <<\1>>)stamp-h<<>>dnl>>,
<<am_indx=1
for am_file in <<$1>>; do
  case " <<$>>CONFIG_HEADERS " in
  *" <<$>>am_file "*<<)>>
    echo timestamp > `echo <<$>>am_file | sed -e 's%:.*%%' -e 's%[^/]*$%%'`stamp-h$am_indx
    ;;
  esac
  am_indx=`expr "<<$>>am_indx" + 1`
done<<>>dnl>>)
changequote([,]))])

# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 1

dnl Usage:
dnl AM_INIT_AUTOMAKE(package,version, [no-define])

AC_DEFUN(AM_INIT_AUTOMAKE,
[AC_REQUIRE([AC_PROG_INSTALL])
dnl We require 2.13 because we rely on SHELL being computed by configure.
AC_PREREQ([2.13])
PACKAGE=[$1]
AC_SUBST(PACKAGE)
VERSION=[$2]
AC_SUBST(VERSION)
dnl test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" && test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi
ifelse([$3],,
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package]))
AC_REQUIRE([AM_SANITY_CHECK])
AC_REQUIRE([AC_ARG_PROGRAM])
dnl FIXME This is truly gross.
missing_dir=`cd $ac_aux_dir && pwd`
AM_MISSING_PROG(ACLOCAL, aclocal, $missing_dir)
AM_MISSING_PROG(AUTOCONF, autoconf, $missing_dir)
AM_MISSING_PROG(AUTOMAKE, automake, $missing_dir)
AM_MISSING_PROG(AUTOHEADER, autoheader, $missing_dir)
AM_MISSING_PROG(MAKEINFO, makeinfo, $missing_dir)
AC_REQUIRE([AC_PROG_MAKE_SET])])

#
# Check to make sure that the build environment is sane.
#

# serial 1

dnl We don't use AC_REQUIRE here in order to have the message displayed
dnl before.  Moreover, few people would run AM_SANITY_CHECKs at hand.
AC_DEFUN(AM_SANITY_CHECK,
[AC_MSG_CHECKING([whether build environment is sane])
AM_SANITY_CHECK_CDPATH
AM_SANITY_CHECK_TIME
AC_MSG_RESULT(yes)])

dnl Verify that we can safely run `cd DIR && pwd'.  Some `cd' output
dnl the directory name when CDPATH is set.
AC_DEFUN(AM_SANITY_CHECK_CDPATH,
[if test -n "`cd /`"; then
  AC_MSG_ERROR([Running cd is not silent as it should be.
Make sure to unset the environment variable CDPATH.])
fi])

AC_DEFUN(AM_SANITY_CHECK_TIME,
[# Just in case
sleep 1
echo timestamp > conftestfile
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftestfile 2> /dev/null`
   if test "[$]*" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftestfile`
   fi
   if test "[$]*" != "X $srcdir/configure conftestfile" \
      && test "[$]*" != "X conftestfile $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment.])
   fi

   test "[$]2" = conftestfile
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock.])
fi
rm -f conftest*])

dnl AM_MISSING_PROG(NAME, PROGRAM, DIRECTORY)
dnl The program must properly implement --version.
AC_DEFUN(AM_MISSING_PROG,
[AC_MSG_CHECKING(for working $2)
# Run test in a subshell; some versions of sh will print an error if
# an executable is not found, even if stderr is redirected.
# Redirect stdin to placate older versions of autoconf.  Sigh.
if ($2 --version) < /dev/null > /dev/null 2>&1; then
   $1=$2
   AC_MSG_RESULT(found)
else
   $1="$3/missing $2"
   AC_MSG_RESULT(missing)
fi
AC_SUBST($1)])

#serial 1

dnl From Jim Meyering.
dnl Find a new-enough version of Perl.
dnl

AC_DEFUN(jm_PERL,
[
  dnl FIXME: don't hard-code 5.003
  dnl FIXME: should we cache the result?
  AC_MSG_CHECKING([for perl5.003 or newer])
  if test "${PERL+set}" = set; then
    # `PERL' is set in the user's environment.
    candidate_perl_names="$PERL"
    perl_specified=yes
  else
    candidate_perl_names='perl perl5'
    perl_specified=no
  fi

  found=no
  AC_SUBST(PERL)
  PERL="$missing_dir/missing perl"
  for perl in $candidate_perl_names; do
    # Run test in a subshell; some versions of sh will print an error if
    # an executable is not found, even if stderr is redirected.
    if ( $perl -e 'require 5.003' ) > /dev/null 2>&1; then
      PERL=$perl
      found=yes
      break
    fi
  done

  AC_MSG_RESULT($found)
  test $found = no && AC_MSG_WARN([
*** You don't seem to have perl5.003 or newer installed.
*** Because of that, you may be unable to regenerate certain files
*** if you modify the sources from which they are derived.] )
])

dnl The same, but the path to perl is wanted.
dnl
dnl By Akim Demaille, do not flame Jim for this :)
AC_DEFUN(jm_PATH_PERL,
[
  dnl FIXME: don't hard-code 5.003
  dnl FIXME: should we cache the result?
  if test "${PERL+set}" = set; then
    # `PERL' is set in the user's environment.
    candidate_perl_names="$PERL"
    perl_specified=yes
  else
    candidate_perl_names='perl perl5'
    perl_specified=no
  fi

  found=no
  for perl_name in $candidate_perl_names; do
    # Run test in a subshell; some versions of sh will print an error if
    # an executable is not found, even if stderr is redirected.
    AC_PATH_PROG(PERL, $perl_name, no)
    if test "$PERL" != "no" \
       && ( $PERL -e 'require 5.003' ) > /dev/null 2>&1; then
      found=yes
      break
    fi
  done

  if test $found = no; then
    PERL="$missing_dir/missing perl"
    AC_SUBST(PERL)
    AC_MSG_WARN([
*** You don't seem to have perl5.003 or newer installed.
*** Because of that, you may be unable to regenerate certain files
*** if you modify the sources from which they are derived.] )
  fi
])


# serial 1

# @defmac AC_PROG_CC_STDC
# @maindex PROG_CC_STDC
# @ovindex CC
# If the C compiler in not in ANSI C mode by default, try to add an option
# to output variable @code{CC} to make it so.  This macro tries various
# options that select ANSI C on some system or another.  It considers the
# compiler to be in ANSI C mode if it handles function prototypes correctly.
#
# If you use this macro, you should check after calling it whether the C
# compiler has been set to accept ANSI C; if not, the shell variable
# @code{am_cv_prog_cc_stdc} is set to @samp{no}.  If you wrote your source
# code in ANSI C, you can make an un-ANSIfied copy of it by using the
# program @code{ansi2knr}, which comes with Ghostscript.
# @end defmac

AC_DEFUN(AM_PROG_CC_STDC,
[AC_REQUIRE([AC_PROG_CC])
AC_BEFORE([$0], [AC_C_INLINE])
AC_BEFORE([$0], [AC_C_CONST])
dnl Force this before AC_PROG_CPP.  Some cpp's, eg on HPUX, require
dnl a magic option to avoid problems with ANSI preprocessor commands
dnl like #elif.
dnl FIXME: can't do this because then AC_AIX won't work due to a
dnl circular dependency.
dnl AC_BEFORE([$0], [AC_PROG_CPP])
AC_MSG_CHECKING(for ${CC-cc} option to accept ANSI C)
AC_CACHE_VAL(am_cv_prog_cc_stdc,
[am_cv_prog_cc_stdc=no
ac_save_CC="$CC"
# Don't try gcc -ansi; that turns off useful extensions and
# breaks some systems' header files.
# AIX			-qlanglvl=ansi
# Ultrix and OSF/1	-std1
# HP-UX 10.20 and later	-Ae
# HP-UX older versions	-Aa -D_HPUX_SOURCE
# SVR4			-Xc -D__EXTENSIONS__
for ac_arg in "" -qlanglvl=ansi -std1 -Ae "-Aa -D_HPUX_SOURCE" "-Xc -D__EXTENSIONS__"
do
  CC="$ac_save_CC $ac_arg"
  AC_TRY_COMPILE(
[#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
/* Most of the following tests are stolen from RCS 5.7's src/conf.sh.  */
struct buf { int x; };
FILE * (*rcsopen) (struct buf *, struct stat *, int);
static char *e (p, i)
     char **p;
     int i;
{
  return p[i];
}
static char *f (char * (*g) (char **, int), char **p, ...)
{
  char *s;
  va_list v;
  va_start (v,p);
  s = g (p, va_arg (v,int));
  va_end (v);
  return s;
}
int test (int i, double x);
struct s1 {int (*f) (int a);};
struct s2 {int (*f) (double a);};
int pairnames (int, char **, FILE *(*)(struct buf *, struct stat *, int), int, int);
int argc;
char **argv;
], [
return f (e, argv, 0) != argv[0]  ||  f (e, argv, 1) != argv[1];
],
[am_cv_prog_cc_stdc="$ac_arg"; break])
done
CC="$ac_save_CC"
])
if test -z "$am_cv_prog_cc_stdc"; then
  AC_MSG_RESULT([none needed])
else
  AC_MSG_RESULT($am_cv_prog_cc_stdc)
fi
case "x$am_cv_prog_cc_stdc" in
  x|xno) ;;
  *) CC="$CC $am_cv_prog_cc_stdc" ;;
esac
])


# serial 2

AC_DEFUN(AM_C_PROTOTYPES,
[AC_REQUIRE([AM_PROG_CC_STDC])
AC_REQUIRE([AC_PROG_CPP])
AC_MSG_CHECKING([for function prototypes])
# Ensure some checks needed by ansi2knr itself.
AC_HEADER_STDC
AC_CHECK_HEADERS(string.h)
if test "$am_cv_prog_cc_stdc" != no; then
  AC_MSG_RESULT(yes)
  AC_DEFINE(PROTOTYPES,1,[Define if compiler has function prototypes])
  U= ANSI2KNR=
else
  AC_MSG_RESULT(no)
  U=_ ANSI2KNR=./ansi2knr
fi
AC_SUBST(U)dnl
AC_SUBST(ANSI2KNR)dnl
])


# serial 2

dnl AM_PROG_FLEX
dnl Look for flex or missing, then run AC_PROG_LEX and AC_DECL_YYTEXT
AC_DEFUN(AM_PROG_FLEX,
[AC_CHECK_PROGS(LEX, flex, missing)
if test "$LEX" = missing; then
  LEX="\$(top_srcdir)/$ac_aux_dir/missing flex"
  LEX_OUTPUT_ROOT=lex.yy
  AC_SUBST(LEX_OUTPUT_ROOT)dnl
else
  AC_PROG_LEX
  AC_DECL_YYTEXT
fi])

# Once this macro is called, you may output with no echo in a Makefile or
# script using:  echo @ECHO_N@ "STRING_TO_OUTPUT@ECHO_C@".

AC_DEFUN(fp_PROG_ECHO,
[AC_MSG_CHECKING(how to suppress newlines using echo)
AC_CACHE_VAL(fp_cv_prog_echo_nonl,
[if (echo "testing\c"; echo 1,2,3) | grep c >/dev/null; then
  if (echo -n testing; echo 1,2,3) | sed s/-n/xn/ | grep xn >/dev/null; then
    fp_cv_prog_echo_nonl=no
  else
    fp_cv_prog_echo_nonl=option
  fi
else
  fp_cv_prog_echo_nonl=escape
fi
])
AC_MSG_RESULT($fp_cv_prog_echo_nonl)
test $fp_cv_prog_echo_nonl = no \
  && echo 2>&1 "WARNING: \`echo' not powerful enough for \`make check'"
case $fp_cv_prog_echo_nonl in
  no) ECHO_N= ECHO_C= ;;
  option) ECHO_N=-n ECHO_C= ;;
  escape) ECHO_N= ECHO_C='\c' ;;
esac
AC_SUBST(ECHO_N)dnl
AC_SUBST(ECHO_C)dnl
])


# serial 29 AM_PROG_LIBTOOL
AC_DEFUN(AM_PROG_LIBTOOL,
[AC_REQUIRE([AM_ENABLE_SHARED])dnl
AC_REQUIRE([AM_ENABLE_STATIC])dnl
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_CANONICAL_BUILD])dnl
AC_REQUIRE([AC_PROG_RANLIB])dnl
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AM_PROG_LD])dnl
AC_REQUIRE([AM_PROG_NM])dnl
AC_REQUIRE([AC_PROG_LN_S])dnl
dnl
# Always use our own libtool.
LIBTOOL='$(SHELL) $(top_builddir)/libtool'
AC_SUBST(LIBTOOL)dnl

# Check for any special flags to pass to ltconfig.
libtool_flags=
test "$enable_shared" = no && libtool_flags="$libtool_flags --disable-shared"
test "$enable_static" = no && libtool_flags="$libtool_flags --disable-static"
test "$silent" = yes && libtool_flags="$libtool_flags --silent"
test "$ac_cv_prog_gcc" = yes && libtool_flags="$libtool_flags --with-gcc"
test "$ac_cv_prog_gnu_ld" = yes && libtool_flags="$libtool_flags --with-gnu-ld"

# Some flags need to be propagated to the compiler or linker for good
# libtool support.
case "$host" in
*-*-irix6*)
  # Find out which ABI we are using.
  echo '[#]line __oline__ "configure"' > conftest.$ac_ext
  if AC_TRY_EVAL(ac_compile); then
    case "`/usr/bin/file conftest.o`" in
    *32-bit*)
      LD="${LD-ld} -32"
      ;;
    *N32*)
      LD="${LD-ld} -n32"
      ;;
    *64-bit*)
      LD="${LD-ld} -64"
      ;;
    esac
  fi
  rm -rf conftest*
  ;;

*-*-sco3.2v5*)
  # On SCO OpenServer 5, we need -belf to get full-featured binaries.
  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -belf"
  AC_CACHE_CHECK([whether the C compiler needs -belf], lt_cv_cc_needs_belf,
    [AC_TRY_LINK([],[],[lt_cv_cc_needs_belf=yes],[lt_cv_cc_needs_belf=no])])
  if test x"$lt_cv_cc_needs_belf" != x"yes"; then
    # this is probably gcc 2.8.0, egcs 1.0 or newer; no need for -belf
    CFLAGS="$SAVE_CFLAGS"
  fi
  ;;

*-*-cygwin32*)
  AM_SYS_LIBTOOL_CYGWIN32
  ;;

esac

# enable the --disable-libtool-lock switch

AC_ARG_ENABLE(libtool-lock,
[  --disable-libtool-lock  force libtool not to do file locking],
need_locks=$enableval,
need_locks=yes)

if test x"$need_locks" = xno; then
  libtool_flags="$libtool_flags --disable-lock"
fi


# Actually configure libtool.  ac_aux_dir is where install-sh is found.
CC="$CC" CFLAGS="$CFLAGS" CPPFLAGS="$CPPFLAGS" \
LD="$LD" NM="$NM" RANLIB="$RANLIB" LN_S="$LN_S" \
DLLTOOL="$DLLTOOL" AS="$AS" \
${CONFIG_SHELL-/bin/sh} $ac_aux_dir/ltconfig --no-reexec \
$libtool_flags --no-verify $ac_aux_dir/ltmain.sh $host \
|| AC_MSG_ERROR([libtool configure failed])

# Redirect the config.log output again, so that the ltconfig log is not
# clobbered by the next message.
exec 5>>./config.log
])

# AM_ENABLE_SHARED - implement the --enable-shared flag
# Usage: AM_ENABLE_SHARED[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AM_ENABLE_SHARED,
[define([AM_ENABLE_SHARED_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(shared,
changequote(<<, >>)dnl
<<  --enable-shared[=PKGS]  build shared libraries [default=>>AM_ENABLE_SHARED_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_shared=yes ;;
no) enable_shared=no ;;
*)
  enable_shared=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_shared=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_shared=AM_ENABLE_SHARED_DEFAULT)dnl
])

# AM_DISABLE_SHARED - set the default shared flag to --disable-shared
AC_DEFUN(AM_DISABLE_SHARED,
[AM_ENABLE_SHARED(no)])

# AM_DISABLE_STATIC - set the default static flag to --disable-static
AC_DEFUN(AM_DISABLE_STATIC,
[AM_ENABLE_STATIC(no)])

# AM_ENABLE_STATIC - implement the --enable-static flag
# Usage: AM_ENABLE_STATIC[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AM_ENABLE_STATIC,
[define([AM_ENABLE_STATIC_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(static,
changequote(<<, >>)dnl
<<  --enable-static[=PKGS]  build static libraries [default=>>AM_ENABLE_STATIC_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_static=yes ;;
no) enable_static=no ;;
*)
  enable_static=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_static=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_static=AM_ENABLE_STATIC_DEFAULT)dnl
])


# AM_PROG_LD - find the path to the GNU or non-GNU linker
AC_DEFUN(AM_PROG_LD,
[AC_ARG_WITH(gnu-ld,
[  --with-gnu-ld           assume the C compiler uses GNU ld [default=no]],
test "$withval" = no || with_gnu_ld=yes, with_gnu_ld=no)
AC_REQUIRE([AC_PROG_CC])
ac_prog=ld
if test "$ac_cv_prog_gcc" = yes; then
  # Check if gcc -print-prog-name=ld gives a path.
  AC_MSG_CHECKING([for ld used by GCC])
  ac_prog=`($CC -print-prog-name=ld) 2>&5`
  case "$ac_prog" in
  # Accept absolute paths.
changequote(,)dnl
  /* | [A-Za-z]:\\*)
changequote([,])dnl
    test -z "$LD" && LD="$ac_prog"
    ;;
  "")
    # If it fails, then pretend we aren't using GCC.
    ac_prog=ld
    ;;
  *)
    # If it is relative, then search for the first ld in PATH.
    with_gnu_ld=unknown
    ;;
  esac
elif test "$with_gnu_ld" = yes; then
  AC_MSG_CHECKING([for GNU ld])
else
  AC_MSG_CHECKING([for non-GNU ld])
fi
AC_CACHE_VAL(ac_cv_path_LD,
[if test -z "$LD"; then
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  for ac_dir in $PATH; do
    test -z "$ac_dir" && ac_dir=.
    if test -f "$ac_dir/$ac_prog"; then
      ac_cv_path_LD="$ac_dir/$ac_prog"
      # Check to see if the program is GNU ld.  I'd rather use --version,
      # but apparently some GNU ld's only accept -v.
      # Break only if it was the GNU/non-GNU ld that we prefer.
      if "$ac_cv_path_LD" -v 2>&1 < /dev/null | egrep '(GNU|with BFD)' > /dev/null; then
	test "$with_gnu_ld" != no && break
      else
        test "$with_gnu_ld" != yes && break
      fi
    fi
  done
  IFS="$ac_save_ifs"
else
  ac_cv_path_LD="$LD" # Let the user override the test with a path.
fi])
LD="$ac_cv_path_LD"
if test -n "$LD"; then
  AC_MSG_RESULT($LD)
else
  AC_MSG_RESULT(no)
fi
test -z "$LD" && AC_MSG_ERROR([no acceptable ld found in \$PATH])
AC_SUBST(LD)
AM_PROG_LD_GNU
])

AC_DEFUN(AM_PROG_LD_GNU,
[AC_CACHE_CHECK([if the linker ($LD) is GNU ld], ac_cv_prog_gnu_ld,
[# I'd rather use --version here, but apparently some GNU ld's only accept -v.
if $LD -v 2>&1 </dev/null | egrep '(GNU|with BFD)' 1>&5; then
  ac_cv_prog_gnu_ld=yes
else
  ac_cv_prog_gnu_ld=no
fi])
])

# AM_PROG_NM - find the path to a BSD-compatible name lister
AC_DEFUN(AM_PROG_NM,
[AC_MSG_CHECKING([for BSD-compatible nm])
AC_CACHE_VAL(ac_cv_path_NM,
[if test -n "$NM"; then
  # Let the user override the test.
  ac_cv_path_NM="$NM"
else
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  for ac_dir in /usr/ucb /usr/ccs/bin $PATH /bin; do
    test -z "$ac_dir" && ac_dir=.
    if test -f $ac_dir/nm; then
      # Check to see if the nm accepts a BSD-compat flag.
      # Adding the `sed 1q' prevents false positives on HP-UX, which says:
      #   nm: unknown option "B" ignored
      if ($ac_dir/nm -B /dev/null 2>&1 | sed '1q'; exit 0) | egrep /dev/null >/dev/null; then
        ac_cv_path_NM="$ac_dir/nm -B"
      elif ($ac_dir/nm -p /dev/null 2>&1 | sed '1q'; exit 0) | egrep /dev/null >/dev/null; then
        ac_cv_path_NM="$ac_dir/nm -p"
      else
        ac_cv_path_NM="$ac_dir/nm"
      fi
      break
    fi
  done
  IFS="$ac_save_ifs"
  test -z "$ac_cv_path_NM" && ac_cv_path_NM=nm
fi])
NM="$ac_cv_path_NM"
AC_MSG_RESULT([$NM])
AC_SUBST(NM)
])

# AM_SYS_LIBTOOL_CYGWIN32 - find tools needed on cygwin32
AC_DEFUN(AM_SYS_LIBTOOL_CYGWIN32,
[AC_CHECK_TOOL(DLLTOOL, dlltool, false)
AC_CHECK_TOOL(AS, as, false)
])

dnl ## --------------------------------------------------------- ##
dnl ##  Check if ps_gecos is in struct passwd                    ##
dnl ##                           demaille@inf.enst.fr            ##
dnl ## --------------------------------------------------------- ##
dnl
dnl
dnl acconfig.h should contains these lines
dnl
dnl /* Define if your struct passwd has pw_gecos.  */
dnl #undef HAVE_PW_GECOS

# serial 1
# If struct passwd contains pw_gecos define HAVE_PW_GECOS
AC_DEFUN(ad_STRUCT_PW_GECOS,
[AC_CACHE_CHECK([for pw_gecos in struct passwd], ac_cv_struct_pw_gecos,
[AC_TRY_COMPILE([#include <pwd.h>], [struct passwd s; s.pw_gecos;],
ac_cv_struct_pw_gecos=yes, ac_cv_struct_pw_gecos=no)])
if test $ac_cv_struct_pw_gecos = yes; then
  AC_DEFINE(HAVE_PW_GECOS, 1,
	    [Define if your struct passwd has pw_gecos.])
fi
])

dnl --- *@-mdw_CHECK_MANYLIBS-@* ---
dnl
dnl Author:	Mark Wooding
dnl
dnl Synopsis:	mdw_CHECK_MANYLIBS(FUNC, LIBS, [IF-FOUND], [IF-NOT-FOUND])
dnl
dnl Arguments:	FUNC = a function to try to find
dnl		LIBS = a whitespace-separated list of libraries to search
dnl		IF-FOUND = what to do when the function is found
dnl		IF-NOT-FOUND = what to do when the function isn't found
dnl
dnl Use:	Searches for a library which defines FUNC.  It first tries
dnl		without any libraries; then it tries each library specified
dnl		in LIBS in turn.  If it finds a match, it adds the
dnl		appropriate library to `LIBS'.
dnl
dnl		This is particularly handy under DIREIX: if you link with
dnl		`-lnsl' then you get non-NIS-aware versions of getpwnam and
dnl		so on, which is clearly a Bad Thing.

AC_DEFUN(mdw_CHECK_MANYLIBS,
[AC_CACHE_CHECK([for library containing $1], [mdw_cv_lib_$1],
[mdw_save_LIBS="$LIBS"
mdw_cv_lib_$1="no"
AC_TRY_LINK(,[$1()], [mdw_cv_lib_$1="none required"])
test "$mdw_cv_lib_$1" = "no" && for i in $2; do
LIBS="-l$i $mdw_save_LIBS"
AC_TRY_LINK(,[$1()],
[mdw_cv_lib_$1="-l$i"
break])
done
LIBS="$mdw_save_LIBS"])
if test "$mdw_cv_lib_$1" != "no"; then
  test "$mdw_cv_lib_$1" = "none required" || LIBS="$mdw_cv_lib_$1 $LIBS"
  $3
else :
  $4
fi])


dnl strcasecmp.c and/or strncasecmp should be available.

# serial 1

AC_DEFUN(ad_REPLACE_FUNC_STRCASECMP,
[AC_CHECK_FUNCS(strcasecmp)
 if test $ac_cv_func_strcasecmp = no; then
   AC_CHECK_FUNCS(stricmp)
   if test $ac_cv_func_stricmp = no; then
     LIBOBJS="$LIBOBJS strcasecmp.o"
   else
     AC_DEFINE_UNQUOTED(strcasecmp, stricmp,
	  [Define to the replacement function if should be used.])
   fi
 fi])

AC_DEFUN(ad_REPLACE_FUNC_STRNCASECMP,
[AC_CHECK_FUNCS(strncasecmp)
 if test $ac_cv_func_strncasecmp = no; then
   AC_CHECK_FUNCS(strnicmp)
   if test $ac_cv_func_strnicmp = no; then
     LIBOBJS="$LIBOBJS strncasecmp.o"
   else
     AC_DEFINE_UNQUOTED(strncasecmp, strnicmp,
	  [Define to the replacement function if should be used.])
   fi
 fi])


dnl rename.c should be available

# serial 1

AC_DEFUN(ad_CHECK_FUNC_RENAME,
[AC_CHECK_FUNCS(rename)
if test $ac_cv_func_rename = yes; then
  AC_CACHE_CHECK([whether rename works], ac_cv_func_rename_works,
  [ac_cv_func_rename=no
   : >conftest.1
   : >conftest.2
   AC_TRY_RUN([#include <stdio.h>
int main () { exit (rename ("conftest.1", "conftest.2")); }],
   test -f conftest.1 || ac_cv_func_rename_works=yes,,:)])
fi])

AC_DEFUN(ad_REPLACE_FUNC_RENAME,
[AC_REQUIRE(ad_CHECK_FUNC_RENAME)
AC_REQUIRE(AC_HEADER_STAT)
if test "$ac_cv_func_rename" = no ||
   test "$ac_cv_func_rename_works" = no; then
  AC_CHECK_FUNCS(link)
  dnl FIXME: Define MVDIR
  LIBOBJS="$LIBOBJS rename.o"
  AC_DEFINE_UNQUOTED(rename, rpl_rename,
   [Define to the replacement function if should be used.])
fi])

dnl ## --------------------------------------------------------- ##
dnl ##  Check if libpaper is available                           ##
dnl ##                           demaille@inf.enst.fr            ##
dnl ## --------------------------------------------------------- ##
dnl
dnl

# serial 1

AC_DEFUN(ad_FUNC_SYSTEMPAPERNAME,
[
  AC_CHECK_LIB(paper, systempapername, 
    dnl Action if found
    [
      AC_DEFINE(HAVE_SYSTEMPAPERNAME, 1,
                [Define if you have the systempapername function])
      LIBS="$LIBS -lpaper"
      AC_CHECK_HEADERS(paper.h)
    ])
])

dnl
dnl acconfig.h should contains these two lines
dnl /* Define to rpl_exit if the replacement function should be used.  */
dnl #undef exit
dnl and atexit.c should be available

# serial 2

AC_DEFUN(ad_FUNC_ATEXIT,
  [AC_CHECK_FUNCS(atexit)
   if test $ac_cv_func_atexit = no; then
     AC_CHECK_FUNCS(on_exit)
     LIBOBJS="$LIBOBJS atexit.o"
     if test $ac_cv_func_on_exit = no; then
       AC_DEFINE_UNQUOTED(exit, rpl_exit,
			  [Define to rpl_exit if the replacement function should be used])
     fi
   fi])


# serial 1

AC_DEFUN(ad_FUNC_STRFTIME,
[  AC_FUNC_STRFTIME
  AC_STRUCT_TIMEZONE
  AC_CHECK_FUNCS(tzset mblen mbrlen)
  AC_CHECK_HEADERS(limits.h)
  AM_FUNC_MKTIME
  if test $ac_cv_func_strftime = no; then
    LIBOBJS="$LIBOBJS strftime.o"
    if test $am_cv_func_working_mktime = no; then
       AC_DEFINE_UNQUOTED(mktime, rpl_mktime,
	[Define to rpl_mktime if the replacement function should be used.])
    fi
  fi])

#serial 4

dnl From Jim Meyering.
dnl FIXME: this should migrate into libit.

AC_DEFUN(AM_FUNC_MKTIME,
[AC_REQUIRE([AC_HEADER_TIME])dnl
 AC_CHECK_HEADERS(sys/time.h unistd.h)
 AC_CHECK_FUNCS(alarm)
 AC_CACHE_CHECK([for working mktime], am_cv_func_working_mktime,
  [AC_TRY_RUN(
changequote(<<, >>)dnl
<</* Test program from Paul Eggert (eggert@twinsun.com)
   and Tony Leneis (tony@plaza.ds.adp.com).  */
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#if !HAVE_ALARM
# define alarm(X) /* empty */
#endif

/* Work around redefinition to rpl_putenv by other config tests.  */
#undef putenv

static time_t time_t_max;

/* Values we'll use to set the TZ environment variable.  */
static const char *const tz_strings[] = {
  (const char *) 0, "TZ=GMT0", "TZ=JST-9",
  "TZ=EST+3EDT+2,M10.1.0/00:00:00,M2.3.0/00:00:00"
};
#define N_STRINGS (sizeof (tz_strings) / sizeof (tz_strings[0]))

/* Fail if mktime fails to convert a date in the spring-forward gap.
   Based on a problem report from Andreas Jaeger.  */
static void
spring_forward_gap ()
{
  /* glibc (up to about 1998-10-07) failed this test) */
  struct tm tm;

  /* Use the portable POSIX.1 specification "TZ=PST8PDT,M4.1.0,M10.5.0"
     instead of "TZ=America/Vancouver" in order to detect the bug even
     on systems that don't support the Olson extension, or don't have the
     full zoneinfo tables installed.  */
  putenv ("TZ=PST8PDT,M4.1.0,M10.5.0");

  tm.tm_year = 98;
  tm.tm_mon = 3;
  tm.tm_mday = 5;
  tm.tm_hour = 2;
  tm.tm_min = 0;
  tm.tm_sec = 0;
  tm.tm_isdst = -1;
  if (mktime (&tm) == (time_t)-1)
    exit (1);
}

static void
mktime_test (now)
     time_t now;
{
  struct tm *lt;
  if ((lt = localtime (&now)) && mktime (lt) != now)
    exit (1);
  now = time_t_max - now;
  if ((lt = localtime (&now)) && mktime (lt) != now)
    exit (1);
}

static void
irix_6_4_bug ()
{
  /* Based on code from Ariel Faigon.  */
  struct tm tm;
  tm.tm_year = 96;
  tm.tm_mon = 3;
  tm.tm_mday = 0;
  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;
  tm.tm_isdst = -1;
  mktime (&tm);
  if (tm.tm_mon != 2 || tm.tm_mday != 31)
    exit (1);
}

static void
bigtime_test (j)
     int j;
{
  struct tm tm;
  time_t now;
  tm.tm_year = tm.tm_mon = tm.tm_mday = tm.tm_hour = tm.tm_min = tm.tm_sec = j;
  /* This test makes some buggy mktime implementations loop.
     Give up after 10 seconds.  */
  alarm (10);
  now = mktime (&tm);
  alarm (0);
  if (now != (time_t) -1)
    {
      struct tm *lt = localtime (&now);
      if (! (lt
	     && lt->tm_year == tm.tm_year
	     && lt->tm_mon == tm.tm_mon
	     && lt->tm_mday == tm.tm_mday
	     && lt->tm_hour == tm.tm_hour
	     && lt->tm_min == tm.tm_min
	     && lt->tm_sec == tm.tm_sec
	     && lt->tm_yday == tm.tm_yday
	     && lt->tm_wday == tm.tm_wday
	     && ((lt->tm_isdst < 0 ? -1 : 0 < lt->tm_isdst)
		  == (tm.tm_isdst < 0 ? -1 : 0 < tm.tm_isdst))))
	exit (1);
    }
}

int
main ()
{
  time_t t, delta;
  int i, j;

  for (time_t_max = 1; 0 < time_t_max; time_t_max *= 2)
    continue;
  time_t_max--;
  delta = time_t_max / 997; /* a suitable prime number */
  for (i = 0; i < N_STRINGS; i++)
    {
      if (tz_strings[i])
	putenv (tz_strings[i]);

      for (t = 0; t <= time_t_max - delta; t += delta)
	mktime_test (t);
      mktime_test ((time_t) 60 * 60);
      mktime_test ((time_t) 60 * 60 * 24);

      for (j = 1; 0 < j; j *= 2)
        bigtime_test (j);
      bigtime_test (j - 1);
    }
  irix_6_4_bug ();
  spring_forward_gap ();
  exit (0);
}
	      >>,
changequote([, ])dnl
	     am_cv_func_working_mktime=yes, am_cv_func_working_mktime=no,
	     dnl When crosscompiling, assume mktime is missing or broken.
	     am_cv_func_working_mktime=no)
  ])
  if test $am_cv_func_working_mktime = no; then
    LIBOBJS="$LIBOBJS mktime.o"
  fi
])

dnl From Jim Meyering.
dnl FIXME: migrate into libit.

AC_DEFUN(AM_FUNC_OBSTACK,
[AC_CACHE_CHECK([for obstacks], am_cv_func_obstack,
 [AC_TRY_LINK([#include "obstack.h"],
	      [struct obstack *mem;obstack_free(mem,(char *) 0)],
	      am_cv_func_obstack=yes,
	      am_cv_func_obstack=no)])
 if test $am_cv_func_obstack = yes; then
   AC_DEFINE(HAVE_OBSTACK,1,[Define if libc includes obstacks])
 else
   LIBOBJS="$LIBOBJS obstack.o"
 fi
])

dnl From Jim Meyering.  Use this if you use the GNU error.[ch].
dnl FIXME: Migrate into libit

AC_DEFUN(AM_FUNC_ERROR_AT_LINE,
[AC_CACHE_CHECK([for error_at_line], am_cv_lib_error_at_line,
 [AC_TRY_LINK([],[error_at_line(0, 0, "", 0, "");],
              am_cv_lib_error_at_line=yes,
	      am_cv_lib_error_at_line=no)])
 if test $am_cv_lib_error_at_line = no; then
   LIBOBJS="$LIBOBJS error.o"
 fi
 AC_SUBST(LIBOBJS)dnl
])

dnl Make all the needed checks to use tinyterm.[ch]
dnl Akim.Demaille@inf.enst.fr

#serial 2

AC_DEFUN(ad_TINYTERM_CHECKS,
[AC_REQUIRE([AM_HEADER_TIOCGWINSZ_NEEDS_SYS_IOCTL])
 AC_CHECK_HEADERS(termios.h sys/types.h unistd.h limits.h)
 AC_REQUIRE([jm_AC_HEADER_INTTYPES_H])
])

dnl From Jim Meyering.

# serial 1

AC_DEFUN(AM_HEADER_TIOCGWINSZ_NEEDS_SYS_IOCTL,
[AC_REQUIRE([AM_SYS_POSIX_TERMIOS])
 AC_CACHE_CHECK([whether use of TIOCGWINSZ requires sys/ioctl.h],
	        am_cv_sys_tiocgwinsz_needs_sys_ioctl_h,
  [am_cv_sys_tiocgwinsz_needs_sys_ioctl_h=no

  gwinsz_in_termios_h=no
  if test $am_cv_sys_posix_termios = yes; then
    AC_EGREP_CPP([yes],
    [#include <sys/types.h>
#     include <termios.h>
#     ifdef TIOCGWINSZ
        yes
#     endif
    ], gwinsz_in_termios_h=yes)
  fi

  if test $gwinsz_in_termios_h = no; then
    AC_EGREP_CPP([yes],
    [#include <sys/types.h>
#     include <sys/ioctl.h>
#     ifdef TIOCGWINSZ
        yes
#     endif
    ], am_cv_sys_tiocgwinsz_needs_sys_ioctl_h=yes)
  fi
  ])
  if test $am_cv_sys_tiocgwinsz_needs_sys_ioctl_h = yes; then
    AC_DEFINE(GWINSZ_IN_SYS_IOCTL,1,
              [Define if TIOCGWINSZ requires sys/ioctl.h])
  fi
])

dnl From Jim Meyering.

# serial 1

AC_DEFUN(AM_SYS_POSIX_TERMIOS,
[AC_CACHE_CHECK([POSIX termios], am_cv_sys_posix_termios,
  [AC_TRY_LINK([#include <sys/types.h>
#include <unistd.h>
#include <termios.h>],
  [/* SunOS 4.0.3 has termios.h but not the library calls.  */
   tcgetattr(0, 0);],
  am_cv_sys_posix_termios=yes,
  am_cv_sys_posix_termios=no)])
])

#serial 3

dnl From Paul Eggert.

# Define HAVE_INTTYPES_H if <inttypes.h> exists,
# doesn't clash with <sys/types.h>, and declares uintmax_t.

AC_DEFUN(jm_AC_HEADER_INTTYPES_H,
[
  AC_CACHE_CHECK([for inttypes.h], jm_ac_cv_header_inttypes_h,
  [AC_TRY_COMPILE(
    [#include <sys/types.h>
#include <inttypes.h>],
    [uintmax_t i = (uintmax_t) -1;],
    jm_ac_cv_header_inttypes_h=yes,
    jm_ac_cv_header_inttypes_h=no)])
  if test $jm_ac_cv_header_inttypes_h = yes; then
    AC_DEFINE_UNQUOTED(HAVE_INTTYPES_H, 1,
[Define if <inttypes.h> exists, doesn't clash with <sys/types.h>,
   and declares uintmax_t. ])
  fi
])

#serial 4

AC_DEFUN(jm_CHECK_DECLARATION,
[
  AC_REQUIRE([AC_HEADER_STDC])dnl
  AC_CHECK_HEADERS(memory.h, string.h, strings.h, stdlib.h, unistd.h)
  AC_MSG_CHECKING([whether $1 is declared])
  AC_CACHE_VAL(jm_cv_func_decl_$1,
    [AC_TRY_COMPILE($2,
      [
#ifndef $1
char *(*pfn) = (char *(*)) $1
#endif
      ],
      eval "jm_cv_func_decl_$1=yes",
      eval "jm_cv_func_decl_$1=no")])

  if eval "test \"`echo '$jm_cv_func_decl_'$1`\" = yes"; then
    AC_MSG_RESULT(yes)
    ifelse([$3], , :, [$3])
  else
    AC_MSG_RESULT(no)
    ifelse([$4], , , [$4
])dnl
  fi
])dnl

dnl jm_CHECK_DECLARATIONS(INCLUDES, FUNCTION... [, ACTION-IF-DECLARED
dnl                       [, ACTION-IF-NOT-DECLARED]])
AC_DEFUN(jm_CHECK_DECLARATIONS,
[
  for jm_func in $2
  do
    jm_CHECK_DECLARATION($jm_func, $1,
    [
      jm_tr_func=HAVE_DECL_`echo $jm_func | tr abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ`
      AC_DEFINE_UNQUOTED($jm_tr_func) $3], $4)dnl
  done
])

# Macro to add for using GNU gettext.
# Ulrich Drepper <drepper@cygnus.com>, 1995.
#
# This file can be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU Public License
# but which still want to provide support for the GNU gettext functionality.
# Please note that the actual code is *not* freely available.

# serial 108

AC_PREREQ(2.13)               dnl Minimum Autoconf version required.

AC_DEFUN(AM_WITH_NLS,
  [AC_MSG_CHECKING([whether NLS is requested])
    dnl Default is enabled NLS
    AC_ARG_ENABLE(nls,
      [  --disable-nls           do not use Native Language Support],
      USE_NLS=$enableval, USE_NLS=yes)
    AC_MSG_RESULT($USE_NLS)
    AC_SUBST(USE_NLS)

    USE_INCLUDED_LIBINTL=no

    dnl If we use NLS figure out what method
    if test "$USE_NLS" = "yes"; then
      AC_DEFINE(ENABLE_NLS, 1, [Define to 1 if NLS is requested.])
      AC_MSG_CHECKING([whether included gettext is requested])
      AC_ARG_WITH(included-gettext,
        [  --with-included-gettext use the GNU gettext library included here],
        nls_cv_force_use_gnu_gettext=$withval,
        nls_cv_force_use_gnu_gettext=no)
      AC_MSG_RESULT($nls_cv_force_use_gnu_gettext)

      nls_cv_use_gnu_gettext="$nls_cv_force_use_gnu_gettext"
      if test "$nls_cv_force_use_gnu_gettext" != "yes"; then
        dnl User does not insist on using GNU NLS library.  Figure out what
        dnl to use.  If gettext or catgets are available (in this order) we
        dnl use this.  Else we have to fall back to GNU NLS library.
	dnl catgets is only used if permitted by option --with-catgets.
	nls_cv_header_intl=
	nls_cv_header_libgt=
	CATOBJEXT=NONE

	AC_CHECK_HEADER(libintl.h,
	  [AC_CACHE_CHECK([for gettext in libc], gt_cv_func_gettext_libc,
	    [AC_TRY_LINK([#include <libintl.h>], [return (int) gettext ("")],
	       gt_cv_func_gettext_libc=yes, gt_cv_func_gettext_libc=no)])

	   if test "$gt_cv_func_gettext_libc" != "yes"; then
	     AC_CHECK_LIB(intl, bindtextdomain,
	       [AC_CACHE_CHECK([for gettext in libintl],
		 gt_cv_func_gettext_libintl,
		 [AC_CHECK_LIB(intl, gettext,
		  gt_cv_func_gettext_libintl=yes,
		  gt_cv_func_gettext_libintl=no)],
		 gt_cv_func_gettext_libintl=no)])

	     if test "$gt_cv_func_gettext_libintl" = yes; then
	       LIBS="$LIBS -lintl"
	     fi

	   fi

	   if test "$gt_cv_func_gettext_libc" = "yes" \
	      || test "$gt_cv_func_gettext_libintl" = "yes"; then
	      AC_DEFINE(HAVE_GETTEXT, 1,
	  [Define to 1 if you have gettext and don't want to use GNU gettext.])
	      AM_PATH_PROG_WITH_TEST(MSGFMT, msgfmt,
		[test -z "`$ac_dir/$ac_word -h 2>&1 | grep 'dv '`"], no)dnl
	      if test "$MSGFMT" != "no"; then
		AC_CHECK_FUNCS(dcgettext)
		AC_PATH_PROG(GMSGFMT, gmsgfmt, $MSGFMT)
		AM_PATH_PROG_WITH_TEST(XGETTEXT, xgettext,
		  [test -z "`$ac_dir/$ac_word -h 2>&1 | grep '(HELP)'`"], :)
		AC_TRY_LINK(, [extern int _nl_msg_cat_cntr;
			       return _nl_msg_cat_cntr],
		  [CATOBJEXT=.gmo
		   DATADIRNAME=share],
		  [CATOBJEXT=.mo
		   DATADIRNAME=lib])
		INSTOBJEXT=.mo
	      fi
	    fi
	])

        if test "$CATOBJEXT" = "NONE"; then
	  AC_MSG_CHECKING([whether catgets can be used])
	  AC_ARG_WITH(catgets,
	    [  --with-catgets          use catgets functions if available],
	    nls_cv_use_catgets=$withval, nls_cv_use_catgets=no)
	  AC_MSG_RESULT($nls_cv_use_catgets)

	  if test "$nls_cv_use_catgets" = "yes"; then
	    dnl No gettext in C library.  Try catgets next.
	    AC_CHECK_LIB(i, main)
	    AC_CHECK_FUNC(catgets,
	      [AC_DEFINE(HAVE_CATGETS, 1,
			 [Define as 1 if you have catgets and don't want to use GNU gettext.])
	       INTLOBJS="\$(CATOBJS)"
	       AC_PATH_PROG(GENCAT, gencat, no)dnl
	       if test "$GENCAT" != "no"; then
		 AC_PATH_PROG(GMSGFMT, gmsgfmt, no)
		 if test "$GMSGFMT" = "no"; then
		   AM_PATH_PROG_WITH_TEST(GMSGFMT, msgfmt,
		    [test -z "`$ac_dir/$ac_word -h 2>&1 | grep 'dv '`"], no)
		 fi
		 AM_PATH_PROG_WITH_TEST(XGETTEXT, xgettext,
		   [test -z "`$ac_dir/$ac_word -h 2>&1 | grep '(HELP)'`"], :)
		 USE_INCLUDED_LIBINTL=yes
		 CATOBJEXT=.cat
		 INSTOBJEXT=.cat
		 DATADIRNAME=lib
		 INTLDEPS='$(top_builddir)/intl/libintl.a'
		 INTLLIBS=$INTLDEPS
		 LIBS=`echo $LIBS | sed -e 's/-lintl//'`
		 nls_cv_header_intl=intl/libintl.h
		 nls_cv_header_libgt=intl/libgettext.h
	       fi])
	  fi
        fi

        if test "$CATOBJEXT" = "NONE"; then
	  dnl Neither gettext nor catgets in included in the C library.
	  dnl Fall back on GNU gettext library.
	  nls_cv_use_gnu_gettext=yes
        fi
      fi

      if test "$nls_cv_use_gnu_gettext" = "yes"; then
        dnl Mark actions used to generate GNU NLS library.
        INTLOBJS="\$(GETTOBJS)"
        AM_PATH_PROG_WITH_TEST(MSGFMT, msgfmt,
	  [test -z "`$ac_dir/$ac_word -h 2>&1 | grep 'dv '`"], msgfmt)
        AC_PATH_PROG(GMSGFMT, gmsgfmt, $MSGFMT)
        AM_PATH_PROG_WITH_TEST(XGETTEXT, xgettext,
	  [test -z "`$ac_dir/$ac_word -h 2>&1 | grep '(HELP)'`"], :)
        AC_SUBST(MSGFMT)
	USE_INCLUDED_LIBINTL=yes
        CATOBJEXT=.gmo
        INSTOBJEXT=.mo
        DATADIRNAME=share
	INTLDEPS='$(top_builddir)/intl/libintl.a'
	INTLLIBS=$INTLDEPS
	LIBS=`echo $LIBS | sed -e 's/-lintl//'`
        nls_cv_header_intl=intl/libintl.h
        nls_cv_header_libgt=intl/libgettext.h
      fi

      dnl Test whether we really found GNU xgettext.
      if test "$XGETTEXT" != ":"; then
	dnl If it is no GNU xgettext we define it as : so that the
	dnl Makefiles still can work.
	if $XGETTEXT --omit-header /dev/null 2> /dev/null; then
	  : ;
	else
	  AC_MSG_RESULT(
	    [found xgettext program is not GNU xgettext; ignore it])
	  XGETTEXT=":"
	fi
      fi

      # We need to process the po/ directory.
      POSUB=po
    else
      DATADIRNAME=share
      nls_cv_header_intl=intl/libintl.h
      nls_cv_header_libgt=intl/libgettext.h
    fi
    if test -z "$nsl_cv_header_intl"; then
      # Clean out junk possibly left behind by a previous configuration.
      rm -f intl/libintl.h
    fi
    AC_LINK_FILES($nls_cv_header_libgt, $nls_cv_header_intl)
    AC_OUTPUT_COMMANDS(
     [case "$CONFIG_FILES" in *po/Makefile.in*)
        sed -e "/POTFILES =/r po/POTFILES" po/Makefile.in > po/Makefile
      esac])


    # If this is used in GNU gettext we have to set USE_NLS to `yes'
    # because some of the sources are only built for this goal.
    if test "$PACKAGE" = gettext; then
      USE_NLS=yes
      USE_INCLUDED_LIBINTL=yes
    fi

    dnl These rules are solely for the distribution goal.  While doing this
    dnl we only have to keep exactly one list of the available catalogs
    dnl in configure.in.
    for lang in $ALL_LINGUAS; do
      GMOFILES="$GMOFILES $lang.gmo"
      POFILES="$POFILES $lang.po"
    done

    dnl Make all variables we use known to autoconf.
    AC_SUBST(USE_INCLUDED_LIBINTL)
    AC_SUBST(CATALOGS)
    AC_SUBST(CATOBJEXT)
    AC_SUBST(DATADIRNAME)
    AC_SUBST(GMOFILES)
    AC_SUBST(INSTOBJEXT)
    AC_SUBST(INTLDEPS)
    AC_SUBST(INTLLIBS)
    AC_SUBST(INTLOBJS)
    AC_SUBST(POFILES)
    AC_SUBST(POSUB)
  ])

AC_DEFUN(AM_GNU_GETTEXT,
  [AC_REQUIRE([AC_PROG_MAKE_SET])dnl
   AC_REQUIRE([AC_PROG_CC])dnl
   AC_REQUIRE([AC_PROG_RANLIB])dnl
   AC_REQUIRE([AC_ISC_POSIX])dnl
   AC_REQUIRE([AC_HEADER_STDC])dnl
   AC_REQUIRE([AC_C_CONST])dnl
   AC_REQUIRE([AC_C_INLINE])dnl
   AC_REQUIRE([AC_FUNC_ALLOCA])dnl
   AC_REQUIRE([AC_FUNC_MMAP])dnl

   AC_REPLACE_TYPES(off_t size_t)
   AC_CHECK_HEADERS([argz.h limits.h locale.h nl_types.h malloc.h string.h \
unistd.h sys/param.h])
   AC_CHECK_FUNCS([getcwd munmap putenv setenv setlocale strchr strcasecmp \
strdup __argz_count __argz_stringify __argz_next stpcpy])

   AM_LC_MESSAGES
   AM_WITH_NLS

   if test "x$CATOBJEXT" != "x"; then
     if test "x$ALL_LINGUAS" = "x"; then
       LINGUAS=
     else
       AC_MSG_CHECKING(for catalogs to be installed)
       NEW_LINGUAS=
       for lang in ${LINGUAS=$ALL_LINGUAS}; do
         case "$ALL_LINGUAS" in
          *$lang*) NEW_LINGUAS="$NEW_LINGUAS $lang" ;;
         esac
       done
       LINGUAS=$NEW_LINGUAS
       AC_MSG_RESULT($LINGUAS)
     fi

     dnl Construct list of names of catalog files to be constructed.
     if test -n "$LINGUAS"; then
       for lang in $LINGUAS; do CATALOGS="$CATALOGS $lang$CATOBJEXT"; done
     fi
   fi

   dnl The reference to <locale.h> in the installed <libintl.h> file
   dnl must be resolved because we cannot expect the users of this
   dnl to define HAVE_LOCALE_H.
   if test $ac_cv_header_locale_h = yes; then
     INCLUDE_LOCALE_H="#include <locale.h>"
   else
     INCLUDE_LOCALE_H="\
/* The system does not provide the header <locale.h>.  Take care yourself.  */"
   fi
   AC_SUBST(INCLUDE_LOCALE_H)

   dnl Determine which catalog format we have (if any is needed)
   dnl For now we know about two different formats:
   dnl   Linux libc-5 and the normal X/Open format
   test -d intl || mkdir intl
   if test "$CATOBJEXT" = ".cat"; then
     AC_CHECK_HEADER(linux/version.h, msgformat=linux, msgformat=xopen)

     dnl Transform the SED scripts while copying because some dumb SEDs
     dnl cannot handle comments.
     sed -e '/^#/d' $srcdir/intl/$msgformat-msg.sed > intl/po2msg.sed
   fi
   dnl po2tbl.sed is always needed.
   sed -e '/^#.*[^\\]$/d' -e '/^#$/d' \
     $srcdir/intl/po2tbl.sed.in > intl/po2tbl.sed

   dnl In the intl/Makefile.in we have a special dependency which makes
   dnl only sense for gettext.  We comment this out for non-gettext
   dnl packages.
   if test "$PACKAGE" = "gettext"; then
     GT_NO="#NO#"
     GT_YES=
   else
     GT_NO=
     GT_YES="#YES#"
   fi
   AC_SUBST(GT_NO)
   AC_SUBST(GT_YES)

   dnl If the AC_CONFIG_AUX_DIR macro for autoconf is used we possibly
   dnl find the mkinstalldirs script in another subdir but ($top_srcdir).
   dnl Try to locate is.
   MKINSTALLDIRS=
   if test -n "$ac_aux_dir"; then
     MKINSTALLDIRS="\$(top_srcdir)/$ac_aux_dir/mkinstalldirs"
   else
     MKINSTALLDIRS="\$(top_srcdir)/mkinstalldirs"
   fi
   AC_SUBST(MKINSTALLDIRS)

   dnl *** For now the libtool support in intl/Makefile is not for real.
   l=
   AC_SUBST(l)

   dnl Generate list of files to be processed by xgettext which will
   dnl be included in po/Makefile.
   test -d po || mkdir po
   if test "x$srcdir" != "x."; then
     if test "x`echo $srcdir | sed 's@/.*@@'`" = "x"; then
       posrcprefix="$srcdir/"
     else
       posrcprefix="../$srcdir/"
     fi
   else
     posrcprefix="../"
   fi
   rm -f po/POTFILES
   sed -e "/^#/d" -e "/^\$/d" -e "s,.*,	$posrcprefix& \\\\," -e "\$s/\(.*\) \\\\/\1/" \
	< $srcdir/po/POTFILES.in > po/POTFILES
  ])

# Search path for a program which passes the given test.
# Ulrich Drepper <drepper@cygnus.com>, 1996.
#
# This file can be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU Public License
# but which still want to provide support for the GNU gettext functionality.
# Please note that the actual code is *not* freely available.

# serial 1

dnl AM_PATH_PROG_WITH_TEST(VARIABLE, PROG-TO-CHECK-FOR,
dnl   TEST-PERFORMED-ON-FOUND_PROGRAM [, VALUE-IF-NOT-FOUND [, PATH]])
AC_DEFUN(AM_PATH_PROG_WITH_TEST,
[# Extract the first word of "$2", so it can be a program name with args.
set dummy $2; ac_word=[$]2
AC_MSG_CHECKING([for $ac_word])
AC_CACHE_VAL(ac_cv_path_$1,
[case "[$]$1" in
  /*)
  ac_cv_path_$1="[$]$1" # Let the user override the test with a path.
  ;;
  *)
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  for ac_dir in ifelse([$5], , $PATH, [$5]); do
    test -z "$ac_dir" && ac_dir=.
    if test -f $ac_dir/$ac_word; then
      if [$3]; then
	ac_cv_path_$1="$ac_dir/$ac_word"
	break
      fi
    fi
  done
  IFS="$ac_save_ifs"
dnl If no 4th arg is given, leave the cache variable unset,
dnl so AC_PATH_PROGS will keep looking.
ifelse([$4], , , [  test -z "[$]ac_cv_path_$1" && ac_cv_path_$1="$4"
])dnl
  ;;
esac])dnl
$1="$ac_cv_path_$1"
if test -n "[$]$1"; then
  AC_MSG_RESULT([$]$1)
else
  AC_MSG_RESULT(no)
fi
AC_SUBST($1)dnl
])

# Check whether LC_MESSAGES is available in <locale.h>.
# Ulrich Drepper <drepper@cygnus.com>, 1995.
#
# This file can be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU Public License
# but which still want to provide support for the GNU gettext functionality.
# Please note that the actual code is *not* freely available.

# serial 2

AC_PREREQ(2.13)               dnl Minimum Autoconf version required.

AC_DEFUN(AM_LC_MESSAGES,
  [if test $ac_cv_header_locale_h = yes; then
    AC_CACHE_CHECK([for LC_MESSAGES], am_cv_val_LC_MESSAGES,
      [AC_TRY_LINK([#include <locale.h>], [return LC_MESSAGES],
       am_cv_val_LC_MESSAGES=yes, am_cv_val_LC_MESSAGES=no)])
    if test $am_cv_val_LC_MESSAGES = yes; then
      AC_DEFINE(HAVE_LC_MESSAGES, 1,
		[Define if your locale.h file contains LC_MESSAGES.])
    fi
  fi])


# serial 2

AC_DEFUN(ad_WITH_DMALLOC,
[AC_MSG_CHECKING(if malloc debugging is wanted)
AC_ARG_WITH(dmalloc,
[  --with-dmalloc[=lvl]    if lvl = 1 or yes, use dmalloc
                          if lvl = 2 use extra debugging functions
                          (ftp://ftp.letters.com/src/dmalloc/dmalloc.tar.gz)],
[case "$withval" in
  yes|1)
    AC_MSG_RESULT(yes)
    AC_DEFINE(WITH_DMALLOC)
    LIBS="$LIBS -ldmalloc"
    LDFLAGS="$LDFLAGS -g"
    ;;
  2)
    AC_MSG_RESULT([yes, with extra debugging functions])
    AC_DEFINE(WITH_DMALLOC)
    AC_DEFINE(DMALLOC_FUNC_CHECK)
    LIBS="$LIBS -ldmalloc"
    LDFLAGS="$LDFLAGS -g"
    ;;
  *)
    AC_MSG_RESULT(no)
    ;;
esac], [AC_MSG_RESULT(no)])
])

dnl Check if the compiler supports useful warning options.  There's a few that
dnl we don't use, simply because they're too noisy:
dnl
dnl     -Wconversion (useful in older versions of gcc, but not in gcc 2.7.x)
dnl     -Wredundant-decls (system headers make this too noisy)
dnl     -Wtraditional (combines too many unrelated messages, only a few useful)
dnl     -Wcast-qual because with char * cp; const char * ccp;
dnl                 cp = (char *) ccp;
dnl                 is the only portable way to do cp = ccp;
dnl     -pedantic
dnl

#serial 3
AC_DEFUN([ad_GCC_WARNINGS],
[AC_ARG_ENABLE(warnings,
     	       [  --enable-warnings       enable compiler warnings])
 if test "$enable_warnings" = "yes"; then
   CF_GCC_WARNINGS
 fi])

AC_DEFUN([CF_GCC_WARNINGS],
[if test -n "$GCC"; then
  AC_CACHE_CHECK([for gcc warning options], ac_cv_prog_gcc_warn_flags,
  [changequote(,)dnl
  cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
int main(int argc, char *argv[]) { return argv[argc-1] == 0; }
EOF
  changequote([,])dnl
  cf_save_CFLAGS="$CFLAGS"
  ac_cv_prog_gcc_warn_flags="-W -Wall"
  for cf_opt in \
   Wbad-function-cast \
   Wcast-align \
   Wmissing-declarations \
   Wmissing-prototypes \
   Wnested-externs \
   Wpointer-arith \
   Wshadow \
   Wstrict-prototypes \
   Wwrite-strings
  do
    CFLAGS="$cf_save_CFLAGS $ac_cv_prog_gcc_warn_flags -$cf_opt"
    if AC_TRY_EVAL(ac_compile); then
      test -n "$verbose" && AC_MSG_RESULT(... -$cf_opt)
	ac_cv_prog_gcc_warn_flags="$ac_cv_prog_gcc_warn_flags -$cf_opt"
	test "$cf_opt" = Wcast-qual && ac_cv_prog_gcc_warn_flags="$ac_cv_prog_gcc_warn_flags -DXTSTRINGDEFINES"
    fi
  done
  rm -f conftest*
  CFLAGS="$cf_save_CFLAGS"])
fi
EXTRA_CFLAGS="${ac_cv_prog_gcc_warn_flags}"
AC_SUBST(EXTRA_CFLAGS)
])dnl


# serial 1

AC_DEFUN(AM_PATH_LISPDIR,
 [# If set to t, that means we are running in a shell under Emacs.
  # If you have an Emacs named "t", then use the full path.
  test "$EMACS" = t && EMACS=
  AC_PATH_PROGS(EMACS, emacs xemacs, no)
  if test $EMACS != "no"; then
    AC_MSG_CHECKING([where .elc files should go])
    dnl Set default value
    lispdir="\$(datadir)/emacs/site-lisp"
    emacs_flavor=`echo "$EMACS" | sed -e 's,^.*/,,'`
    if test "x$prefix" = "xNONE"; then
      if test -d $ac_default_prefix/share/$emacs_flavor/site-lisp; then
	lispdir="\$(prefix)/share/$emacs_flavor/site-lisp"
      else
	if test -d $ac_default_prefix/lib/$emacs_flavor/site-lisp; then
	  lispdir="\$(prefix)/lib/$emacs_flavor/site-lisp"
	fi
      fi
    else
      if test -d $prefix/share/$emacs_flavor/site-lisp; then
	lispdir="\$(prefix)/share/$emacs_flavor/site-lisp"
      else
	if test -d $prefix/lib/$emacs_flavor/site-lisp; then
	  lispdir="\$(prefix)/lib/$emacs_flavor/site-lisp"
	fi
      fi
    fi
    AC_MSG_RESULT($lispdir)
  fi
  AC_SUBST(lispdir)])


# serial 1

dnl 
AC_DEFUN(ad_ENABLE_PATHS,
  [AC_MSG_CHECKING(whether paths should be hardcoded)
   AC_ARG_ENABLE(paths,
     	         [  --enable-paths          hard code the path of the tools])
   test "$enable_paths" = "yes" || enable_paths=no
   AC_MSG_RESULT($enable_paths)])

dnl ad_CHECK_PROG(PROGRAM[, COMMENT ON FAILURE])
dnl defines $COM_PROGRAM to `#' if PROGRAM is not available, to `' otherwise
dnl and defines $PROGRAM to PROGRAM if paths should not be hardcoded,
dnl and to the path to PROGRAM otherwise.
AC_DEFUN(ad_CHECK_PROG,
  [if test "$enable_paths" = "no"; then
     AC_CHECK_PROG(COM_$1, $1, yes, no)
     $1="$1";
     if test "[$]COM_$1" = "yes"; then
       COM_$1="";
     else
       COM_$1="#";
     fi
   else
     AC_PATH_PROG($1, $1, [#])
     if test "$1" = "#"; then #  not found
       $1="$1" # let the name of the program as path
       COM_$1="#"
     else
       COM_$1=""
     fi
   fi
   if test "[$]COM_$1" = "#" && test -n "$2"; then
     AC_MSG_WARN([============================================================
$2]);
     AC_MSG_WARN([============================================================]);
   fi
   AC_SUBST($1)
   AC_SUBST(COM_$1)])

dnl ad_CHECK_PROG(PROGRAM[, COMMENT ON FAILURE])
dnl defines $COM_PROGRAM to `#' if PROGRAM is not available, to `' otherwise
dnl and defines $PROGRAM to PROGRAM if paths should not be hardcoded,
dnl and to the path to PROGRAM otherwise.
AC_DEFUN([ad_CHECK_PROGS],
[ad_FOREACH(AC_PROG, [$1], [ad_CHECK_PROG(AC_PROG, $2)])])


# Implement foreach loops at m4' level (not shell)
# Akim.Demaille@inf.enst.fr

#serial 1
dnl foreach(x, (item_1, item_2, ..., item_n), stmt)
define([m4shift], [builtin([shift], $@)])
define([foreach], [pushdef([$1], [])_foreach([$1], [$2], [$3])popdef([$1])])
define([_arg1], [$1])
define([_foreach], 
        [ifelse([$2], [()], ,
                [define([$1], _arg1$2)$3[]_foreach([$1], (m4shift$2), [$3])])])

dnl _comatize(item_1 item_2... item_n)
define([_comatize],
[changequote(<<, >>)dnl
patsubst(patsubst(<<$1>>, <<\\
>>, << >>), <<\>[ \t]+\<>>, <<, >>)<<>>dnl
changequote([, ])])dnl

dnl ad_FOREACH(x, (item_1 item_2 ... item_n), stmt)
AC_DEFUN([ad_FOREACH], [foreach([$1], (_comatize($2)), [$3])])

define([m4paste], [$1$2])

dnl ad_DEFUNS(NAME, STMT)
AC_DEFUN(ad_DEFUNS,
[define($1, [$2])
define($1S, [ad_FOREACH(ac_var, ]m4paste($, 1)[, [$1(ac_var)])])
])

# Define a conditional.

AC_DEFUN(AM_CONDITIONAL,
[AC_SUBST($1_TRUE)
AC_SUBST($1_FALSE)
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi])


# serial 4

dnl This is a test to know what is the correct way to get
dnl file following the links
dnl I hope it is robust, but I'm a novice :) Akim Demaille
AC_DEFUN(ad_PROG_FILE_LINK,
  [AC_PATH_PROG(file_prog, file)dnl
   AC_MSG_CHECKING(for the option for file to follow the links)
   AC_CACHE_VAL(ac_cv_prog_file_link_option,[
     touch conftestfile
     ln -s conftestfile conftestfile1
     for file_opt in '' ' -L'
     do
       res=`eval $file_prog $file_opt conftestfile1 2>&-` || res=symbolic
       case $res in
	  *symbolic*) ;;
	  *) ac_cv_prog_file_link_option=$file_opt ; break    ;;
       esac
     done
     rm conftestfile conftestfile1
     if test "X$ac_cv_prog_file_link_option" = X; then
        ac_cv_prog_file_link_option="no";
     fi])dnl
   # If no flag were found, default to `file' with no options
   case "X$ac_cv_prog_file_link_option" in
     X)   AC_MSG_RESULT(none required) ;
          FILE_LINK=${file_prog} ;;
     Xno) AC_MSG_RESULT(no way);
          FILE_LINK=${file_prog} ;;
     *)   AC_MSG_RESULT($ac_cv_prog_file_link_option);
          FILE_LINK=${file_prog}${ac_cv_prog_file_link_option} ;;
   esac])


# serial 3

#
#	Find the program to send to the printer
# Put the prog name in LPR, its option for destination in LPR_QUEUE_OPTION
#
AC_DEFUN(ad_PROG_LPR,
  [AC_CHECK_PROGS(LPR, lp lpr, no)
   AC_MSG_CHECKING([for printer queue selection])
   case "x$LPR" in
      xlp)  LPR_QUEUE_OPTION=-d ;;
      xlpr) LPR_QUEUE_OPTION=-P ;;
      xno)  LPR="cat"
            LPR_QUEUE_OPTION=">"
	    ;;
      *)    # This user has specified per $LPR and $LPR_QUEUE_OPTION
	    ;;
   esac
   AC_MSG_RESULT([$LPR $LPR_QUEUE_OPTION PRINTER])
   AC_SUBST(LPR_QUEUE_OPTION)])

dnl Testing delegations for a2ps: a recent version of psutils
dnl
dnl Usage: a2_PSUTILS(major, minor)
dnl
dnl If psutils version MAJOR, patch level MINOR is available
dnl     SUBST(PSUTILS) to <nothing>
dnl else
dnl     SUBST(PSUTILS) to `#'
dnl Akim.Demaille@inf.enst.fr

# serial 2

AC_DEFUN(a2_PSUTILS,
[ad_CHECK_PROG(psselect)
ad_CHECK_PROG(psnup)
if test "$COM_psselect" = "#"; then
  AC_MSG_WARN([a2ps works better with the psutils.  Available at])
  AC_MSG_WARN([  http://www.dcs.ed.ac.uk/home/ajcd/psutils/])
  PSUTILS="#"
else
  # There is one.  Check version > MAJOR.MINOR
  ac_prog_psselect_banner=`psselect -v 2>&1 | sed 1q`
  ac_prog_psselect_release=`set -- $ac_prog_psselect_banner && echo $[3]`
  ac_prog_psselect_patch=`set -- $ac_prog_psselect_banner && echo $[5]`
  test ${ac_prog_psselect_release}0 -lt $1[0] && COM_psselect=no
  test ${ac_prog_psselect_patch}0 -lt $2[0] && COM_psselect=no
  if test "$COM_psselect" = "#"; then
     AC_MSG_WARN([a2ps works better with the psutils.  Yours is too old.])
     AC_MSG_WARN([  http://www.dcs.ed.ac.uk/home/ajcd/psutils/])
     COM_PSUTILS="#"
  else
     COM_PSUTILS=""
  fi
fi
AC_SUBST(COM_PSUTILS)])

