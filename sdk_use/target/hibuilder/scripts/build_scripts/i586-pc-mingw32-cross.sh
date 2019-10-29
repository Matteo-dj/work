#!/bin/bash
#
# This file contains the complete sequence of commands
# Huawei used to build this version of i586 Linux cross toolchain.
# 
# For each free or open-source component of the cross toolchain,
# the source code provided includes all of the configuration
# scripts and makefiles for that component, including any and
# all modifications made by Huawei. From this list of
# commands, you can see every configuration option used by
# Huawei during the build process.
# 
# This file is provided as a guideline for users who wish to
# modify and rebuild a free or open-source component of
# i586 Linux cross toolchain from source. For a number of reasons, though,
# you may not be able to successfully run this script directly
# on your system. Certain aspects of the Huawei build
# environment (such as directory names) are included in these
# commands. So you may need to modify various configuration options 
# and paths if you are building on your Linux system. 

set -e
inform_fd=2 
umask 022
exec < /dev/null

error_handler()
{
    exit 1
}

check_status() {
    local status="$?"
    if [ "$status" -ne 0 ]; then
	error_handler
    fi
}

check_pipe() {
    local -a status=("${PIPESTATUS[@]}")
    local limit=$1
    local ix
    if [ -z "$limit" ] ; then
	limit="${#status[@]}"
    fi
    for ((ix=0; ix != $limit ; ix++)); do
	if [ "${status[$ix]}" != "0" ]; then
	    error_handler
	fi
    done
}

error() {
    echo "$script: error: $@" >& $inform_fd
    exit 1
}

warning() {
    echo "$script: warning: $@" >& $inform_fd
}

verbose() {
    if $gnu_verbose; then
	echo "$script: $@" >& $inform_fd
    fi
}

copy_dir() {
    mkdir -p "$2"
    (cd "$1" && tar cf - .) | (cd "$2" && tar xf -)
    check_pipe
}

copy_dir_clean() {
    mkdir -p "$2"
    (cd "$1" && tar cf - \
	--exclude=CVS --exclude=.svn --exclude=.git --exclude=.pc \
	--exclude="*~" --exclude=".#*" \
	--exclude="*.orig" --exclude="*.rej" \
	.) | (cd "$2" && tar xf -)
    check_pipe
}

update_dir_clean() {
    mkdir -p "$2"
    (cd "$1" && tar cf - \
	--exclude=CVS --exclude=.svn --exclude=.git --exclude=.pc \
	--exclude="*~" --exclude=".#*" \
	--exclude="*.orig" --exclude="*.rej" \
	--after-date="$3" \
	. 2> /dev/null) | (cd "$2" && tar xf -)
    check_pipe
}

copy_dir_exclude() {
    local source="$1"
    local dest="$2"
    local excl="$3"
    shift 3
    mkdir -p "$dest"
    (cd "$source" && tar cfX - "$excl" "$@") | (cd "$dest" && tar xf -)
    check_pipe
}

copy_dir_only() {
    local source="$1"
    local dest="$2"
    shift 2
    mkdir -p "$dest"
    (cd "$source" && tar cf - "$@") | (cd "$dest" && tar xf -)
    check_pipe
}

pushenv() {
    pushenv_level=$(($pushenv_level + 1))
    eval pushenv_vars_${pushenv_level}=
}

pushenv_level=0
pushenv_vars_0=

pushenvvar() {
    local pushenv_var="$1"
    local pushenv_newval="$2"
    eval local pushenv_oldval=\"\$$pushenv_var\"
    eval local pushenv_oldset=\"\${$pushenv_var+set}\"
    local pushenv_save_var=saved_${pushenv_level}_${pushenv_var}
    local pushenv_savep_var=savedp_${pushenv_level}_${pushenv_var}
    eval local pushenv_save_set=\"\${$pushenv_savep_var+set}\"
    if [ "$pushenv_save_set" = "set" ]; then
	error "Pushing $pushenv_var more than once at level $pushenv_level"
    fi
    if [ "$pushenv_oldset" = "set" ]; then
	eval $pushenv_save_var=\"\$pushenv_oldval\"
    else
	unset $pushenv_save_var
    fi
    eval $pushenv_savep_var=1
    eval export $pushenv_var=\"\$pushenv_newval\"
    local pushenv_list_var=pushenv_vars_${pushenv_level}
    eval $pushenv_list_var=\"\$$pushenv_list_var \$pushenv_var\"
}

prependenvvar() {
    local pushenv_var="$1"
    local pushenv_newval="$2"
    eval local pushenv_oldval=\"\$$pushenv_var\"
    pushenvvar "$pushenv_var" "$pushenv_newval$pushenv_oldval"
}

popenv() {
    local pushenv_var=
    eval local pushenv_vars=\"\$pushenv_vars_${pushenv_level}\"
    for pushenv_var in $pushenv_vars; do
	local pushenv_save_var=saved_${pushenv_level}_${pushenv_var}
	local pushenv_savep_var=savedp_${pushenv_level}_${pushenv_var}
	eval local pushenv_save_val=\"\$$pushenv_save_var\"
	eval local pushenv_save_set=\"\${$pushenv_save_var+set}\"
	unset $pushenv_save_var
	unset $pushenv_savep_var
	if [ "$pushenv_save_set" = "set" ]; then
	    eval export $pushenv_var=\"\$pushenv_save_val\"
	else
	    unset $pushenv_var
	fi
    done
    unset pushenv_vars_${pushenv_level}
    if [ "$pushenv_level" = "0" ]; then
	error "Popping environment level 0"
    else
	pushenv_level=$(($pushenv_level - 1))
    fi
}

prepend_path() {
    if $(eval "test -n \"\$$1\""); then
	prependenvvar "$1" "$2:"
    else
	prependenvvar "$1" "$2"
    fi
}

# Version info
GMP=gcc-linaro-4.9-2015.06/gmp-6.1.0
MPFR=gcc-linaro-4.9-2015.06/mpfr-3.1.4
MPC=gcc-linaro-4.9-2015.06/mpc-1.0.3
BINUTILS=binutils-2.26
GCC=gcc-linaro-4.9-2015.06
W32API=mingw_header/w32api-3.17-2-mingw32
MINGWRT=mingw_header/mingw-runtime-3.14

BUILD=$(gcc -dumpmachine)
HOST=$(gcc -dumpmachine)
TARGET=i586-pc-mingw32

ROOT_DIR=`pwd`/env_prepare
PREFIX=${ROOT_DIR}/../install-i586-pc-mingw32-cross
PKG_VERSION='SDK_V100R005C00B901[revision 124146]'

# Environment variables that never change during the build process
export CFLAGS_FOR_TARGET="-g -O2"
export CXXFLAGS_FOR_TARGET="$CFLAGS_FOR_TARGET"
export LDFLAGS_FOR_TARGET="--sysroot=${PREFIX}/sysroot"

echo "Initiating directories..."

# task [001/062] Initiating directories...
pushenv
pushenvvar CC_FOR_BUILD gcc
rm -rf $ROOT_DIR/../obj
rm -rf $PREFIX
mkdir -p $ROOT_DIR/../obj
popenv


echo "Building gmp..."

echo "Configure gmp..."
##task [014/062] Configure gmp
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenv
pushenvvar CFLAGS '-g -O2'
rm -rf $ROOT_DIR/../obj/build-gmp
mkdir -p $ROOT_DIR/../obj/build-gmp
pushd $ROOT_DIR/../obj/build-gmp
$ROOT_DIR/$GMP/configure --build=$BUILD --host=$HOST --prefix=${PREFIX}/host-libs/usr --disable-shared --enable-cxx
popd
popenv
popenv
popenv

echo "Build gmp..."
#task [015/062] Builde gmp
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
echo $PATH
prepend_path PATH ${PREFIX}/bin
echo $PATH
pushenv
pushenv
pushenvvar CFLAGS '-g -O2'
pushd $ROOT_DIR/../obj/build-gmp
make -j4
popd
popenv
popenv
popenv

echo "Install gmp..."
#task [016/062] Install gmp
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenv
pushenvvar CFLAGS '-g -O2'
pushd $ROOT_DIR/../obj/build-gmp
make install
popd
popenv
popenv
popenv

# echo "Check gmp..."
# #task [017/062] Check gmp
# pushenv
# pushenvvar CC_FOR_BUILD gcc
# pushenvvar CC gcc
# pushenvvar CXX g++
# pushenvvar AR ar
# pushenvvar RANLIB ranlib
# prepend_path PATH ${PREFIX}/bin
# pushenv
# pushenv
# pushenvvar CFLAGS '-g -O2'
# pushd $ROOT_DIR/../obj/build-gmp
# make  check
# popd
# popenv
# popenv
# popenv

echo "Build gmp done!"


echo "Building mpfr..."

echo "Configure mpfr..."
#task [018/062] Configure mpfr
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenv
rm -rf $ROOT_DIR/../obj/build-mpfr
mkdir -p $ROOT_DIR/../obj/build-mpfr
pushd $ROOT_DIR/../obj/build-mpfr
$ROOT_DIR/$MPFR/configure --build=$BUILD --host=$HOST --prefix=${PREFIX}/host-libs/usr --disable-shared --with-gmp=${PREFIX}/host-libs/usr
popd
popenv
popenv
popenv

echo "Build mpfr..."
#task [019/062] Build mpfr
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenv
pushd $ROOT_DIR/../obj/build-mpfr
make -j4
popd
popenv
popenv
popenv

echo "Install mpfr..."
#task [020/062] Install mpfr
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenv
pushd $ROOT_DIR/../obj/build-mpfr
make install
popd
popenv
popenv
popenv

# echo "Check mpfr..."
# #task [021/062] Check mpfr
# pushenv
# pushenvvar CC_FOR_BUILD gcc
# pushenvvar CC gcc
# pushenvvar CXX g++
# pushenvvar AR ar
# pushenvvar RANLIB ranlib
# prepend_path PATH ${PREFIX}/bin
# pushenv
# pushenv
# pushd $ROOT_DIR/../obj/build-mpfr
# make  check
# popd
# popenv
# popenv
# popenv

echo "Build mpfr done!"


echo "Building mpc..."

echo "Configure mpc..."
#task [022/062] Configure mpc
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenv
rm -rf $ROOT_DIR/../obj/build-mpc
mkdir -p $ROOT_DIR/../obj/build-mpc
pushd $ROOT_DIR/../obj/build-mpc
$ROOT_DIR/$MPC/configure --build=$BUILD --host=$HOST --prefix=${PREFIX}/host-libs/usr --disable-shared --with-gmp=${PREFIX}/host-libs/usr --with-mpfr=${PREFIX}/host-libs/usr
popd
popenv
popenv
popenv

echo "Build mpc..."
#task [023/062] Build mpc
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenv
pushd $ROOT_DIR/../obj/build-mpc
make -j4
popd
popenv
popenv
popenv

echo "Install mpc..."
#task [024/062] Install mpc
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenv
pushd $ROOT_DIR/../obj/build-mpc
make install
popd
popenv
popenv
popenv

# echo "Check mpc..."
# #task [025/062] Check mpc
# pushenv
# pushenvvar CC_FOR_BUILD gcc
# pushenvvar CC gcc
# pushenvvar CXX g++
# pushenvvar AR ar
# pushenvvar RANLIB ranlib
# prepend_path PATH ${PREFIX}/bin
# pushenv
# pushenv
# pushd $ROOT_DIR/../obj/build-mpc
# make  check
# popd
# popenv
# popenv
# popenv

echo "Build mpc done!"
 
 
echo "Building binutils..."

echo "Configure binutils..."
# task [037/062] Configure binutils
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenv
pushenvvar CPPFLAGS -I${PREFIX}/host-libs/usr/include
pushenvvar LDFLAGS -L${PREFIX}/host-libs/usr/lib
rm -rf $ROOT_DIR/../obj/build-binutils
mkdir -p $ROOT_DIR/../obj/build-binutils
pushd $ROOT_DIR/../obj/build-binutils
$ROOT_DIR/$BINUTILS/configure --build=$BUILD --host=$HOST --target=$TARGET --prefix=${PREFIX} --with-sysroot=${PREFIX}/sysroot --with-pkgversion="${PKG_VERSION}" --with-gmp=${PREFIX}/host-libs/usr --with-mpfr=${PREFIX}/host-libs/usr --with-mpc=${PREFIX}/host-libs/usr --with-host-libstdcxx="-lstdc++ -lsupc++"
popd
popenv
popenv
popenv


echo "Build binutils..."
# task [038/062] Build binutils
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenv
pushenvvar CPPFLAGS -I${PREFIX}/host-libs/usr/include
pushenvvar LDFLAGS -L${PREFIX}/host-libs/usr/lib
pushd $ROOT_DIR/../obj/build-binutils
make -j4
popd
popenv
popenv
popenv

echo "Install binutils..."
# task [039/062] Install binutils
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenv
pushenvvar CPPFLAGS -I${PREFIX}/host-libs/usr/include
pushenvvar LDFLAGS -L${PREFIX}/host-libs/usr/lib
pushd $ROOT_DIR/../obj/build-binutils
make install
popd
popenv
popenv
popenv

echo "binutils done!!"


echo "Creating sysroot for header..."

#task [041/062] Creating sysroot for glibc
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
mkdir -p ${PREFIX}/sysroot
popenv
echo "done"


echo "Installing kernel headers..."

#task [040/062] Installing kernel headers
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushd $ROOT_DIR/$W32API/include
mkdir -p ${PREFIX}/sysroot/usr/include
cp -r * ${PREFIX}/sysroot/usr/include
popd
popenv
	

echo "Installing running headers..."

# task [040/062] Installing kernel headers
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushd $ROOT_DIR/$MINGWRT/include
mkdir -p ${PREFIX}/sysroot/usr/include
cp -r * ${PREFIX}/sysroot/usr/include
popd
popenv

echo "header done!!!"


# task [012/062] 将头文件拷贝到${PREFIX}/i586-pc-mingw32
pushenv
pushenvvar CC_FOR_BUILD gcc
pushd ${PREFIX}/$TARGET
cp -r ${PREFIX}/sysroot/usr/include .
popd
popenv


echo "Building GCC first..."

echo "Configure GCC first..."
# task [042/062] Configure GCC first
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenvvar AR_FOR_TARGET $TARGET-ar
pushenvvar NM_FOR_TARGET $TARGET-nm
pushenvvar OBJDUMP_FOR_TARGET $TARGET-objdump
pushenvvar STRIP_FOR_TARGET $TARGET-strip
rm -rf $ROOT_DIR/../obj/build-gcc-first
mkdir -p $ROOT_DIR/../obj/build-gcc-first
pushd $ROOT_DIR/../obj/build-gcc-first
$ROOT_DIR/$GCC/configure --build=$BUILD --host=$HOST --target=$TARGET --disable-win32-registry --disable-libssp --prefix=${PREFIX} --disable-shared --disable-threads --without-headers --with-newlib --enable-languages=c --with-pkgversion="${PKG_VERSION}" --with-gmp=${PREFIX}/host-libs/usr --with-mpfr=${PREFIX}/host-libs/usr --with-mpc=${PREFIX}/host-libs/usr
popd
popenv
popenv

echo "Build GCC first..."
# task [043/062] Build GCC first
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenvvar AR_FOR_TARGET $TARGET-ar
pushenvvar NM_FOR_TARGET $TARGET-nm
pushenvvar OBJDUMP_FOR_TARGET $TARGET-objdump
pushenvvar STRIP_FOR_TARGET $TARGET-strip
pushd $ROOT_DIR/../obj/build-gcc-first
make -j4 CPPFLAGS_FOR_TARGET=--sysroot=${PREFIX}/sysroot build_tooldir=${PREFIX}/$TARGET 
popd
popenv
popenv

echo "Install GCC first..."
# task [044/062] Install GCC first
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenvvar AR_FOR_TARGET $TARGET-ar
pushenvvar NM_FOR_TARGET $TARGET-nm
pushenvvar OBJDUMP_FOR_TARGET $TARGET-objdump
pushenvvar STRIP_FOR_TARGET $TARGET-strip
pushd $ROOT_DIR/../obj/build-gcc-first
make install prefix=${PREFIX} exec_prefix=${PREFIX} libdir=${PREFIX}/lib
popd
popenv
popenv
echo "gcc first done!!"



echo "Installing w32api headers for 32 bit..."

echo "Configure w32api headers for 32 bit..."
#task [045/062] Configure eglibc headers for 32 bit
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
mkdir -p ${PREFIX}/sysroot/lib/ ${PREFIX}/sysroot/usr/lib/
rm -rf $ROOT_DIR/../obj/glibc-headers
mkdir -p $ROOT_DIR/../obj/glibc-headers
pushd "$ROOT_DIR/../obj/glibc-headers"
pushenv
pushenvvar BUILD_CC gcc
pushenvvar CC "${PREFIX}/bin/$TARGET-gcc -g -O2 "
pushenvvar CXX "${PREFIX}/bin/$TARGET-g++ -g -O2 "
pushenvvar CFLAGS ""
pushenvvar AS "${PREFIX}/bin/$TARGET-as"
pushenvvar LD "${PREFIX}/bin/$TARGET-ld"
pushenvvar AR "${PREFIX}/bin/$TARGET-ar"
pushenvvar RANLIB "${PREFIX}/bin/$TARGET-ranlib"
pushenvvar PERL /usr/bin/perl
pushenvvar BASH_SHELL /bin/bash
$ROOT_DIR/$W32API/configure --host=$TARGET --prefix=${PREFIX}/$TARGET
make -j4
make install
popenv
popd
popenv

echo "Installing w32api headers done!"


echo "Installing running headers for 32 bit..."

echo "Configure running headers for 32 bit..."
#task [045/062] Configure eglibc headers for 32 bit
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
mkdir -p ${PREFIX}/sysroot/lib/ ${PREFIX}/sysroot/usr/lib/
rm -rf $ROOT_DIR/../obj/glibc-headers
mkdir -p $ROOT_DIR/../obj/glibc-headers
pushd "$ROOT_DIR/../obj/glibc-headers"
pushenv
pushenvvar BUILD_CC gcc
pushenvvar CC "${PREFIX}/bin/$TARGET-gcc -g -O2 -I${PREFIX}/sysroot/usr/include"
pushenvvar CXX "${PREFIX}/bin/$TARGET-g++ -g -O2 "
pushenvvar CFLAGS ""
pushenvvar AS "${PREFIX}/bin/$TARGET-as"
pushenvvar LD "${PREFIX}/bin/$TARGET-ld"
pushenvvar AR "${PREFIX}/bin/$TARGET-ar"
pushenvvar RANLIB "${PREFIX}/bin/$TARGET-ranlib"
pushenvvar PERL /usr/bin/perl
pushenvvar BASH_SHELL /bin/bash
$ROOT_DIR/$MINGWRT/configure --host=$TARGET --prefix=${PREFIX}/$TARGET
make 
make install
popenv
popd
popenv

echo "Installing running headers done!"




# task [012/062] 将头文件拷贝到${PREFIX}/i586-pc-mingw32
pushenv
pushenvvar CC_FOR_BUILD gcc
mkdir -p ${PREFIX}/sysroot/mingw
pushd ${PREFIX}/sysroot/mingw
cp -r ${PREFIX}/sysroot/usr/include .
popd
popenv

# task [012/062] 将头文件拷贝到${PREFIX}/i586-pc-mingw32

pushenv
pushenvvar CC_FOR_BUILD gcc
mkdir -p $ROOT_DIR/../obj/build-gcc-final/$TARGET/winsup/mingw
pushd $ROOT_DIR/../obj/build-gcc-final/$TARGET/winsup/mingw 
cp -r ${PREFIX}/sysroot/usr/lib   .
popd
popenv


echo "Building GCC final..."

echo "Configure GCC final..."
# task [058/062] Configure GCC final...
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenvvar AR_FOR_TARGET $TARGET-ar
pushenvvar NM_FOR_TARGET $TARGET-nm
pushenvvar OBJDUMP_FOR_TARGET $TARGET-objdump
pushenvvar STRIP_FOR_TARGET $TARGET-strip
rm -rf $ROOT_DIR/../obj/build-gcc-final
mkdir -p $ROOT_DIR/../obj/build-gcc-final
pushd $ROOT_DIR/../obj/build-gcc-final
$ROOT_DIR/$GCC/configure --build=$BUILD --host=$HOST --target=$TARGET --prefix=${PREFIX} --enable-shared --enable-threads --enable-languages=c,c++ --with-sysroot=${PREFIX}/sysroot --with-build-sysroot=${PREFIX}/sysroot --with-pkgversion="${PKG_VERSION}" --with-gmp=${PREFIX}/host-libs/usr --with-mpfr=${PREFIX}/host-libs/usr --with-mpc=${PREFIX}/host-libs/usr --with-host-libstdcxx="-lstdc++ -lsupc++" --with-build-time-tools=${PREFIX}/$TARGET/bin
popd
popenv
popenv

echo "Build GCC final..."
# task [059/062] Build GCC final...
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenvvar AR_FOR_TARGET $TARGET-ar
pushenvvar NM_FOR_TARGET $TARGET-nm
pushenvvar OBJDUMP_FOR_TARGET $TARGET-objdump
pushenvvar STRIP_FOR_TARGET $TARGET-strip
pushd $ROOT_DIR/../obj/build-gcc-final
make -j4 CPPFLAGS_FOR_TARGET=--sysroot=${PREFIX}/sysroot build_tooldir=${PREFIX}/$TARGET
popd
popenv
popenv

echo "Install GCC final..."
# task [060/062] Install GCC final...
pushenv
pushenvvar CC_FOR_BUILD gcc
pushenvvar CC gcc
pushenvvar CXX g++
pushenvvar AR ar
pushenvvar RANLIB ranlib
prepend_path PATH ${PREFIX}/bin
pushenv
pushenvvar AR_FOR_TARGET $TARGET-ar
pushenvvar NM_FOR_TARGET $TARGET-nm
pushenvvar OBJDUMP_FOR_TARGET $TARGET-objdump
pushenvvar STRIP_FOR_TARGET $TARGET-strip
pushd $ROOT_DIR/../obj/build-gcc-final
make install prefix=${PREFIX} exec_prefix=${PREFIX} libdir=${PREFIX}/lib
popd
popenv
popenv

echo "Build gcc final done!"
