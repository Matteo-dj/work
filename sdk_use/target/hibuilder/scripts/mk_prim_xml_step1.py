import os, time, string, re ,shutil, hashlib, binascii, sys
#os.environ["PATH"]=os.environ["PATH"]+r'F:\code\install_compile_ball_liteos\arm-hisiv300-linux\bin;'
g_params = {}
g_file_id_dic = {}
g_mod_id_dic = {}

#set print target file
def set_print_target(log_file_name):
    log_file_dir = os.path.dirname(log_file_name)
    if not os.path.isdir(log_file_dir):    
        os.makedirs(log_file_dir)
    log_file_fp = open(log_file_name,'w+')
    last_print_target = sys.stdout
    sys.stdout = log_file_fp
    return (0,log_file_fp,last_print_target)

#restore print target
def restore_print_target(log_file_fp,last_print_target):
    sys.stdout = last_print_target
    log_file_fp.close()
    
#convert sys argvs from str to dictionary    
def mk_param_dic():
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
    #add interl param
    g_params['I_FILE_DIR'] = g_params['PRIM_XML_TEMP_ROOT_DIR'].strip()+'/'+g_params['PRIM_XML_KEY_WORD']

#convert file_id_file(lm_file_id.cfg) to dictionary    
def create_file_id_dic():
    file_name = g_params['PRIM_XML_FILE_ID_FILE'].strip(' ')
    f1=open(file_name,'r')
    for line in f1:
        list_temp=re.split('[ \t\n\r\f\v]+',line)
        if len(list_temp) >= 2 and list_temp[0][-2:]=='.c' and list_temp[1].isdigit():
            g_file_id_dic[list_temp[0]]=list_temp[1]
    print('g_file_id_dic',g_file_id_dic)

#convert mod_id_file(lm_file_id.cfg) to dictionary      
def create_mod_id_dic():
    file_name = g_params['PRIM_XML_MOD_ID_FILE'].strip(' ')
    f1=open(file_name,'r')
    for line in f1:
        list_temp=re.split('[ \t\n\r\f\v]+',line)
        if len(list_temp) >= 2 and list_temp[0][-2:]=='.c' and list_temp[1].isdigit():
            g_mod_id_dic[list_temp[0]]=list_temp[1]
    print('g_mod_id_dic',g_mod_id_dic)  
    
#Generates a list of files(.c) from all subdirectories
def creat_src_file_list():
    full_file_name_list = []
    sub_dir_list = g_params['PRIM_XML_SRC_SUB_DIR'].split(' ')
    for sub_dir in sub_dir_list:
        sub_dir=sub_dir.strip()
        if sub_dir:
            full_dir = g_params['PRIM_XML_SRC_ROOT_DIR']
            full_dir = full_dir.strip()
            full_dir = full_dir+'/'+sub_dir
            print('full_dir=',full_dir)
            file_name_list=os.listdir(full_dir)
            print('file_name_list=',file_name_list)
            for file_name in file_name_list:
                full_file_name=full_dir+'/'+file_name
                if os.path.isfile(full_file_name):
                    print('isfile=',full_file_name)
                    if full_file_name.endswith('.c'):
                        print('cfile=',full_file_name)
                        full_file_name_list.append(full_file_name)
    print('full_file_name_list=',full_file_name_list)
    return full_file_name_list

#pre-compile .c to .i    
def conver_c_2_i(full_file_name_list): 
    dst_full_file_name_list = []
    print('conver_c_2_i=',full_file_name_list)
    temp_dir = g_params['I_FILE_DIR'].strip()
    if os.path.isdir(temp_dir):
        shutil.rmtree(temp_dir)
    os.makedirs(temp_dir) 
    cc=g_params['CC'].strip()
    cflag = g_params['CFLAGS'].strip()
    for src_full_file_name in full_file_name_list:
        src_file_name = src_full_file_name.split('/')[-1]
        
        if src_file_name in g_file_id_dic and src_file_name in g_mod_id_dic:
            print('E2I:',src_file_name,g_file_id_dic[src_file_name],g_mod_id_dic[src_file_name])
            dst_file_name = src_file_name
            dst_file_name=dst_file_name.replace('.c','.i')
            dst_full_file_name = temp_dir+'/'+dst_file_name
            
            cmd_line = cc+' -E '+src_full_file_name+' '+cflag+' -D__FILE_NAME__='+src_file_name+' -D__FILE_IDX__='+g_file_id_dic[src_file_name]+' -D__MOD_IDX__='+g_mod_id_dic[src_file_name]+' -P '+' > '+dst_full_file_name
            print('cmd_line=',cmd_line)
            os.system(cmd_line)
            dst_full_file_name_list.append(dst_full_file_name)
    return dst_full_file_name_list

def para_exist_i_file(file_name):
    key_word = g_params['PRIM_XML_KEY_WORD'].strip()
    parse_file = os.path.dirname(file_name)+'/'+key_word+'.cfg'
    print('file_name parse_file=',file_name,parse_file)
    f1=open(file_name,'r')
    f2=open(parse_file,'w+')
    for line in f1:
        match=re.search('PRIM_XML_HI_DIAG_LOG_MSG_',line)
        if match:
            match_id=re.search('_PRIM_ID_:',line)
            match_sz=re.search(',_PRIM_SZ_:',line)
            match_line=re.search(',_PRIM_LINE_:',line)
            match_file=re.search(',_PRIM_FILE_:',line)
            match_file=re.search(',_PRIM_FILE_:',line)
    f1.close()
    f2.close()
#parse file :get print info 
def cp_effect_content_on_file(cfg_fp,file_name):
    src_fp = open(file_name,'r')
    print('file_name-----',file_name)
    #time.sleep(5)
    for line in src_fp:
        
        match=re.search('_PRIM_ST',line)
        if match:
            print('line-----',line)
            cfg_fp.write(line)
        
def cp_effect_content(dst_full_file_name_list):
    base_folder = g_params['PRIM_XML_TEMP_BASE_ROOT_DIR'].strip()
    key_word = g_params['PRIM_XML_KEY_WORD'].strip()
    #cfg dir not exist-->mk dir 
    print('base_folder',base_folder)
    if not os.path.isdir(base_folder):
        print('base_folder2',base_folder)
        os.makedirs(base_folder)
        
        
    cfg_file_name = base_folder+'/'+key_word+'.cfg'
    #cfg file is exist-->recreate it
    if os.path.isfile(cfg_file_name):
        os.remove(cfg_file_name)
    cfg_fp=open(cfg_file_name,'w+',encoding='UTF-8')

    
    for src_file_name in dst_full_file_name_list:
        time_out_sec = 3
        cur_wati_sec = 0
        b_file_exist = 0
        while cur_wati_sec<time_out_sec:
            if os.path.isfile(src_file_name):
                b_file_exist = 1
                break
            time.sleep(0.1)
            cur_wati_sec = cur_wati_sec+0.1
        if  b_file_exist:
            cp_effect_content_on_file(cfg_fp,src_file_name) 
    
    cfg_fp.close()
    
#main func    
def mk_prim_main():
    print('mk_prim_main in') 
    #creat sys params dictionary
    mk_param_dic()
    #print('g_params',g_params)
    
    ret,log_file_fp,last_print_target = set_print_target(g_params['LOG_FILE_NAME'].strip())
    
    create_file_id_dic()
    create_mod_id_dic()
    
    full_file_name_list = creat_src_file_list()
    dst_full_file_name_list = conver_c_2_i(full_file_name_list)
    cp_effect_content(dst_full_file_name_list)
    restore_print_target(log_file_fp,last_print_target)
    exit(0)  
#test_cmd_line=r'arm-hisiv300-linux-uclibcgnueabi-gcc -E F:/code/liteos_ku/app/mrs/dfx/haiwen.c -fno-aggressive-loop-optimizations -fno-builtin -gdwarf-2 -nostdinc -nostdlib -std=c99 -Wpointer-arith -Wstrict-prototypes -Wno-write-strings -mno-thumb-interwork -ffunction-sections -fdata-sections -fno-exceptions -fno-short-enums -mcpu=arm926ej-s -Wundef -I F:/code/liteos_ku/target/hibuilder/ver_cfg -I F:/code/liteos_ku/liteos/Huawei_LiteOS/kernel/include -I F:/code/liteos_ku/liteos/Huawei_LiteOS/kernel/base/include -I F:/code/liteos_ku/liteos/Huawei_LiteOS/lib/libc/include -I F:/code/liteos_ku/liteos/Huawei_LiteOS/lib/libc/stdio -I F:/code/liteos_ku/liteos/Huawei_LiteOS/lib/libc/time -I F:/code/liteos_ku/liteos/Huawei_LiteOS/lib/libc/include/sys -I F:/code/liteos_ku/liteos/Huawei_LiteOS/lib/libm/include -I F:/code/liteos_ku/liteos/Huawei_LiteOS/lib/libc/include/arch-arm/include -I F:/code/liteos_ku/liteos/Huawei_LiteOS/lib/libc/include/kernel -I F:/code/liteos_ku/liteos/Huawei_LiteOS/lib/libc/include/kernel/arch -I F:/code/liteos_ku/liteos/Huawei_LiteOS/compat/posix/include -I F:/code/liteos_ku/liteos/Huawei_LiteOS/compat/linux/include -I F:/code/liteos_ku/liteos/Huawei_LiteOS/compat/posix/src -I F:/code/liteos_ku/liteos/Huawei_LiteOS/compat/linux/include/linux -I F:/code/liteos_ku/liteos/Huawei_LiteOS/platform/bsp/hi3911/include -I F:/code/liteos_ku/liteos/Huawei_LiteOS/platform/bsp/hi3911/config -I F:/code/liteos_ku/liteos/Huawei_LiteOS/platform/cpu/arm/arm926 -I F:/code/liteos_ku/liteos/Huawei_LiteOS/net/lwip_sack/include/lwip -I F:/code/liteos_ku/liteos/Huawei_LiteOS/net/lwip_sack/include -I F:/code/liteos_ku/liteos/Huawei_LiteOS/net/lwip_sack/include/ipv4 -I F:/code/liteos_ku/include -I F:/code/liteos_ku/include/net/lwip/include -I F:/code/liteos_ku/include/net/lwip/include/ipv4 -I F:/code/liteos_ku/mdm/inc -I F:/code/liteos_ku/mdm/mac/inc -I F:/code/liteos_ku/mdm/drv_vx -I F:/code/liteos_ku/mdm/sal/inc -I F:/code/liteos_ku/mdm/cmn/inc -I F:/code/liteos_ku/mdm/upg -I F:/code/liteos_ku/target/include -I F:/code/liteos_ku/target/include/plc -I F:/code/liteos_ku/target/include/plc/boot -I F:/code/liteos_ku/target/config/diag -I F:/code/liteos_ku/target/config/app -I F:/code/liteos_ku/target/config/nv -I F:/code/liteos_ku/target/config/product/cfg -I F:/code/liteos_ku/target/config/newbee -DCYGPKG_POSIX_SIGNALS -D__ECOS__ -D__RTOS_ -DOBC_IMAGE_MODEM -O2 -DPRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO -DMAKE_PRIM_XML_PROCESS_IN -P  > F:/code/liteos_ku/target/delivery/HiMDL_release/TEMP_DIR/CCO/PRIM/app/mrs/app_dfx.c'    

#sysresult=os.system(r'arm-hisiv300-linux-uclibcgnueabi-gcc -E F:/code/liteos_ku/app/mrs/dfx/app_dfx.c > F:/code/liteos_ku/target/delivery/HiMDL_release/TEMP_DIR/CCO/PRIM/app/mrs/app_dfx.c')
#sysresult=os.system(test_cmd_line)
#result=os.popen(r"F:/wangjun/TestLongTimeSleep.exe")

#while 1:
#print('--------------',sysresult)
#print('result=',result,result.read())
#print('resultread=',result.read())
#print('sysresult=',sysresult)
#time.sleep(1)

mk_prim_main()
