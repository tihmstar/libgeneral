AC_PREREQ([2.69])
AC_INIT([libgeneral], m4_esyscmd([git rev-list --count HEAD | tr -d '\n']), [tihmstar@gmail.com])

AC_CANONICAL_SYSTEM
AC_CANONICAL_HOST
AM_PROG_LIBTOOL

AM_INIT_AUTOMAKE([subdir-objects])
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
      AC_CHECK_LIB([c++], [_ZNSt13bad_exceptionD0Ev],[have_libcpp=true],[have_libcpp=false])
      if test "x$have_libcpp" == "xtrue"; then
        ORIG_CXXFLAGS=${CXXFLAGS}
        CXXFLAGS+=" -stdlib=libc++" # normal case
        AC_SUBST([cpp_lib_flags], [-stdlib=libc++])
        AC_LANG_PUSH([C++])
        AC_CHECK_HEADER([string],[have_libcpp=true],[
          # iOS 5 and 6 have both, libc++ and libstdc++, but the former is not actually yet usable
          have_libcpp=false
          CXXFLAGS=${ORIG_CXXFLAGS}
        ])
        AC_LANG_POP([C++])
      fi
      if test "x$have_libcpp" != "xtrue"; then
        AC_CHECK_LIB([stdc++], [_ZNSt13bad_exceptionD0Ev],[have_libstdcpp=true],[have_libstdcpp=false])
        if test "x$have_libstdcpp" == "xtrue"; then
          CXXFLAGS+=" -stdlib=libstdc++" # iOS <= 6 (very old lib) case
          AC_SUBST([cpp_lib_flags], [-stdlib=libstdc++])
          AC_DEFINE([DONT_HAVE_FUNCTIONAL],[1],[Define if you DONT have <functional>]) # header exists, but std::function doesn't
        fi
      fi
      AC_LANG_PUSH([C++])
      AC_CHECK_HEADER([functional],[],[
        AC_DEFINE([DONT_HAVE_FUNCTIONAL],[1],[Define if you DONT have <functional>])
      ])
      AC_LANG_POP([C++])
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

AC_ARG_ENABLE([cleanup],
            [AS_HELP_STRING([--disable-cleanup],
            [disable cleanup macro(default is no)])],
            [cleanup_disabled=true],
            [cleanup_disabled=false])


if test "x$cleanup_disabled" == "xtrue"; then
  AC_DEFINE(LIBGENERAL_CLEANUP_IS_DISABLED, 1, [Define if you want to disable 'cleanup' macro globally])
  AC_SUBST([LIBGENERAL_CLEANUP_IS_DISABLED], [1])
else
  AC_SUBST([LIBGENERAL_CLEANUP_IS_DISABLED], [0])
fi
AM_CONDITIONAL(LIBGENERAL_CLEANUP_IS_DISABLED, test x$cleanup_disabled == xtrue)

if test "x$build_minimal" == "xyes"; then
  AC_DEFINE(BUILD_MINIMAL, 1, [Define if you're building minimal lib])
fi
AM_CONDITIONAL(BUILD_MINIMAL, test x$build_minimal == xyes)

AC_CONFIG_FILES([Makefile
                 include/libgeneral/macros.h
                 include/Makefile
                 libgeneral.pc
                 libgeneral/Makefile])

AC_OUTPUT

echo "
Configuration for $PACKAGE-$VERSION:
-------------------------------------------

  install prefix ..........: $prefix
  minimal lib .............: $build_minimal
  disable cleanup macro ...: $cleanup_disabled"
echo "  compiler ................: ${CC}

  Now type 'make' to build $PACKAGE-$VERSION,
  and then 'make install' for installation.
"
