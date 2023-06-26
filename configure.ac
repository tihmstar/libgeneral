AC_PREREQ([2.69])
AC_INIT([libgeneral], m4_esyscmd([git rev-list --count HEAD | tr -d '\n']), [tihmstar@gmail.com])

AC_CANONICAL_SYSTEM
AC_CANONICAL_HOST
AM_PROG_LIBTOOL

AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

AC_DEFINE([VERSION_COMMIT_COUNT], "m4_esyscmd([git rev-list --count HEAD | tr -d '\n'])", [Git commit count])
AC_DEFINE([VERSION_COMMIT_SHA], "m4_esyscmd([git rev-parse HEAD | tr -d '\n'])", [Git commit sha])
AC_SUBST([VERSION_COMMIT_COUNT], ["m4_esyscmd([git rev-list --count HEAD | tr -d '\n'])"])
AC_SUBST([VERSION_COMMIT_SHA], ["m4_esyscmd([git rev-parse HEAD | tr -d '\n'])"])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

CXXFLAGS+=" -std=c++11"
CFLAGS+=" -std=c11"

# Check for operating system
AC_MSG_CHECKING([whether we need platform-specific build settings])
case $host_os in
  darwin* )
      AC_LANG_PUSH([C++])
      AC_CHECK_HEADER([functional],[
        CXXFLAGS+=" -stdlib=libc++" # normal case
      ],[
        CXXFLAGS+=" -stdlib=libstdc++" # iOS 6 (very old lib) case
      ])
      AC_LANG_POP([C++])
      AC_CHECK_HEADER([functional],[],[AC_DEFINE([DONT_HAVE_FUNCTIONAL],[1],[Define if you DONT have <functional>])])
      ;;
  mingw32*)
        LDFLAGS+=" -no-undefined -lpthread"
        ;;
esac

AC_ARG_WITH([extras],
            [AS_HELP_STRING([--without-extras],
            [build only minimal lib @<:@default=no@:>@])],
            [build_minimal=yes],
            [build_minimal=no])


if test "x$build_minimal" == "xyes"; then
  AC_DEFINE(BUILD_MINIMAL, 1, [Define if you're building minimal lib])
fi
AM_CONDITIONAL(BUILD_MINIMAL, test x$build_minimal == xyes)

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 libgeneral.pc
                 libgeneral/Makefile])

AC_OUTPUT

echo "
Configuration for $PACKAGE-$VERSION:
-------------------------------------------

  install prefix ..........: $prefix
  minimal lib .............: $build_minimal"
echo "  compiler ................: ${CC}

  Now type 'make' to build $PACKAGE-$VERSION,
  and then 'make install' for installation.
"
