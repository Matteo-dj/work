#!/bin/bash

TEMP=$1
COMPILE_NAME=${TEMP%x*}
VERSION_NUM=$2
LIB=$3
CUR_DIR=$PWD
FIND_FLAG=0

#FILE_NUM=`find $CUR_DIR/lib/cxxstl -name *.h | wc -l`
FILE_NUM=`find $CUR_DIR/lib/cxxstl/c++/$VERSION_NUM -name *.h | wc -l`

if [[ $FILE_NUM -gt 0 ]];then
	echo "do not copy the header again!!!!"
	exit
fi

######### usb special characters to keep the blank ########
PATH_NO_BLANK="$(echo $PATH | sed 's/ /_-_/g')"

######### find the path of compiler ############
OLD_IFS="$IFS"
IFS=":"
arr=($PATH_NO_BLANK)
IFS="$OLD_IFS"

for s in ${arr[@]}
do
PATH_TEMP="$(echo $s | sed s'/_-_/ /g')"
if [[ "$PATH_TEMP" =~ .*$COMPILE_NAME.* ]];then
	OBJ_DIR_TEMP=$s
	break
fi
done

########## find the name of compiler ###########
OLD_IFS="$IFS"
IFS="/"
arr=($OBJ_DIR_TEMP)
IFS="$OLD_IFS"

OBJ_DIR="/"
for s in ${arr[@]}
do
FILE_NAME="$(echo $s | sed s'/_-_/ /g')"
OBJ_DIR="$OBJ_DIR""$FILE_NAME"
if [[ "$FILE_NAME" =~ .*$COMPILE_NAME.* ]];then
        COMPILE_NAME_TEMP=$FILE_NAME
	FIND_FLAG=1
        break
fi
OBJ_DIR="$OBJ_DIR""/"
done

if [ "$FIND_FLAG" == 0 ];then
	echo "find the compiler error!!!!"
	echo "+++++you do not have the compiler!!!+++++"
	exit
fi

FIRST_C=`echo ${COMPILE_NAME_TEMP:0:1}`
if [ "$FIRST_C" == "." ];then
	COMPILE_NAME_TEMP=`echo ${COMPILE_NAME_TEMP:1}`
fi
ls "$OBJ_DIR" > $CUR_DIR/dir_list.txt
while read line
do
        if [[ $line =~ .*$COMPILE_NAME_TEMP.* ]];then
		COMPILE_NAME_TEMP=$line
		break
	fi
done < $CUR_DIR/dir_list.txt
rm -rf $CUR_DIR/dir_list.txt

############## copy the lib ####################
GCC_LIB_DIR="$OBJ_DIR"/lib/gcc/$COMPILE_NAME_TEMP/$VERSION_NUM/a7_softfp_neon-vfpv4
mkdir -p $CUR_DIR/lib/a7_softfp_neon-vfpv4
cp "$GCC_LIB_DIR"/libgcc.a "$GCC_LIB_DIR"/libgcc_eh.a "$GCC_LIB_DIR"/libgcov.a $CUR_DIR/lib/a7_softfp_neon-vfpv4

GCC_LIB_DIR="$OBJ_DIR"/lib/gcc/$COMPILE_NAME_TEMP/$VERSION_NUM/a17_softfp_neon-vfpv4
mkdir -p $CUR_DIR/lib/a17_softfp_neon-vfpv4
cp "$GCC_LIB_DIR"/libgcc.a "$GCC_LIB_DIR"/libgcc_eh.a "$GCC_LIB_DIR"/libgcov.a $CUR_DIR/lib/a17_softfp_neon-vfpv4

GCC_LIB_DIR="$OBJ_DIR"/lib/gcc/$COMPILE_NAME_TEMP/$VERSION_NUM/armv5te_arm9_soft
mkdir -p $CUR_DIR/lib/armv5te_arm9_soft
cp "$GCC_LIB_DIR"/libgcc.a "$GCC_LIB_DIR"/libgcc_eh.a "$GCC_LIB_DIR"/libgcov.a $CUR_DIR/lib/armv5te_arm9_soft

if [ "$LIB" == "CXX" ]
then
CXX_LIB_DIR="$OBJ_DIR"/$COMPILE_NAME_TEMP/lib/a7_softfp_neon-vfpv4
cp "$CXX_LIB_DIR"/libstdc++.a "$CXX_LIB_DIR"/libsupc++.a $CUR_DIR/lib/a7_softfp_neon-vfpv4

CXX_LIB_DIR="$OBJ_DIR"/$COMPILE_NAME_TEMP/lib/a17_softfp_neon-vfpv4
cp "$CXX_LIB_DIR"/libstdc++.a "$CXX_LIB_DIR"/libsupc++.a $CUR_DIR/lib/a17_softfp_neon-vfpv4

CXX_LIB_DIR="$OBJ_DIR"/$COMPILE_NAME_TEMP/lib/armv5te_arm9_soft
cp "$CXX_LIB_DIR"/libstdc++.a "$CXX_LIB_DIR"/libsupc++.a $CUR_DIR/lib/armv5te_arm9_soft

CXX_HEAD_DIR=$CUR_DIR/lib/cxxstl
rm -rf $CXX_HEAD_DIR/c++ $CXX_HEAD_DIR/gdbinclude $CXX_HEAD_DIR/gccinclude
mkdir -p $CXX_HEAD_DIR/gdbinclude
mkdir -p $CXX_HEAD_DIR/gccinclude
mkdir -p $CXX_HEAD_DIR/gccinclude/bits
mkdir -p $CXX_HEAD_DIR/gccinclude/sys

############# copy c++ headers ####################
cp -rf "$OBJ_DIR"/$COMPILE_NAME_TEMP/include/c++ $CXX_HEAD_DIR
cp "$OBJ_DIR"/lib/gcc/$COMPILE_NAME_TEMP/$VERSION_NUM/include/*.h $CXX_HEAD_DIR/gdbinclude
cp "$OBJ_DIR"/target/usr/include/*.h $CXX_HEAD_DIR/gccinclude
cp "$OBJ_DIR"/target/usr/include/bits/*.h $CXX_HEAD_DIR/gccinclude/bits
cp "$OBJ_DIR"/target/usr/include/sys/*.h $CXX_HEAD_DIR/gccinclude/sys

############# delete the headers which conflict LiteOS header ##########
while read line
do
	rm $CUR_DIR/lib/cxxstl/$line
done < $CUR_DIR/lib/cxxstl/del_file.txt

############### patch the modify of c++ headers #############
pushd $CUR_DIR/lib
if [ "$COMPILE_NAME_TEMP" == "arm-hisiv300-linux-uclibcgnueabi" ];then
patch -p1 < $CUR_DIR/lib/cxxstl/cxx_hisiv300.patch
patch -p1 < $CUR_DIR/lib/cxxstl/cxx_hisiv300_ios_memory.patch
elif [ "$COMPILE_NAME_TEMP" == "arm-hisiv500-linux-uclibcgnueabi" ];then
patch -p1 < $CUR_DIR/lib/cxxstl/cxx_hisiv500.patch
patch -p1 < $CUR_DIR/lib/cxxstl/cxx_hisiv500_ios_memory.patch
elif [ "$COMPILE_NAME_TEMP" == "arm-liteos-linux-uclibcgnueabi" ];then
patch -p1 < $CUR_DIR/lib/cxxstl/cxx_liteos.patch
patch -p1 < $CUR_DIR/lib/cxxstl/cxx_liteos_ios_memory.patch
elif [ "$COMPILE_NAME_TEMP" == "arm-hisiv600-linux-gnueabi" ];then
mkdir -p $CXX_HEAD_DIR/gccinclude/gnu
cp "$OBJ_DIR"/target/usr/include/gnu/*.h $CXX_HEAD_DIR/gccinclude/gnu
patch -p1 < $CUR_DIR/lib/cxxstl/cxx_hisiv600.patch
patch -p1 < $CUR_DIR/lib/cxxstl/cxx_hisiv600_ios_memory.patch
else
	echo "patch error!!!"
fi
popd


fi #if [ $LIB == "CXX"]
