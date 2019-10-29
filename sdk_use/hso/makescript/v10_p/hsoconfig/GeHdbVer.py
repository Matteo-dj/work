#从hb_ver_cfg.h中获取版本号，写入到datavase_ver.txt中
#w00291248 2017/07/29
import os
import re
import sys
import time

def GetValFromFile(pattern, file_path):
    file = open(file_path)
    try:
        file_lines = file.readlines()
    finally:
        file.close()

    for strLine in file_lines:
        re_result = pattern.match(strLine)
        if re_result != None:
            bFound = True
            strVal = re_result.group(1)
            break
    return (bFound, strVal)
            
vr_path = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir, os.pardir, os.pardir, os.pardir))
#print ("top path: " + vr_path)

source_version_file_path = vr_path + r"\target\hibuilder\ver_cfg\hb_ver_cfg.h"
#print ("source version file path: " + source_version_file_path)

target_release_path = sys.argv[1]
hso_cfg_file_path = target_release_path + r"\database\database_ver.txt"
#print ("database_ver.txt path: " + hso_cfg_file_path)

patternChip = re.compile(r"[ \t]*#define[ \t]*" + "PRODUCT_CFG_CHIP_SOLUTION_NAME" + r"[ \t]*(\S*)\s*")
patternSof = re.compile(r"[ \t]*#define[ \t]*" + "PRODUCT_CFG_VERSION_STR" + r"[ \t]*(\S*)\s*")

bFoundChipVer, strChipVer = GetValFromFile(patternChip, source_version_file_path)
bFoundSofVer, strSofVer = GetValFromFile(patternSof, source_version_file_path)

if bFoundChipVer and bFoundSofVer:
    strChipVer = strChipVer.strip("\"")
    strSofVer = strSofVer.strip("\"")
    strProductVer = strChipVer + " " + strSofVer
#增加时间戳 w00291248 2017/08/02
    strProductVer = strProductVer + "  " + time.ctime()
    hso_cfg_file_lines = []
    hso_cfg_file_lines.append(strProductVer)

    hso_cfg_file = open(hso_cfg_file_path, "w")
    try:
        hso_cfg_file.writelines(hso_cfg_file_lines)
    finally:
        hso_cfg_file.close()
    print ("generate hdb ver success")    
else:
    print ("File version not found in: " + source_version_file_path)



