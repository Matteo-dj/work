import sys
import os
import CopyFileByList
import shutil

#read source file list


#source_folder = r"D:\v1r3\mdm\inc"
#file_for_source_file_list = open(r"D:\v1r3\mdm\inc\hso_sync.hcfg")
#destination_folder = r"D:\v1r3\hso\deploy\ndm_inc"

#current path: D:\v1r3\target\onebuilder\toolsNew

vr_path = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir, os.pardir, os.pardir))
print (vr_path)

source_folder_name = vr_path + r"\mdm\inc"
#print (source_folder_name)
source_file_list_file_name = vr_path + r"\mdm\inc\hso_sync.hcfg"
destination_folder_name = vr_path + r"\hso\deploy\ndm_inc"

#shutil.copy("D:\\v1r3\\mdm\\inc\\sal_inf.h", "D:\\v1r3\\hso\\deploy\\ndm_inc\\sal_inf.h")
#shutil.copy("D:\\v1r3\\hso\\deploy\\ndm_inc\\sal_inf.h", "D:\\v1r3\\mdm\\inc")

CopyFileByList.CopyFileByList(source_folder_name, source_file_list_file_name, destination_folder_name)

print ("\n")

#source_folder = r"D:\v1r3\target\include"
#file_for_source_file_list = open(r"D:\v1r3\target\include\hso_sync.hcfg")
#destination_folder = r"D:\v1r3\hso\deploy\ndm_inc"

#current path: D:\v1r3\target\onebuilder\toolsNew
source_folder_name = vr_path + (r"\target\include")
source_file_list_file_name = vr_path + (r"\target\include\hso_sync.hcfg")
destination_folder_name = vr_path + (r"\hso\deploy\ndm_inc")


#shutil.copyfile(r"D:\v1r3\target\include\hi_config.h", r"D:\v1r3\hso\deploy\ndm_inc\hi_config.h")

CopyFileByList.CopyFileByList(source_folder_name, source_file_list_file_name, destination_folder_name)

