#!perl
# change keywords in .bcfg file.
# Parameters:   <.pcf> <rsf path> 
#
# History
# Author                Date          Version 
# shenhankun/00130424   2013-08-10    V1.0  
###########################################
use strict;

my $cfg_file=$ARGV[0];
my $out_path=$ARGV[1];
my $out_file;
my @fdata;
my @file_output_data;
my @file_output2;
my @file_input;
my $file_output;
my $file_hrd="#include \"cts_cfg_interface.h\"\n\n";


if(!open(FH, $cfg_file)){
 warn("***error: fail to open file: $cfg_file\n");  
 close(FH);
}
@fdata = <FH>;
close(FH);

my $count = 0;
push(@file_output_data, $file_hrd);

my @words = split(/\//, $cfg_file);
my $pcfg_name = pop(@words);
my $pcfg_out_file;

while($count < @fdata) 
{
  my $sz = @fdata[$count];
  
  $count++;
  
  if($sz=~ /^\/\/@/)
  {
	my @words = split(/ /, $sz);
	my $t = shift(@words);
	$t =~ s/@//g;
	$t =~ s/^\/\///g;
	$t =~ s/^@//g;
	$t =~ s/\n//g;
	$t =~ s/\r//g;
	$pcfg_out_file = sprintf("%s/%s.rt.%s.rtcfg", $out_path,$pcfg_name, $t);
	next;
  }
  if($sz=~ /^\/\//i)
  {
	my $file = $sz;
	$file =~ s/HUPG = //g;
	$file =~ s/\s//g;
	next;
  }
  if($sz=~ /};/i)
  {
	if(@file_output_data == 1)
	{
	    warn("***error: fail to find valid formate in config file: $cfg_file\n");
	    exit(1);
	}
	push(@file_output_data, $sz);
	if(!open(FH, ">$pcfg_out_file"))
	{
	 warn("***error: fail to open file: $pcfg_out_file\n");  
	 close(FH);
	 exit(1);
	}
	print FH @file_output_data;
	close(FH);
	print "rt script file: ", $pcfg_out_file, "\n";
	$pcfg_out_file = "";
	@file_output_data=();
	push(@file_output_data, $file_hrd);
	next;
  }
  if(length $pcfg_out_file)
  {
	  if(length $sz)
	  {
	     push(@file_output_data, $sz);
	  }
  }
}
