#! python
# -*- coding: utf-8 -*- 
import sys,os, time, string, re ,shutil, time, xml.etree.ElementTree as ET

def modify_upg_head(file_path):
    #f1=open(file_path,"rb")
    #f2=open(file_path+"back","wb")
    #f2.write(f1.read())
    #f1.close()
    #f2.close()
    
    f=open(file_path,"r+b")
    f.seek(0)
    f.write(b'\x48\x49\x53\x49')
    f.close()
    

modify_upg_head(sys.argv[1])