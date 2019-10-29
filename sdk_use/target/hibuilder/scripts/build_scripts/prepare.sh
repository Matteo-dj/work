#!/bin/sh -e

set -x
export BASE_DIR=$PWD

mkdir env_prepare
cd env_prepare
tar xf $BASE_DIR/tarball/binutils-2.26.tar.gz
tar xf $BASE_DIR/tarball/gcc-linaro-4.9-2015.06.tar.xz
tar xf $BASE_DIR/tarball/mpfr-3.1.4.tar.xz  -C gcc-linaro-4.9-2015.06/
tar xf $BASE_DIR/tarball/gmp-6.1.0.tar.bz2  -C gcc-linaro-4.9-2015.06/
tar xf $BASE_DIR/tarball/mpc-1.0.3.tar.gz  -C gcc-linaro-4.9-2015.06/
pushd gcc-linaro-4.9-2015.06 
patch -p1 < $BASE_DIR/tarball/gcc-linaro-4.9-2015.06.patch
popd 

mkdir mingw_header
tar xf ../tarball/mingw-runtime-3.14-src.tar.gz  -C mingw_header/
tar xf ../tarball/w32api-3.17-2-mingw32-src.tar.lzma  -C mingw_header/
