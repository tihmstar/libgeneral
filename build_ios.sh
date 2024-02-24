
export HOST=arm-apple-darwin

export TARGET_OS=ios
export ARCH_FLAGS="-arch arm64 -arch arm64e"

if [ -z "${PREFIX}" ]; then
    echo "Error ios builroot PREFIX needs to be defined"
    exit 1
fi

if [ "x${TARGET_OS}" == "xios" ]; then
    if [ -z "${SDKPATH}" ]; then
        echo -e "SDKPATH not set, autodetecting...\n"
        export SDKPATH=$(xcrun --sdk iphoneos --show-sdk-path)
    fi
# elif [ "x${TARGET_OS}" == "xwatchos" ]; then
# elif [ "x${TARGET_OS}" == "xappletvos" ]; then
# elif [ "x${TARGET_OS}" == "xbridgeos" ]; then
else
    echo "ERROR: unexpected OS: ${TARGET_OS}"
    exit 1
fi

export SDK_VERSION_MAJOR=$(echo $SDKPATH | rev | cut -d'/' -f1 | rev | cut -d '.' -f1 | grep -oE '[0-9]+')
export MIN_IOS_VERSION="${SDK_VERSION_MAJOR}.0"
if [ "x${TARGET_OS}" == "xios" ]; then
    export CC="clang -miphoneos-version-min=${MIN_IOS_VERSION} -isysroot ${SDKPATH} ${ARCH_FLAGS}"
    export CXX="clang++ -miphoneos-version-min=${MIN_IOS_VERSION} -isysroot ${SDKPATH} ${ARCH_FLAGS}"
    export OBJC="clang -miphoneos-version-min=${MIN_IOS_VERSION} -isysroot ${SDKPATH} ${ARCH_FLAGS}"
    export OBJCXX="clang++ -miphoneos-version-min=${MIN_IOS_VERSION} -isysroot ${SDKPATH} ${ARCH_FLAGS}"
    export LD="ld -miphoneos-version-min=${MIN_IOS_VERSION} -isysroot ${SDKPATH} ${ARCH_FLAGS}"
# elif [ "x${TARGET_OS}" == "xwatchos" ]; then
#     export HOST_FLAGS="-mwatchos-version-min=${MIN_IOS_VERSION} -isysroot ${SDKPATH}"
# elif [ "x${TARGET_OS}" == "xappletvos" ]; then
#     export HOST_FLAGS="-mtvos-version-min=${MIN_IOS_VERSION} -isysroot ${SDKPATH}"
# elif [ "x${TARGET_OS}" == "xbridgeos" ]; then
#     export HOST_FLAGS="-mbridgeos-version-min=${MIN_IOS_VERSION} -isysroot ${SDKPATH}"
else
    echo "ERROR: unexpected OS: ${TARGET_OS}"
    exit 1
fi

export PKG_CONFIG_PATH="${PREFIX}/usr/local/lib/pkgconfig"
export PKG_CONFIG_LIBDIR="${PREFIX}/usr/local/lib/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="${PREFIX}"

echo "PREFIX=${PREFIX}"
echo "SDKPATH=${SDKPATH}"
echo "MIN_IOS_VERSION=${MIN_IOS_VERSION}"
echo "PKG_CONFIG_PATH=${PKG_CONFIG_PATH}"

export CHOST=${HOST}
export CFLAGS="${TARGET_CFLAGS} ${HOST_FLAGS}"
export CXXFLAGS="${TARGET_CXXFLAGS} ${HOST_FLAGS}"
export OBJCFLAGS="${TARGET_OBJCFLAGS} ${HOST_FLAGS}"
export OBJCXXFLAGS="${TARGET_OBJCXXFLAGS} ${HOST_FLAGS}"
export LDFLAGS="${TARGET_LDFLAGS} ${HOST_FLAGS}"
# export ARFLAGS="${TARGET_ARFLAGS} ${HOST_FLAGS}"

make distclean
./autogen.sh --with-sysroot=${PREFIX} --host=${HOST} $@ || exit
make -j || exit
make install DESTDIR=${PREFIX}