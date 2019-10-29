#! python
# -*- coding: utf-8 -*- 
import os, time, string, re ,shutil, time, xml.etree.ElementTree as ET
from configparser import ConfigParser
#=================================================#
cfg = ConfigParser()
cfg.read('config.ini')
root_path        = os.getcwd() + "/../../../"
pkt_path         = root_path + cfg.get('path','pkt_path')


#目标文件地址#
upg_pub_path    = pkt_path + "/image/" 


#=================================================#


def filerename():
    files = os.listdir(upg_pub_path)
    for file in files:
        if os.path.splitext(file)[1] == '.hbin':
            filename = os.path.splitext(file)[0]
            src = os.path.join(upg_pub_path, file)
            #print(file)
            new_file = os.path.join(upg_pub_path, filename+'.bin')
            #print(new_file)
            os.rename(src, new_file)
        elif os.path.splitext(file)[1] == '.hupg':
            filename = os.path.splitext(file)[0]
            src = os.path.join(upg_pub_path, file)
            #print(file)
            new_file = os.path.join(upg_pub_path, filename+'.upg')
            #print(new_file)
            os.rename(src, new_file) 

# main function #
if __name__ == '__main__':
    
    filerename()
    #fc0 = NvSdkBuild()
    #fc0.work()

    #print('TOTAL TIME:%ss'%str(time.time() - t1))
