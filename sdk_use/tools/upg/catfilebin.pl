#!perl
# Combine bin files  
# the max. input file size is limitted to 5MB.
#
# History
# Author                Date          Version 
# shenhankun/00130424   2013-08-10    V1.0  
#

use strict;
use lib qw( blib/lib lib );
use FileHandle;
use Archive::Zip;

my $f1buffer;
my $f2buffer;
my $fil1size;
my $fil2size;
my $crc = 0;
my $i = 0;
my $file1=$ARGV[0];
my $file2=$ARGV[1];
my $file_out=$ARGV[2];
my $magic=$ARGV[3];

if ( -d $file1 ) {
	warn "${file1}: Is a directory\n";
	exit 1;
}

if ( -d $file2 ) {
	warn "${file2}: Is a directory\n";
	exit 1;
}

my $fh1 = FileHandle->new();
if ( !$fh1->open( $file1, 'r' ) ) {
	warn("***error: fail to open file:$file1\n");
	exit 2;
}
binmode($fh1);
my $fh2 = FileHandle->new();
if ( !$fh2->open( $file2, 'r' ) ) {
	warn("***error: fail to open file:$file2\n");
	exit 2;
}
binmode($fh2);

my $fh_out = FileHandle->new();
$fh_out->open( $file_out, 'w' );
binmode($fh_out);

$fil1size = $fh1->read($f1buffer, 5*1024*1024);
$fil2size = $fh2->read($f2buffer, 5*1024*1024);
my $crc;

if($fil1size < 392){
	warn("***error: file is too small,file1:$file1\n");
	exit 2;
}
if($fil2size < 392){
	warn("***error: file is too small,file2:$file2\n");
	exit 2;
}
if($fil1size > 5*1024*1024){
	warn("***error: file is too large(>5MB),file1:$file1\n");
	exit 2;
}
if($fil2size > 5*1024*1024){
	warn("***error: file is too large(>5MB),file2:$file2\n");
	exit 2;
}

#将第一个UPG头写到新UPG中
$fh_out->write($f1buffer, 396);

#偏移0字节处写magic
$fh_out->seek(0, 0);
my $outVar = pack("a4", $magic);
$fh_out->write($outVar, 4);

#偏移4字节处写CRC,下面会修改CRC值
$fh_out->seek(4, 0);
my $crchex = sprintf("%08X\n",$crc);
my $outVar = pack("H*", $crchex);
$fh_out->write($outVar, 4);

#ucNumberOfCodeSections 02
$fh_out->seek(29, 0);
$outVar = pack("H*", "02");
$fh_out->write($outVar, 1);

#ucNumberOfCfgSections 02
$fh_out->seek(30, 0);
$outVar = pack("H*", "02");
$fh_out->write($outVar, 1);

#ucFmtVer  161
$fh_out->seek(31, 0);
$outVar = pack("H*", "A1");
$fh_out->write($outVar, 1);

my $f2buffertmp;
my $fil2sizetmp;
my $temp;

#f2的stSecCode.astCfg[0]写到 fh_out的stSecCode.astCfg[1]
$fh2->seek(196,0);
$fil2sizetmp = $fh2->read($f2buffertmp, 5*1024);
$fh_out->seek(216, 0);
$fh_out->write($f2buffertmp, 20);

#f2的stSecCfg.astCfg[0]写到 fh_out的stSecCfg.astCfg[1]
$fh2->seek(32,0);
$fil2sizetmp = $fh2->read($f2buffertmp, 5*1024);
$fh_out->seek(52, 0);
$fh_out->write($f2buffertmp, 20);


#fh_out的stSecCode.astCfg[0].ulOffset写0
$fh_out->seek(196+4,0);
$temp = sprintf("%08x", 0);
for($i = 0; $i < (length $temp); $i = $i + 2){
    my $dat = 0;
    $dat = substr($temp,((length $temp) - $i - 2),2);
	my $outVar = pack("H*", $dat);
	$fh_out->write($outVar, 2);
}

#fh_out的stSecCode.astCfg[0].ulSectionSize写fil1size
$temp = sprintf("%08x", $fil1size);
for($i = 0; $i < (length $temp); $i = $i + 2){
    my $dat = 0;
    $dat = substr($temp,((length $temp) - $i - 2),2);
	my $outVar = pack("H*", $dat);
	$fh_out->write($outVar, 2);
}

#fh_out的stSecCode.astCfg[1].ulOffset写fil1size
$fh_out->seek(196+20+4,0);
$temp = sprintf("%08x", $fil1size);
for($i = 0; $i < (length $temp); $i = $i + 2){
    my $dat = 0;
    $dat = substr($temp,((length $temp) - $i - 2),2);
	my $outVar = pack("H*", $dat);
	$fh_out->write($outVar, 2);
}

#fh_out的stSecCode.astCfg[1].ulSectionSize写fil2size
$temp = sprintf("%08x", $fil2size);
for($i = 0; $i < (length $temp); $i = $i + 2){
    my $dat = 0;
    $dat = substr($temp,((length $temp) - $i - 2),2);
	my $outVar = pack("H*", $dat);
	$fh_out->write($outVar, 2);
}

# Clear aulSHA256
$fh_out->seek(360,0);
$outVar = pack("H*", "000000000000000000000000000000000000000000000000000000000000000000000000");
$fh_out->write($outVar, 36);

# write fil1size 到aulSHA256[0]
$fh_out->seek(360,0);
$temp = sprintf("%08x", $fil1size);
for($i = 0; $i < (length $temp); $i = $i + 2){
    my $dat = 0;
    $dat = substr($temp,((length $temp) - $i - 2),2); # substr($buffer,$i,2)
	my $outVar = pack("H*", $dat);
	$fh_out->write($outVar, 2);
}

# write fil2size 到aulSHA256[1]
$temp = sprintf("%08x", $fil2size);
for($i = 0; $i < (length $temp); $i = $i + 2){
    my $dat = 0;
    $dat = substr($temp,((length $temp) - $i - 2),2);
	my $outVar = pack("H*", $dat);
	$fh_out->write($outVar, 2);
}
# write fil1size+fil2size 到 ulDataSize
$fh_out->seek(392,0);
$temp = sprintf("%08X", $fil1size+$fil2size);
for($i = 0; $i < (length $temp); $i = $i + 2){
    my $dat = 0;
    $dat = substr($temp,((length $temp) - $i - 2),2);
	my $outVar = pack("H*", $dat);
	$fh_out->write($outVar, 2);
}

#写file1和file2内容到head尾部
$fh_out->seek(396,0);
$fh_out->write($f1buffer,$fil1size);
$fh_out->write($f2buffer,$fil2size);

#偏移4字节处写CRC
my $fobuffer;
my $fobuffercrc;
$fh_out->open( $file_out, 'r' );
binmode($fh_out);
my $filesize = $fh_out->read($fobuffer, 8*1024*1024);
$fh_out->seek(8,0);
my $filesizecrc = $fh_out->read($fobuffercrc, 8*1024*1024);
$crc = Archive::Zip::computeCRC32( $fobuffercrc, $crc );
$fh_out->seek(0,0);
my $tmpbuffercrc;
my $tmpsizecrc = $fh_out->read($tmpbuffercrc, 4);
$crc = Archive::Zip::computeCRC32( $tmpbuffercrc, $crc );

$crchex = sprintf("%08X",$crc);
$fh_out->open( $file_out, 'w' );
binmode($fh_out);
$fh_out->write($fobuffer, $filesize);
$fh_out->seek(4,0);
$temp = $crchex;
for($i = 0; $i < (length $temp); $i = $i + 2){
    my $dat = 0;
    $dat = substr($temp,((length $temp) - $i - 2),2);
	my $outVar = pack("H*", $dat);
	$fh_out->write($outVar, 2);
}

#printf("%s: %d(%d); CRC:%s\n", $file_out, $filesize,$fil1size+$fil2size, $crchex);
#printf("%s: %d\n", $file1, $fil1size);
#printf("%s: %d\n", $file2, $fil2size);

