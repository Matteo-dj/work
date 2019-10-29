import os, time, string, re ,shutil, hashlib, binascii, sys
g_params = {}
g_file_id_dic = {}
g_mod_id_dic = {}
def input_to_dic():
    key='default_key'
    key_val=''
    i=0
    while i< len(sys.argv):
        if sys.argv[i]=='_PYTHON_ARG_':
            if key!='default_key':
                g_params[key]=key_val
            else:
                pass 
			
            key = sys.argv[i+1]
            key_val = ''
            i=i+1
        else:
            key_val=key_val+sys.argv[i]+' '
        i=i+1
		
	#last key process	
    if key!='default_key':
        g_params[key]=key_val

#convert file_id_file(lm_file_id.cfg) to dictionary    
def create_file_id_dic():
    file_name = g_params['CURRENT_DIR'].strip()+'/'+'lm_file_id.cfg'
    if os.path.isfile(file_name):
        f1=open(file_name,'r')
        for line in f1:
            list_temp=re.split('[ \t\n\r\f\v]+',line)
            if len(list_temp) >= 2 and list_temp[0][-2:]=='.c' and list_temp[1].isdigit():
                g_file_id_dic[list_temp[0]]=list_temp[1]
    

#convert mod_id_file(lm_file_id.cfg) to dictionary      
def create_mod_id_dic():
    file_name = g_params['CURRENT_DIR'].strip()+'/'+'lm_mod_id.cfg'
    if os.path.isfile(file_name):
        f1=open(file_name,'r')
        for line in f1:
            list_temp=re.split('[ \t\n\r\f\v]+',line)
            if len(list_temp) >= 2 and list_temp[0][-2:]=='.c' and list_temp[1].isdigit():
                g_mod_id_dic[list_temp[0]]=list_temp[1]
    
def do_cc():
    
    src_file_path_name = g_params['SRC_FILE_NAME']
    src_file_name = src_file_path_name.split('/')[-1].strip()
    if src_file_name in g_file_id_dic and src_file_name in g_mod_id_dic:
        file_id  = g_file_id_dic[src_file_name]
        mod_id = g_mod_id_dic[src_file_name]
    else:
        file_id = 0
        mod_id = 0
   
    cc_line = g_params['CC']
    cc_line = cc_line+' '+g_params['CFLAGS']
    cc_line = cc_line+' '+'-D__NEW_FILE_ID__='+str(file_id)
    cc_line = cc_line+' '+'-D__NEW_MOD_ID__='+str(mod_id)
    cc_line = cc_line+' '+' -c'
    cc_line = cc_line+' '+g_params['SRC_FILE_NAME']
    cc_line = cc_line+' '+' -o'
    cc_line = cc_line+' '+g_params['DST_FILE_NAME']
    #print('cc_line',cc_line)
    ret=os.system(cc_line)
    if ret!=0:
        sys.exit(ret)
def cc_main():
    input_to_dic()
    for key in g_params:
        #make file中 \"传入到python后变成",恢复回去
        temp=g_params[key].replace('\"','\\\"')
        g_params[key]=temp

    create_file_id_dic()
    create_mod_id_dic()
    do_cc()
cc_main()    