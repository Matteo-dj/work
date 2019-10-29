#v500 for windows
#!/bin/sh -e
set -x
export BASE_DIR=$PWD
#echo "$PWD"
export HOST_LIB=$BASE_DIR/win_install/host_lib
export DOC_DIR=$BASE_DIR/win_install/doc_dir
export HOST=i586-pc-mingw32

export TARGET=arm-hisiv500-linux-uclibcgnueabi
export TARDIR=$BASE_DIR/win_install/arm-hisiv500-linux
export TARGET_CROSS_TOOLCHAIN_DIR=$BASE_DIR/install/arm-hisiv500-linux
export SYSROOT=$TARDIR/target
export PATH=$PATH:$HOST_LIB/texinfo/bin
export PATH=$TARDIR/bin:$PATH
export GCC_DIR=gcc-linaro-4.9-2015.06
export BINUTILS_DIR=binutils-2.26

export HOST_CROSS_CC=$BASE_DIR/install-i586-pc-mingw32-cross/bin/i586-pc-mingw32-gcc
export HOST_CROSS_CXX=$BASE_DIR/install-i586-pc-mingw32-cross/bin/i586-pc-mingw32-g++
export HOST_CROSS_AS=$BASE_DIR/install-i586-pc-mingw32-cross/bin/i586-pc-mingw32-as
export HOST_CROSS_AR=$BASE_DIR/install-i586-pc-mingw32-cross/bin/i586-pc-mingw32-ar
export HOST_CROSS_NM=$BASE_DIR/install-i586-pc-mingw32-cross/bin/i586-pc-mingw32-nm
export HOST_CROSS_RANLIB=$BASE_DIR/install-i586-pc-mingw32-cross/bin/i586-pc-mingw32-ranlib

export TARGET_CROSS_CC=${TARGET_CROSS_TOOLCHAIN_DIR}/bin/arm-hisiv500-linux-uclibcgnueabi-gcc
export TARGET_CROSS_CXX=${TARGET_CROSS_TOOLCHAIN_DIR}/bin/arm-hisiv500-linux-uclibcgnueabi-g++
export TARGET_CROSS_AS=${TARGET_CROSS_TOOLCHAIN_DIR}/bin/arm-hisiv500-linux-uclibcgnueabi-as
export TARGET_CROSS_AR=${TARGET_CROSS_TOOLCHAIN_DIR}/bin/arm-hisiv500-linux-uclibcgnueabi-ar
export TARGET_CROSS_NM=${TARGET_CROSS_TOOLCHAIN_DIR}/bin/arm-hisiv500-linux-uclibcgnueabi-nm
export TARGET_CROSS_RANLIB=${TARGET_CROSS_TOOLCHAIN_DIR}/bin/arm-hisiv500-linux-uclibcgnueabi-ranlib
export TARGET_CROSS_OBJDUMP=${TARGET_CROSS_TOOLCHAIN_DIR}/bin/arm-hisiv500-linux-uclibcgnueabi-objdump
export TARGET_CROSS_STRIP=${TARGET_CROSS_TOOLCHAIN_DIR}/bin/arm-hisiv500-linux-uclibcgnueabi-strip

export PARALLEL=128
export SHELL=/bin/bash

export PATH=${TARGET_CROSS_TOOLCHAIN_DIR}/bin:$PATH

CLOOG_DIR=cloog-0.18.4
EXPAT_DIR=expat-2.1.1
GDB_DIR=gdb-7.11
GMP_DIR=gmp-6.1.0
LIBELF_DIR=libelf-0.8.13
LINUX_DIR=linux-3.5.4
MPC_DIR=mpc-1.0.3
MPFR_DIR=mpfr-3.1.4
PPL_DIR=ppl-1.2
TEXINFO_DIR=texinfo-6.1
UCLIBC_DIR=uClibc-0.9.33.2

ISL_DIR=isl-0.12.2
ZLIB_DIR=zlib-1.2.8

BUILD=$(gcc -dumpmachine)

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

#task  [001]   /init/dirs
build_initdir()
{
rm -rf $BASE_DIR/win_install
mkdir -p $HOST_LIB $TARDIR $SYSROOT $DOC_DIR
mkdir -p $SYSROOT/armv5te_arm9_soft
mkdir -p $SYSROOT/armv5te_arm9_vfp
mkdir -p $SYSROOT/a9_soft
mkdir -p $SYSROOT/a9_softfp_vfp
mkdir -p $SYSROOT/a9_softfp_vfpv3-d16
mkdir -p $SYSROOT/a9_softfp_vfpv4
mkdir -p $SYSROOT/a9_softfp_neon-vfpv4
mkdir -p $SYSROOT/a9_hard_neon-vfpv4
mkdir -p $SYSROOT/a7_soft
mkdir -p $SYSROOT/a7_softfp_vfpv4
mkdir -p $SYSROOT/a7_softfp_neon-vfpv4
mkdir -p $SYSROOT/a7_hard_neon-vfpv4
mkdir -p $SYSROOT/a17_soft
mkdir -p $SYSROOT/a17_softfp_vfpv4
mkdir -p $SYSROOT/a17_softfp_neon-vfpv4
mkdir -p $SYSROOT/a17_hard_neon-vfpv4
mkdir -p $SYSROOT/a17_a7_soft
mkdir -p $SYSROOT/a17_a7_softfp_vfpv4
mkdir -p $SYSROOT/a17_a7_softfp_neon-vfpv4
mkdir -p $SYSROOT/a17_a7_hard_neon-vfpv4
}
#task  [002]   /init/cleanup 
build_cleanup()
{
rm -rf $BASE_DIR/$BINUTILS_DIR
rm -rf $BASE_DIR/binutils_build 
rm -rf $BASE_DIR/$CLOOG_DIR
rm -rf $BASE_DIR/cloog_build
rm -rf $BASE_DIR/$EXPAT_DIR
rm -rf $BASE_DIR/expat_build
rm -rf $BASE_DIR/$GCC_DIR
rm -rf $BASE_DIR/gcc_build 
rm -rf $BASE_DIR/finalgcc_build
rm -rf $BASE_DIR/$GDB_DIR
rm -rf $BASE_DIR/gdb_build
rm -rf $BASE_DIR/$GMP_DIR
rm -rf $BASE_DIR/gmp_build
rm -rf $BASE_DIR/$LIBELF_DIR 
rm -rf $BASE_DIR/libelf_build 
rm -rf $BASE_DIR/$LINUX_DIR
rm -rf $BASE_DIR/$MPC_DIR
rm -rf $BASE_DIR/mpc_build
rm -rf $BASE_DIR/$MPFR_DIR
rm -rf $BASE_DIR/mpfr_build
rm -rf $BASE_DIR/$PPL_DIR
rm -rf $BASE_DIR/ppl_build
rm -rf $BASE_DIR/$TEXINFO_DIR
rm -rf $BASE_DIR/texinfo_build
rm -rf $BASE_DIR/$UCLIBC_DIR
rm -rf $BASE_DIR/$ISL_DIR
rm -rf $BASE_DIR/isl_build
#rm -rf $BASE_DIR/$ZLIB_DIR
#rm -rf $BASE_DIR/zlib_build
}
#task  [003]   /init/source_package
build_source_prepare()
{
tar -xf $BASE_DIR/tarball/$BINUTILS_DIR.tar.gz -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$CLOOG_DIR.tar.gz -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$EXPAT_DIR.tar.bz2  -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$GCC_DIR.tar.xz -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$GDB_DIR.tar.gz  -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$GMP_DIR.tar.bz2 -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$LIBELF_DIR.tar.gz -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$LINUX_DIR.tar.gz  -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$MPC_DIR.tar.gz -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$MPFR_DIR.tar.xz -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$PPL_DIR.tar.xz -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$TEXINFO_DIR.tar.gz -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$UCLIBC_DIR.tar.bz2 -C $BASE_DIR
tar -xf $BASE_DIR/tarball/$ISL_DIR.tar.gz -C $BASE_DIR
#tar -xf $BASE_DIR/tarball/$ZLIB_DIR.tar.xz -C $BASE_DIR

pushd $BASE_DIR/$GCC_DIR
patch -p1 < $BASE_DIR/tarball/hisilicon-multilibs-patch_20150831.patch
popd

pushd $BASE_DIR/$LINUX_DIR
patch -p1 <$BASE_DIR/tarball/linux_2012_09.patch
popd

pushd $BASE_DIR/$UCLIBC_DIR
patch -p1 < $BASE_DIR/tarball/uClibc-0.9.33.2_20130620_config.patch
patch -p1 < $BASE_DIR/tarball/uClibc_fwrite_20160223.patch
popd
}
#task  [004]   /build/hostlib/texinfo
build_hostlib_texinfo()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar NM $HOST_CROSS_NM
pushenvvar AS $HOST_CROSS_AS
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
rm $BASE_DIR/texinfo_build -rf
mkdir -p $BASE_DIR/texinfo_build
pushd $BASE_DIR/texinfo_build
../$TEXINFO_DIR/configure --build=$BUILD --host=$HOST --target=$HOST --prefix=$HOST_LIB/texinfo -disable-shared
make -j $PARALLEL
make install
popd
popd texinfo_build
popenv
}

#task  [005]   /build/hostlib/gmp
build_hostlib_gmp()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar NM $HOST_CROSS_NM
pushenvvar AS $HOST_CROSS_AS
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
rm -rf $BASE_DIR/gmp_build
mkdir -p $BASE_DIR/gmp_build
pushd $BASE_DIR/gmp_build
../$GMP_DIR/configure --build=$BUILD  --host=$HOST --target=$HOST --prefix=$HOST_LIB --disable-shared --enable-cxx --enable-nls
make -j $PARALLEL
make install
#make check
popd
popenv
}
 
#task  [006]   /build/hostlib/mpfr
build_hostlib_mpfr()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar NM $HOST_CROSS_NM
pushenvvar AS $HOST_CROSS_AS
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
rm -rf $BASE_DIR/mpfr_build
mkdir -p $BASE_DIR/mpfr_build
pushd $BASE_DIR/mpfr_build
../$MPFR_DIR/configure --build=$BUILD --host=$HOST --target=$TARGET --prefix=$HOST_LIB --disable-shared --enable-nls --with-gmp=$HOST_LIB
make all -j $PARALLEL
make install
#make check
popd
popenv
}
#task  [006]   /build/hostlib/mpc fixme
build_hostlib_mpc()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar NM $HOST_CROSS_NM
pushenvvar AS $HOST_CROSS_AS
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
rm -rf $BASE_DIR/mpc_build
mkdir -p $BASE_DIR/mpc_build
pushd $BASE_DIR/mpc_build
../$MPC_DIR/configure --build=$BUILD --host=$HOST --target=$TARGET --prefix=$HOST_LIB --disable-shared --enable-nls --with-gmp=$HOST_LIB --with-mpfr=$HOST_LIB
make all -j $PARALLEL
make install
#make check
popd
popenv
}
#task  [007]   /build/hostlib/ppl
build_hostlib_ppl()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar NM $HOST_CROSS_NM
pushenvvar AS $HOST_CROSS_AS
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
rm -rf $BASE_DIR/ppl_build
mkdir $BASE_DIR/ppl_build
pushd $BASE_DIR/ppl_build
../$PPL_DIR/configure --build=$BUILD --host=$HOST --target=$TARGET  --prefix=$HOST_LIB  --disable-shared --enable-nls --with-gmp=$HOST_LIB --with-libgmp-prefix=$HOST_LIB --with-gmp-prefix=$HOST_LIB --disable-watchdog
make -j $PARALLEL
make install
popd
popenv
}
#task  [008_0]   /build/hostlib/isl
build_hostlib_isl()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar NM $HOST_CROSS_NM
pushenvvar AS $HOST_CROSS_AS
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
rm -rf $BASE_DIR/isl_build
mkdir $BASE_DIR/isl_build
pushd $BASE_DIR/isl_build
../$ISL_DIR/configure --build=$BUILD --host=$HOST  --prefix=$HOST_LIB --disable-shared --enable-nls --with-gmp-prefix=$HOST_LIB
make -j $PARALLEL
make install
#make check
popd
popenv
}
#task  [008]   /build/hostlib/cloog
build_hostlib_cloog()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar NM $HOST_CROSS_NM
pushenvvar AS $HOST_CROSS_AS
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
rm -rf $BASE_DIR/cloog_build
mkdir $BASE_DIR/cloog_build
pushd $BASE_DIR/cloog_build
../$CLOOG_DIR/configure --build=$BUILD --host=$HOST --target=$TARGET --prefix=$HOST_LIB --disable-shared --enable-nls --with-ppl=$HOST_LIB --with-gmp-prefix=$HOST_LIB --with-isl-prefix=$HOST_LIB
make -j $PARALLEL
make install
#make check
popd
popenv
}
#task  [009]   /build/hostlib/libelf
build_hostlib_libelf()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar NM $HOST_CROSS_NM
pushenvvar AS $HOST_CROSS_AS
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
rm -rf $BASE_DIR/libelf_build
mkdir $BASE_DIR/libelf_build
pushd $BASE_DIR/libelf_build
../$LIBELF_DIR/configure --build=$BUILD --host=$HOST --target=$TARGET --prefix=$HOST_LIB --disable-shared --enable-nls
make -j $PARALLEL
make install
popd
popenv
}
#task  [010]   /build/binutils
build_binutils()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
rm -rf $BASE_DIR/binutils_build
mkdir $BASE_DIR/binutils_build
pushd $BASE_DIR/binutils_build
CFLAGS="-static -g -O2" CFLAGS_FOR_TARGET="-g -O2" ../$BINUTILS_DIR/configure --host=$HOST --build=$BUILD --target=$TARGET --prefix=$TARDIR --enable-nls --with-sysroot=$SYSROOT --enable-poison-system-directories --enable-plugins --disable-werror |tee chh_config.log
#CFLAGS="-static -g -O2" CFLAGS_FOR_TARGET="-g -O2" ../binutils-2012.09/configure --host=$HOST --build=$BUILD --target=$TARGET --prefix=$TARDIR --enable-nls --with-sysroot=$SYSROOT --enable-poison-system-directories '--with-pkgversion=Hisilicon_v500_20160223' --with-bugurl=http://www.hisilicon.com/cn/service/claim.html --enable-plugins --disable-werror |tee chh_config.log
make all -j $PARALLEL |tee chh_make.log
make install prefix=$TARDIR exec_prefix=$TARDIR libdir=$TARDIR/lib htmldir=$DOC_DIR/share/doc/$TARGET/html pdfdir=$DOC_DIR/share/doc/$TARGET/pdf infodir=$DOC_DIR/share/doc/$TARGET/info  mandir=$DOC_DIR/share/doc/$TARGET/man  datadir=$DOC_DIR/share |tee chh_install.log
popd
popenv
}
#task  [011]   /build/kernel/header
build_copy_header()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
pushd $TARDIR
#cp -r ${TARGET_CROSS_TOOLCHAIN_DIR}/target .
popd
popenv
}
#task  [011]   /build/kernel/header
build_kernel_header()
{
pushd $BASE_DIR/$LINUX_DIR
make distclean
make ARCH=arm INSTALL_HDR_PATH=$BASE_DIR/$LINUX_DIR/tmp-install headers_install
mkdir -p $SYSROOT/usr/include
cp -a $BASE_DIR/$LINUX_DIR/tmp-install/include $SYSROOT/usr
make distclean
popd
}
#task  [012]   /build/gcc-first
build_gcc_first()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
pushenv
pushenvvar AR_FOR_TARGET $TARGET_CROSS_AR
pushenvvar NM_FOR_TARGET $TARGET_CROSS_NM
pushenvvar OBJDUMP_FOR_TARGET $TARGET_CROSS_OBJDUMP
pushenvvar STRIP_FOR_TARGET $TARGET_CROSS_STRIP
pushenvvar GCC_FOR_TARGET $TARGET_CROSS_CC

sed -i -r 's/^(typedef __caddr_t caddr_t;)/\/\/\1/' $SYSROOT/usr/include/sys/types.h

rm -rf $BASE_DIR/gcc_build
mkdir $BASE_DIR/gcc_build
pushd $BASE_DIR/gcc_build
LDFLAGS="-lstdc++ -lm"
CFLAGS="-static -g -O2"
CFLAGS_FOR_TARGET="-g -O2"
../$GCC_DIR/configure --host=$HOST --build=$BUILD --target=$TARGET --prefix=$TARDIR  --disable-libmudflap --disable-libstdcxx-pch --with-arch=armv5te --with-gnu-as --with-gnu-ld --enable-lto  --enable-symvers=gnu --enable-__cxa_atexit --enable-nls --enable-clocale=gnu --disable-shared  --enable-extra-hisi-multilibs  --disable-threads --disable-libssp --disable-libgomp --disable-decimal-float --disable-libffi --disable-libquadmath --disable-libitm --disable-libatomic --enable-languages=c --with-sysroot=$SYSROOT --with-gmp=$HOST_LIB --with-mpfr=$HOST_LIB --with-mpc=$HOST_LIB --with-ppl=$HOST_LIB --with-cloog=$HOST_LIB --with-libelf=$HOST_LIB  --disable-libgomp --disable-libitm --disable-libsanitizer --enable-poison-system-directories --without-headers --with-newlib --with-libelf=$HOST_LIB |tee chh_config.log
make -j $PARALLEL |tee chh_make.log
make install prefix=$TARDIR exec_prefix=$TARDIR libdir=$TARDIR/lib htmldir=$DOC_DIR/share/doc/$TARGET/html pdfdir=$DOC_DIR/share/doc/$TARGET/pdf infodir=$DOC_DIR/share/doc/$TARGET/info  mandir=$DOC_DIR/share/doc/$TARGET/man  datadir=$DOC_DIR/share |tee chh_install.log
popd
popenv
popenv
sed -i -r 's/\/\/(typedef __caddr_t caddr_t;)/\1/'  $SYSROOT/usr/include/sys/types.h 
}
#task  [013]   /build/uclibc-0   -march=armv5te -mcpu=arm926ejs 
build_uclibc_0()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp   arm9_soft_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [014]   /build/uclibc-1   -march=armv5te -mcpu=arm926ejs -msoft-float
build_uclibc_1()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=armv5te_arm9_soft/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp   arm9_soft_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [015]   /build/uclibc-2  -march=armv5te -mcpu=arm926ej-s -mfloat-abi=softfp -mfpu=vfp
build_uclibc_2()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a9_soft/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp arm9_vfp_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [017]   /build/uclibc-3   -march=armv7-a -mcpu=cortex-a9 -msoft-float
build_uclibc_3()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a9_softfp_vfp/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a9_soft_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [018]   /build/uclibc-4   -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=vfp
build_uclibc_4()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=armv5te_arm9_vfp/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a9_vfp_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [019]   /build/uclibc-5   -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=vfpv3_d16
build_uclibc_5()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a9_softfp_vfpv3-d16/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a9_vfpv3d16_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}

#task  [021]   /build/uclibc-7   -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=vfpv4
build_uclibc_6()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a9_softfp_vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a9_vfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [022]   /build/uclibc-8   -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=neon-vfpv4
build_uclibc_7()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a9_softfp_neon-vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a9_neonvfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [023]   /build/uclibc-9   -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon-vfpv4
build_uclibc_8()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a9_hard_neon-vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a9_hard_neonvfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}

#task  [020]   /build/uclibc-6   -march=armv7-a -mcpu=cortex-a7 -mfloat-abi=soft
build_uclibc_9()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a7_soft/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a7_soft_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [021]   /build/uclibc-7   -march=armv7-a -mcpu=cortex-a7 -mfloat-abi=softfp -mfpu=vfpv4
build_uclibc_10()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a7_softfp_vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a7_vfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [022]   /build/uclibc-8   -march=armv7-a -mcpu=cortex-a7 -mfloat-abi=softfp -mfpu=neon-vfpv4
build_uclibc_11()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a7_softfp_neon-vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a7_neonvfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [023]   /build/uclibc-9   -march=armv7-a -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4
build_uclibc_12()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a7_hard_neon-vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a7_hard_neonvfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}

#------------------------------------------
#task  [020]   /build/uclibc-6   -march=armv7-a -mcpu=cortex-a17 -mfloat-abi=soft
build_uclibc_13()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a17_soft/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a17_soft_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [021]   /build/uclibc-7   -march=armv7-a -mcpu=cortex-a17 -mfloat-abi=softfp -mfpu=vfpv4
build_uclibc_14()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a17_softfp_vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a17_vfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [022]   /build/uclibc-8   -march=armv7-a -mcpu=cortex-a17 -mfloat-abi=softfp -mfpu=neon-vfpv4
build_uclibc_15()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a17_softfp_neon-vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a17_neonvfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [023]   /build/uclibc-9   -march=armv7-a -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4
build_uclibc_16()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a17_hard_neon-vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a17_hard_neonvfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}

#-----------------------------------------------------
#task  [020]   /build/uclibc-6   -march=armv7-a -mcpu=cortex-a17.cortex-a7 -mfloat-abi=soft
build_uclibc_17()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a17_a7_soft/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a17_a7_soft_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [021]   /build/uclibc-7   -march=armv7-a -mcpu=cortex-a17.cortex-a7 -mfloat-abi=softfp -mfpu=vfpv4
build_uclibc_18()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a17_a7_softfp_vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a17_a7_vfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [022]   /build/uclibc-8   -march=armv7-a -mcpu=cortex-a17.cortex-a7 -mfloat-abi=softfp -mfpu=neon-vfpv4
build_uclibc_19()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a17_a7_softfp_neon-vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a17_a7_neonvfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}
#task  [023]   /build/uclibc-9   -march=armv7-a -mcpu=cortex-a17.cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4
build_uclibc_20()
{
UC_MAKE_CFG="PREFIX=$SYSROOT KERNEL_HEADERS=$SYSROOT/usr/include RUNTIME_PREFIX=/ DEVEL_PREFIX=/usr/lib/ MULTILIB_DIR=a17_a7_hard_neon-vfpv4/ CROSS_COMPILER_PREFIX=arm-hisiv500-linux-uclibcgnueabi-"
pushd $BASE_DIR/$UCLIBC_DIR
make $UC_MAKE_CFG distclean
cp a17_a7_hard_neonvfpv4_config .config
make $UC_MAKE_CFG pregen
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG -j 12
[ $? != 0 ] && exit 1
make $UC_MAKE_CFG install
[ $? != 0 ] && exit 1
popd
}

#task  [023]   /build/finalgcc
build_gcc_final()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar NM $HOST_CROSS_NM
pushenvvar AS $HOST_CROSS_AS
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
sed -i -r 's/^(typedef __caddr_t caddr_t;)/\/\/\1/' $SYSROOT/usr/include/sys/types.h
rm -rf finalgcc_build 
mkdir finalgcc_build
pushd finalgcc_build
LDFLAGS="-lstdc++ -lm" CFLAGS="-static -g -O2" CFLAGS_FOR_TARGET="-g -O2" ../$GCC_DIR/configure --host=$HOST --build=$BUILD --target=$TARGET --prefix=$TARDIR --enable-threads --disable-libmudflap --disable-libssp --disable-libstdcxx-pch --with-arch=armv5te --with-gnu-as --with-gnu-ld --enable-languages=c,c++ --enable-shared --enable-lto --enable-symvers=gnu --enable-__cxa_atexit --enable-nls --enable-clocale=gnu --enable-extra-hisi-multilibs --with-sysroot=$SYSROOT --with-build-sysroot=$SYSROOT --with-gmp=$HOST_LIB --with-mpfr=$HOST_LIB --with-mpc=$HOST_LIB --with-ppl=$HOST_LIB --with-cloog=$HOST_LIB --with-libelf=$HOST_LIB --with-isl=$HOST_LIB --enable-libgomp --disable-libitm --disable-libsanitizer --enable-poison-system-directories --with-libelf=$HOST_LIB |tee chh_config.log
#cp ../finalgcc_build_linux/gcc/insn-opinit.c gcc/insn-opinit.c
#cp ../finalgcc_build_linux/gcc/xgcc gcc/
#cp ../finalgcc_build_linux/fixincludes/fixincl fixincludes/
#cp ../finalgcc_build_linux/gcc/cc1 gcc/
#cp ../finalgcc_build_linux/gcc/cc1plus gcc/
#cp ../finalgcc_build_linux/gcc/cpp gcc/
#cp ../finalgcc_build_linux/gcc/collect2 gcc/

make -j $PARALLEL | tee chh_make.log
make install prefix=$TARDIR exec_prefix=$TARDIR libdir=$TARDIR/lib htmldir=$DOC_DIR/share/doc/$TARGET/html pdfdir=$DOC_DIR/share/doc/$TARGET/pdf infodir=$DOC_DIR/share/doc/$TARGET/info  mandir=$DOC_DIR/share/doc/$TARGET/man  datadir=$DOC_DIR/share |tee chh_install.log
popd
popenv
}

#task  [024]   /build/expat

build_hostlib_expat()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar NM $HOST_CROSS_NM
pushenvvar AS $HOST_CROSS_AS
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
rm -rf $BASE_DIR/expat_build
mkdir -p $BASE_DIR/expat_build
pushd $BASE_DIR/expat_build
../$EXPAT_DIR/configure --build=$BUILD --host=$HOST --target=$HOST --prefix=$HOST_LIB --disable-shared  --enable-nls
make -j $PARALLEL
make install
popd
popenv
}

#task  [025]   /build/gdb
build_hostgdb()
{
pushenv
pushenvvar CC $HOST_CROSS_CC
pushenvvar NM $HOST_CROSS_NM
pushenvvar AS $HOST_CROSS_AS
pushenvvar CXX $HOST_CROSS_CXX
pushenvvar AR $HOST_CROSS_AR
pushenvvar RANLIB $HOST_CROSS_RANLIB
rm -rf gdb_build
mkdir gdb_build
pushd gdb_build
CFLAGS="-static -g -O2" ../$GDB_DIR/configure --build=$BUILD --target=$TARGET --prefix=$SYSROOT --host=$HOST --disable-sim --disable-libmcheck --enable-nls --with-libexpat-prefix=$HOST_LIB  '--with-gdb-datadir=$TARDIR/$TARGET/share/gdb' 
make  -j $PARALLEL
make install prefix=$TARDIR exec_prefix=$TARDIR libdir=$TARDIR/lib htmldir=$DOC_DIR/share/doc/$TARGET/html pdfdir=$DOC_DIR/share/doc/$TARGET/pdf infodir=$DOC_DIR/share/doc/$TARGET/info  mandir=$DOC_DIR/share/doc/$TARGET/man  datadir=$DOC_DIR/share |tee chh_install.log
popd
popenv
}

#ALL='initdir cleanup source_prepare hostlib_texinfo hostlib_gmp hostlib_mpfr hostlib_mpc hostlib_ppl hostlib_isl hostlib_cloog hostlib_libelf binutils copy_header kernel_header gcc_first uclibc_0 uclibc_1 uclibc_2 uclibc_3 uclibc_4 uclibc_5 uclibc_6 uclibc_7 uclibc_8 uclibc_9 uclibc_10 uclibc_11 uclibc_12 uclibc_13 uclibc_14 uclibc_15 uclibc_16 uclibc_17 uclibc_18 uclibc_19 uclibc_20 gcc_final hostlib_expat hostgdb ' 

ALL='gcc_final'

[ $# -eq 0 ] && { $0 $ALL; exit; }

while [ $# -gt 0 ]
do
	echo *****************************build_$1*************************************
	sleep 1
	eval build_$1
	shift
done
#chmod 777 ./build_windows.sh ;./build_windows.sh > log 2>&1
echo "*********************************finish******************************************"

