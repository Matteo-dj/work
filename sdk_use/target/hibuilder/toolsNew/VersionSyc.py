import os
import re

#current path: E:\br_HiMDLV200R001C00B013_LiteosHibuild\target\hibuilder\toolsNew
#source version file path: E:\br_HiMDLV200R001C00B013_LiteosHibuild\target\hibuilder\ver_cfg\hb_ver_cfg.h
#main resource file path: E:\br_HiMDLV200R001C00B013_LiteosHibuild\hso\sop\main\main\HiStudio.rc
#hso/sop/main/hsosc/hsosc.rc


def FindVal(strValName, file_lines, pattern):
    strVal = ""
    for strLine in file_lines:
        #print (strLine)
        re_result = pattern.search(strLine)    
        if re_result != None:
            #print ("======================================")
            #print (re_result)
            idx = strLine.find(strValName)
            if idx >= 0:
                strVal = strLine[idx + len(strValName):]
                strVal = strVal.strip()
                strVal = strVal.strip("\"")
                #print ("Value: " + strVal)
    return strVal                

def ReplaceVer(strValName, strVal, file_lines):
    pattern = re.compile(r"\s*VALUE\s*\"" + strValName +"\"\s*,\s*\"(.+)\"")
    iLine = 0
    for strLine in file_lines:
        re_result = pattern.search(strLine)
        if re_result != None:
            #print ("-----------------------------")
            #print (re_result)
            idx = strLine.find(",")
            idx = strLine.find("\"", idx + 1)
            strLineNew = strLine[0: idx + 1] + strVal + "\"\n"
            #print (strLineNew)
            file_lines[iLine] = strLineNew
        iLine = iLine + 1

def ReplaceVerHead(strValName, strVal, file_lines):
    pattern = re.compile(r"^[ \t]*" + strValName + "[ \t]*[\d,]+[ \t]*$")
    iLine = 0
    pattern_in_line = re.compile(r"[\d,]+")
    for strLine in file_lines:
        re_result = pattern.search(strLine)
        if re_result != None:
            #print (strLine)
            strLineNew = pattern_in_line.sub(strVal, strLine)
            #print (strVal)
            print (strLineNew)
            file_lines[iLine] = strLineNew
        iLine = iLine + 1

def SyncVerFile(source_version_file_path, dest_version_file_path, remove_comma):
    
    dest_version_file_path_new = dest_version_file_path# + "New"

    source_version_file_lines = []
    source_version_file = open(source_version_file_path)
    try:
        source_version_file_lines = source_version_file.readlines()
    finally:
        source_version_file.close()

    #line to find in source version file path: #define PRODUCT_CFG_FULL_VERSION_STR "Hi3912 V100R003C00SPC001B035"

    pattern_line_source_version_file = re.compile(r"\s*#define\s+\w+\s+\"?.*\"?");
    #pattern_full_version_source = re.compile(r"\s+")

    strFullVer = FindVal("PRODUCT_CFG_FULL_VERSION_STR", source_version_file_lines, pattern_line_source_version_file)
    strFileVer = FindVal("PRODUCT_CFG_FILE_VERSION_STR", source_version_file_lines, pattern_line_source_version_file)


    #line to find in main resource file: VALUE "PrivateBuild", "Hi3911 V200R001C00B014"

    main_resource_file_lines = []
    main_resource_file = open(dest_version_file_path)
    try:
        main_resource_file_lines = main_resource_file.readlines()
    finally:
        main_resource_file.close()

    strFileVer = strFileVer.replace('.', ',')
    strProductVer = strFileVer
    if remove_comma:
        strFileVer = strFileVer.replace(',', '')
        strProductVer = strProductVer.replace(',', '')

    ReplaceVerHead("FILEVERSION", strFileVer, main_resource_file_lines)
    ReplaceVerHead("PRODUCTVERSION", strProductVer, main_resource_file_lines)

    ReplaceVer("PrivateBuild", strFullVer, main_resource_file_lines)
    ReplaceVer("FileVersion", strFileVer, main_resource_file_lines)
    ReplaceVer("ProductVersion", strProductVer, main_resource_file_lines)

    #print (main_resource_file_lines)
    main_resource_file_new = open(dest_version_file_path_new, "w")
    try:
        main_resource_file_new.writelines(main_resource_file_lines)
    finally:
        main_resource_file_new.close()

def GetValFromFile(pattern, file_path):
    file = open(file_path)
    try:
        file_lines = file.readlines()
    finally:
        file.close()

    #pattern = re.compile(r"[ \t]*" + strValName + r"[ \t]*" + strSeperator + r"[ \t]*(\S*)\s*")
    for strLine in file_lines:
        re_result = pattern.match(strLine)
        if re_result != None:
            bFound = True
            #print (strLine)
            #print (re_result)
            #print (re_result.group(1))
            strVal = re_result.group(1)
            break
    return (bFound, strVal)
            
vr_path = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir, os.pardir, os.pardir))
print ("top path: " + vr_path)

source_version_file_path = vr_path + r"\target\hibuilder\ver_cfg\hb_ver_cfg.h"
print ("source version file path: " + source_version_file_path)

main_resource_file_path = vr_path + r"\hso\sop\main\main\HiStudio.rc"
print ("main resource file path: " + main_resource_file_path)

#hso/sop/main/hsosc/hsosc.rc
hsosc_resource_file_path = vr_path + r"\hso\sop\main\hsosc\hsosc.rc"
print ("main resource file path: " + main_resource_file_path)

#v2r1\hb.cfg
hb_cfg_file_path = vr_path + r"\hb.cfg"

SyncVerFile(source_version_file_path, main_resource_file_path, False)
SyncVerFile(source_version_file_path, hsosc_resource_file_path, True)


#D:\br_HiMDLV200R001C00B013_LiteosHibuild\hso
#hso_cfg.mk
hso_cfg_file_path = vr_path + r"\hso\hso_cfg.mk"

pattern = re.compile(r"[ \t]*#define[ \t]*" + "PRODUCT_CFG_FILE_VERSION_STR" + r"[ \t]*(\S*)\s*")

bFound, strProductVer = GetValFromFile(pattern, source_version_file_path)

if bFound:
    strProductVer = strProductVer.strip("\"")
    hso_cfg_file_lines = []
    hso_cfg_file_lines.append("PRODUCT_VERSION=" + strProductVer)

    hso_cfg_file = open(hso_cfg_file_path, "w")
    try:
        hso_cfg_file.writelines(hso_cfg_file_lines)
    finally:
        hso_cfg_file.close()
else:
    print ("File version not found in: " + source_version_file_path)



