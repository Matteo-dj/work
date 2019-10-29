#! python

import os, time, string, re ,shutil, time, hashlib, binascii, sys
g_src_dir = sys.argv[1]
g_dst_dir = sys.argv[2]
g_base_datatype_def_txt=sys.argv[3]
g_first_line = r'#include "base_datatype_def.txt"'
#print(g_src_dir)
#print(g_dst_dir)
#print('hhhhhhhh',sys.argv[3])


#txt过滤
def c_2_txt_file(src_file,dst_file):
    f1 = open(src_file, 'r')
    f2 = open(dst_file, 'w')
    f2.write(g_first_line)	
    for line in f1:
        if(re.match(r"[ ]*#",line)):
            pass
        elif(re.match(r"[ ]*HI_EXTERN",line)): 	
            pass
        elif(re.match(r"[ ]*HI_PRV",line)): 	
            pass	
        elif(re.match(r"[ ]*HI_EAPI",line)): 	
            pass	
        elif(re.match(r"[ ]*extern",line)): 	
            pass				
        else:
            f2.write(line)	
    f1.close()
    f2.close()

#遍历文件
def c_2_txt_dir(src_dir,dst_dir):
    src_dir_list = os.listdir(src_dir)	
    for x in src_dir_list:
        src_file = src_dir+'/'+x
        dst_file = dst_dir+'/'+x
        #print('haiwen src_dst',src_file,dst_file)
        c_2_txt_file(src_file,dst_file)
#开始	
c_2_txt_dir(g_src_dir,g_dst_dir)
shutil.copy(g_base_datatype_def_txt,g_dst_dir) 
