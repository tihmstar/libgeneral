#!/bin/bash

test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.

olddir=`pwd`

cd $srcdir
#cleanup cache for correct versioning when run multiple times
rm -rf autom4te.cache

aclocal -I m4
autoconf
autoheader
automake --add-missing
autoreconf -i

cd "$olddir"
if [ -z "$NOCONFIGURE" ]; then
    "$srcdir/configure" "$@"
fi
