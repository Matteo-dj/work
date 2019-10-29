import shutil 
import os

#source_folder_name:source folder
#destination_folder_name:destination folder
#source_file_list_file_name:file containing source file list

def CopyFileByList(source_folder_name, source_file_list_file_name, destination_folder_name):
    
    print ("source_folder: " + source_folder_name)
    print ("source_file_list_file: " + source_file_list_file_name)
    print ("destination_folder: " + destination_folder_name)
    
    if(not os.path.exists(source_folder_name)):
        return

    if not os.path.isfile(source_file_list_file_name):
        return
    
    if(not os.path.exists(destination_folder_name)):
        os.makedirs(destination_folder_name)

    if(source_folder_name[len(source_folder_name) - 1] != "\\" and source_folder_name[len(source_folder_name) - 1] != "/"):
        source_folder_name += "\\"

    if(destination_folder_name[len(destination_folder_name) - 1] != "\\" and source_folder_name[len(source_folder_name) - 1] != "/"):
        destination_folder_name += "\\"

    #get source file list
    source_file_list_file = open(source_file_list_file_name)
    try:
         source_file_list_raw = source_file_list_file.readlines( )
         #print source_file_list_raw
    finally:
         source_file_list_file.close( )
     
    #delete Comments
    source_file_list = []
    for strLine in source_file_list_raw:
        strLine = strLine.strip()
        
        if(strLine == ""):
            continue
        if(strLine[0] == "#"):
            continue
        
        source_file_list.append(strLine)
        
    if(len(source_file_list) <= 0):
        return
    
    #print source_file_list

    print ("copy")
    for strLine in source_file_list:
        source_folder_full_path = source_folder_name + strLine
        destination_folder_full_path = destination_folder_name + strLine
        #print source_folder_full_path
        #print destination_folder_full_path
        if os.path.isfile(source_folder_full_path):
            print (source_folder_full_path + " to " + destination_folder_full_path)
            #shutil.copyfile(source_folder_full_path, destination_folder_full_path)
            cmd = 'copy' + ' ' + source_folder_full_path + ' ' + destination_folder_full_path + ' /y'
            #print cmd
            result = os.popen(cmd)
            #print result






