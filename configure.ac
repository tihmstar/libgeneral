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
AC_PROG_CXX([clang++])
AC_PROG_CC([clang])

CXXFLAGS+=" -std=c++11"
CFLAGS+=" -std=c11"

# Check for operating system
AC_MSG_CHECKING([whether we need platform-specific build settings])
case $host_os in
  darwin* )
      AC_MSG_RESULT([${host_os}])
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
  *mingw*|*msys*|*linux*)
        AC_MSG_RESULT([${host_os}])
        LDFLAGS+=" -no-undefined -lpthread"
        AC_SUBST([extra_lib_flags], ["-no-undefined -lpthread"])
        ;;
esac

AC_ARG_ENABLE([debug],
            [AS_HELP_STRING([--enable-debug],
            [enable debug build(default is no)])],
            [debug_build=true],
            [debug_build=false])

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


if test "$debug_build" = true; then
  echo "*** Note: debug build requested ***"
  CFLAGS+=" -g -O0 -DDEBUG=1"
  CXXFLAGS+=" -g -O0 -DDEBUG=1"
fi

if test "x$build_minimal" == "xyes"; then
  AC_DEFINE(BUILD_MINIMAL, 1, [Define if you're building minimal lib])
fi
AM_CONDITIONAL(BUILD_MINIMAL, test x$build_minimal == xyes)

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h byteswap.h endian.h sys/param.h winsock.h])

# Check for functions
AC_CHECK_DECLS([htonll], [], [], [[#include <arpa/inet.h>]])

AC_CHECK_DECLS([htole64], [
    AC_SUBST([HAVE_DECL_HTOLE64], [1])
], [
    AC_SUBST([HAVE_DECL_HTOLE64], [0])
], [[#include <endian.h>]])

AC_CHECK_DECLS([htole32], [
    AC_SUBST([HAVE_DECL_HTOLE32], [1])
], [
    AC_SUBST([HAVE_DECL_HTOLE32], [0])
], [[#include <endian.h>]])

AC_CHECK_DECLS([htole16], [
    AC_SUBST([HAVE_DECL_HTOLE16], [1])
], [
    AC_SUBST([HAVE_DECL_HTOLE16], [0])
], [[#include <endian.h>]])

#Generate Headers
if test "x$cleanup_disabled" == "xtrue"; then
  AC_DEFINE(LIBGENERAL_CLEANUP_IS_DISABLED, 1, [Define if you want to disable 'cleanup' macro globally])
  AC_SUBST([LIBGENERAL_CLEANUP_IS_DISABLED], [1])
else
  AC_SUBST([LIBGENERAL_CLEANUP_IS_DISABLED], [0])
fi
AM_CONDITIONAL(LIBGENERAL_CLEANUP_IS_DISABLED, test x$cleanup_disabled == xtrue)

if test "x$ac_cv_header_arpa_inet_h" == "xyes"; then
  AC_SUBST([HEADER_HAVE_ARPA_INET_H], [1])
else
  AC_SUBST([HEADER_HAVE_ARPA_INET_H], [0])
fi

if test "x$ac_cv_header_byteswap_h" == "xyes"; then
  AC_SUBST([HEADER_HAVE_BYTESWAP_H], [1])
else
  AC_SUBST([HEADER_HAVE_BYTESWAP_H], [0])
fi

if test "x$ac_cv_header_endian_h" == "xyes"; then
  AC_SUBST([HEADER_HAVE_ENDIAN_H], [1])
else
  AC_SUBST([HEADER_HAVE_ENDIAN_H], [0])
fi

if test "x$ac_cv_header_sys_param_h" == "xyes"; then
  AC_SUBST([HEADER_HAVE_SYS_PARAM_H], [1])
else
  AC_SUBST([HEADER_HAVE_SYS_PARAM_H], [0])
fi

if test "x$ac_cv_header_winsock_h" == "xyes"; then
  AC_SUBST([HEADER_HAVE_WINSOCK_H], [1])
else
  AC_SUBST([HEADER_HAVE_WINSOCK_H], [0])
fi

if test "x$ac_cv_have_decl_htonll" == "xyes"; then
  AC_SUBST([FUNC_HAVE_DECL_HTONLL], [1])
else
  AC_SUBST([FUNC_HAVE_DECL_HTONLL], [0])
fi

AC_SUBST([DEB_VERSION], [0.${VERSION_COMMIT_COUNT}-git-${VERSION_COMMIT_SHA}])

if test "x${host_cpu}" == "xx86_64"; then
  AC_SUBST([DEB_ARCH], [amd64])
fi

AC_CONFIG_FILES([Makefile
                 include/libgeneral/macros.h
                 include/libgeneral/ByteOrder.hpp
                 include/Makefile
                 libgeneral.pc
                 libgeneral/Makefile
                 debian/libgeneral0.control
                 debian/libgeneral-dev.control])

AC_OUTPUT

echo "
Configuration for $PACKAGE-$VERSION:
-------------------------------------------

  install prefix ..........: $prefix
  minimal lib .............: $build_minimal
  debug build  ............: $debug_build
  disable cleanup macro ...: $cleanup_disabled"
echo "  compiler ................: ${CC}

  Now type 'make' to build $PACKAGE-$VERSION,
  and then 'make install' for installation.
"
