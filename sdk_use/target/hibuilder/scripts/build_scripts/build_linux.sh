# crosstool for linux
#!/bin/sh

export BASE_DIR=$PWD
export HOST_LIB=$BASE_DIR/install/host_lib
export DOC_DIR=$BASE_DIR/install/doc_dir
export HOST=x86_64-linux-gnu
#export HOST=i386-redhat-linux
export TARGET=arm-hisiv500-linux-uclibcgnueabi
export TARDIR=$BASE_DIR/install/arm-hisiv500-linux
export SYSROOT=$TARDIR/target
export PATH=$HOST_LIB/texinfo/bin:$PATH
export PATH=$TARDIR/bin:$PATH
export PARALLEL=128
export SHELL=/bin/bash

BINUTILS_DIR=binutils-2.26
CLOOG_DIR=cloog-0.18.4
EXPAT_DIR=expat-2.1.1
GCC_DIR=gcc-linaro-4.9-2015.06
GDB_DIR=gdb-7.11
GMP_DIR=gmp-6.1.0
LIBELF_DIR=libelf-0.8.13
LINUX_DIR=linux-3.5.4
MPC_DIR=mpc-1.0.3
MPFR_DIR=mpfr-3.1.4
PPL_DIR=ppl-1.2
TEXINFO_DIR=texinfo-6.1
UCLIBC_DIR=uClibc-0.9.33.2

#task  [001]   /init/dirs
build_initdir()
{
rm -rf $BASE_DIR/install
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
rm -rf $BASE_DIR/gdb-build
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
rm $BASE_DIR/texinfo_build -rf
mkdir -p $BASE_DIR/texinfo_build
pushd $BASE_DIR/texinfo_build
../$TEXINFO_DIR/configure --build=$HOST --host=$HOST --target=$HOST  --prefix=$HOST_LIB/texinfo -disable-shared
make -j $PARALLEL
make install
popd
}

#task  [005]   /build/hostlib/gmp
build_hostlib_gmp()
{
rm -rf $BASE_DIR/gmp_build
mkdir -p $BASE_DIR/gmp_build
pushd $BASE_DIR/gmp_build
../$GMP_DIR/configure --build=$HOST --host=$HOST --target=$HOST  --prefix=$HOST_LIB --disable-shared --enable-cxx --enable-nls
make -j $PARALLEL
make install
make check
popd
}
 
#task  [006]   /build/hostlib/mpfr
build_hostlib_mpfr()
{
rm -rf $BASE_DIR/mpfr_build
mkdir -p $BASE_DIR/mpfr_build
pushd $BASE_DIR/mpfr_build
../$MPFR_DIR/configure --build=$HOST --host=$HOST --target=$TARGET --prefix=$HOST_LIB --disable-shared --enable-nls --with-gmp=$HOST_LIB
make all -j $PARALLEL
make install
make check
popd
}
#task  [006]   /build/hostlib/mpc fixme
build_hostlib_mpc()
{
rm -rf $BASE_DIR/mpc_build
mkdir -p $BASE_DIR/mpc_build
pushd $BASE_DIR/mpc_build
../$MPC_DIR/configure --build=$HOST --host=$HOST --target=$TARGET --prefix=$HOST_LIB --disable-shared --enable-nls --with-gmp=$HOST_LIB --with-mpfr=$HOST_LIB
make all -j $PARALLEL
make install
make check
popd
}
#task  [007]   /build/hostlib/ppl
build_hostlib_ppl()
{
rm -rf $BASE_DIR/ppl_build
mkdir $BASE_DIR/ppl_build
pushd $BASE_DIR/ppl_build
../$PPL_DIR/configure --build=$HOST --host=$HOST  --target=$TARGET --prefix=$HOST_LIB --disable-shared --enable-nls --with-libgmp-prefix=$HOST_LIB --with-gmp-prefix=$HOST_LIB --disable-watchdog
make -j $PARALLEL
make install
popd
}
#task  [008]   /build/hostlib/cloog
build_hostlib_cloog()
{
rm -rf $BASE_DIR/cloog_build
mkdir $BASE_DIR/cloog_build
pushd $BASE_DIR/cloog_build
../$CLOOG_DIR/configure --build=$HOST --host=$HOST  --target=$TARGET --prefix=$HOST_LIB --disable-shared --enable-nls --with-ppl=$HOST_LIB --with-gmp=$HOST_LIB
make -j $PARALLEL
make install
make check
popd
}
#task  [009]   /build/hostlib/libelf
build_hostlib_libelf()
{
rm -rf $BASE_DIR/libelf_build
mkdir $BASE_DIR/libelf_build
pushd $BASE_DIR/libelf_build
../$LIBELF_DIR/configure --build=$HOST --host=$HOST --target=$TARGET  --prefix=$HOST_LIB --disable-shared --enable-nls
make -j $PARALLEL
make install
popd
}
#task  [010]   /build/binutils
build_binutils()
{
rm -rf $BASE_DIR/binutils_build
mkdir $BASE_DIR/binutils_build
pushd $BASE_DIR/binutils_build
CFLAGS="-static -g -O2" CFLAGS_FOR_TARGET="-g -O2" ../$BINUTILS_DIR/configure --host=$HOST --build=$HOST --target=$TARGET --prefix=$TARDIR --enable-nls --with-sysroot=$SYSROOT --enable-poison-system-directories --enable-plugins --disable-werror |tee chh_config.log
#CFLAGS="-static -g -O2" CFLAGS_FOR_TARGET="-g -O2" ../binutils-2012.09/configure --host=$HOST --build=$HOST --target=$TARGET --prefix=$TARDIR --enable-nls --with-sysroot=$SYSROOT --enable-poison-system-directories '--with-pkgversion=Hisilicon_v500_20160223' --with-bugurl=http://www.hisilicon.com/cn/service/claim.html --enable-plugins --disable-werror |tee chh_config.log
make all -j $PARALLEL |tee chh_make.log
make install prefix=$TARDIR exec_prefix=$TARDIR libdir=$TARDIR/lib htmldir=$DOC_DIR/share/doc/$TARGET/html pdfdir=$DOC_DIR/share/doc/$TARGET/pdf infodir=$DOC_DIR/share/doc/$TARGET/info  mandir=$DOC_DIR/share/doc/$TARGET/man  datadir=$DOC_DIR/share |tee chh_install.log
popd
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

rm -rf $BASE_DIR/gcc_build
mkdir $BASE_DIR/gcc_build
pushd $BASE_DIR/gcc_build
LDFLAGS="-lstdc++ -lm"
CFLAGS="-static -g -O2"
CFLAGS_FOR_TARGET="-g -O2"
../$GCC_DIR/configure --host=$HOST --build=$HOST --target=$TARGET --prefix=$TARDIR  --disable-libmudflap --disable-libstdcxx-pch --with-arch=armv5te --with-gnu-as --with-gnu-ld --enable-lto  --enable-symvers=gnu --enable-__cxa_atexit --enable-nls --enable-clocale=gnu --disable-shared --enable-extra-hisi-multilibs --disable-threads --disable-libssp --disable-libgomp --disable-decimal-float --disable-libffi --disable-libquadmath --disable-libitm --disable-libatomic --enable-languages=c --with-sysroot=$SYSROOT  --with-gmp=$HOST_LIB --with-mpfr=$HOST_LIB --with-mpc=$HOST_LIB --with-ppl=$HOST_LIB --with-cloog=$HOST_LIB --with-libelf=$HOST_LIB  --disable-libgomp --disable-libitm --disable-libsanitizer --enable-poison-system-directories --without-headers --with-newlib --with-libelf=$HOST_LIB |tee chh_config.log
make -j $PARALLEL |tee chh_make.log
make install prefix=$TARDIR exec_prefix=$TARDIR libdir=$TARDIR/lib htmldir=$DOC_DIR/share/doc/$TARGET/html pdfdir=$DOC_DIR/share/doc/$TARGET/pdf infodir=$DOC_DIR/share/doc/$TARGET/info  mandir=$DOC_DIR/share/doc/$TARGET/man  datadir=$DOC_DIR/share |tee chh_install.log
popd
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
rm -rf finalgcc_build
mkdir finalgcc_build
pushd finalgcc_build
LDFLAGS="-lstdc++ -lm" CFLAGS="-static -g -O2" CFLAGS_FOR_TARGET="-g -O2" ../$GCC_DIR/configure --host=$HOST --build=$HOST --target=$TARGET --prefix=$TARDIR --enable-threads --disable-libmudflap --disable-libssp --disable-libstdcxx-pch --with-arch=armv5te --with-gnu-as --with-gnu-ld --enable-languages=c,c++ --enable-shared --enable-lto --enable-symvers=gnu --enable-__cxa_atexit --enable-nls --enable-clocale=gnu --enable-extra-hisi-multilibs --with-sysroot=$SYSROOT --with-build-sysroot=$SYSROOT  --with-gmp=$HOST_LIB --with-mpfr=$HOST_LIB --with-mpc=$HOST_LIB --with-ppl=$HOST_LIB --with-cloog=$HOST_LIB --with-libelf=$HOST_LIB  --enable-libgomp --disable-libitm --disable-libsanitizer --enable-poison-system-directories --with-libelf=$HOST_LIB |tee chh_config.log
make -j $PARALLEL |tee chh_make.log
make install prefix=$TARDIR exec_prefix=$TARDIR libdir=$TARDIR/lib htmldir=$DOC_DIR/share/doc/$TARGET/html pdfdir=$DOC_DIR/share/doc/$TARGET/pdf infodir=$DOC_DIR/share/doc/$TARGET/info  mandir=$DOC_DIR/share/doc/$TARGET/man  datadir=$DOC_DIR/share |tee chh_install.log
popd
}

#task  [024]   /build/expat

build_hostlib_expat()
{
rm -rf $BASE_DIR/expat_build
mkdir -p $BASE_DIR/expat_build
pushd $BASE_DIR/expat_build
../$EXPAT_DIR/configure --build=$HOST --host=$HOST --target=$HOST  --prefix=$HOST_LIB --disable-shared  --enable-nls
make -j $PARALLEL
make install
popd
}

#task  [025]   /build/gdb
build_hostgdb()
{
rm -rf gdb_build
mkdir gdb_build
pushd gdb_build
CFLAGS="-static -g -O2" ../$GDB_DIR/configure --build=$HOST --target=$TARGET --prefix=$SYSROOT --host=$HOST --disable-sim --disable-libmcheck --enable-nls --with-libexpat-prefix=$HOST_LIB  '--with-gdb-datadir=$TARDIR/$TARGET/share/gdb' 
make  -j $PARALLEL
make install prefix=$TARDIR exec_prefix=$TARDIR libdir=$TARDIR/lib htmldir=$DOC_DIR/share/doc/$TARGET/html pdfdir=$DOC_DIR/share/doc/$TARGET/pdf infodir=$DOC_DIR/share/doc/$TARGET/info  mandir=$DOC_DIR/share/doc/$TARGET/man  datadir=$DOC_DIR/share |tee chh_install.log
popd
}

ALL='initdir cleanup source_prepare hostlib_texinfo hostlib_gmp hostlib_mpfr hostlib_mpc hostlib_ppl hostlib_cloog hostlib_libelf binutils kernel_header gcc_first uclibc_0 uclibc_1 uclibc_2 uclibc_3 uclibc_4 uclibc_5 uclibc_6 uclibc_7 uclibc_8 uclibc_9 uclibc_10 uclibc_11 uclibc_12 uclibc_13 uclibc_14 uclibc_15 uclibc_16 uclibc_17 uclibc_18 uclibc_19 uclibc_20 gcc_final hostlib_expat hostgdb ' 

#ALL='cleanup' 
[ $# -eq 0 ] && { $0 $ALL; exit; }
echo "$# $0 $1 $2"
while [ $# -gt 0 ]
do
	echo *****************************build_$1*************************************
	sleep 1
	eval build_$1
	shift
done

