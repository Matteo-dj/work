import os, time, string, re ,shutil, time, hashlib, binascii, sys
import xml.etree.ElementTree as ET
#g_temp_dir= r'F:\code\liteos_ku\target\delivery\HiMDL_Release\TEMP_DIR\CCO\PRIM\base'
#g_temp_xml_file = r'F:\code\liteos_ku\target\config\hdbcfg\mss_prim_db.xml'
g_params = {}

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
        
def int_prim_xml_tree(tree):
    
    root = tree.getroot()
    msg_root_element = ET.Element('')
    for child_1 in root:
        if child_1.tag=='SUBSYSTEM':
            for child_2 in child_1:
                if child_2.tag=='USERPLANE_LOG' and child_2.attrib['NAME']=='MAC_PHY_UL_GRANT_HARQ_IND':
                    msg_root_element = child_2
                    break
            break
                  
    if  msg_root_element.tag=='USERPLANE_LOG':
        temp_attrib=msg_root_element.attrib
        temp_text=msg_root_element.text
        temp_tail = msg_root_element.tail
        msg_root_element.clear()
        msg_root_element.attrib = temp_attrib
        msg_root_element.text = temp_text
        msg_root_element.tail = temp_tail

    return msg_root_element
    
#ET.dump(msg_root_element)
#tree.write('mss_prim_db.xml',"UTF-8")
def write_prim_xml_tree(tree):
    tree.write(g_params['PRIM_XML_DST_XML_FILE'],encoding="UTF-8",xml_declaration = '<?xml version="1.0" encoding="utf-8" ?>',method="xml",short_empty_elements=True )
#{_PRIM_ST,_PRIM_PRI_=1,_PRIM_ID_=SOCKET_FILE_LOG_FLAG_021,_PRIM_SZ_="LOG_021",_PRIM_LINE_=6,_PRIM_FILE_=haiwen.c,_PRIM_FILE_ID_=6066,_PRIM_MOD_ID_=813,_PRIM_END_};  
def add_content_to_xml(file_name,msg_root_element):
    print(file_name)
    src_fp = open(file_name,encoding='UTF-8')
    for line in src_fp:
        match_st = re.search('_PRIM_ST',line)
        match_pri = re.search(',_PRIM_PRI_=',line)
        match_id =  re.search(',_PRIM_ID_=',line)
        match_sz = re.search(',_PRIM_SZ_=',line)
        match_line = re.search(',_PRIM_LINE_=',line)
        match_file = re.search(',_PRIM_FILE_=',line)
        match_file_id =re.search(',_PRIM_FILE_ID_=',line)
        match_mod_id = re.search(',_PRIM_MOD_ID_=',line)
        match_end = re.search(',_PRIM_END_',line)
        if match_st and match_pri and match_id and match_sz and match_line and match_file and match_file_id and match_mod_id and match_end:
            prim_pri  = line[match_pri.end():match_id.start()]
            prim_id = line[match_id.end():match_sz.start()]
            prim_sz = line[match_sz.end():match_line.start()].strip(r'"')
            prim_line = line[match_line.end():match_file.start()]
            prim_file = line[match_file.end():match_file_id.start()]
            prim_file_id = line[match_file_id.end():match_mod_id.start()]
            prim_mod_id = line[match_mod_id.end():match_end.start()]
            #LOG_BUF
            if int(prim_pri)==0:
                real_pri = int(prim_pri)
                pri_str = 'ERROR'
            elif int(prim_pri)==1:
                real_pri = int(prim_pri)
                pri_str = 'WARNING'
            elif int(prim_pri)==2:
                real_pri = int(prim_pri)
                pri_str = 'INFO'
            elif int(prim_pri)==3:
                real_pri = int(0)
                pri_str = 'BUF'  
                
            if prim_id.isdigit():
                if int(prim_id)==0:
                    real_id = int(prim_line)
                else:
                    real_id = int(prim_id)
            else:
                i = 1
                while 1:
                    if not prim_id[-i:].isdigit():
                        break;
                    i=i+1
                
                if(i==1):
                    real_id = 0
                else:
                    i=i-1;
                    real_id = int(prim_id[-i:])
                
             
            print('prim_pri=',prim_pri)
            print('prim_id=',prim_id)
            print('prim_sz=',prim_sz)
            print('prim_line=',prim_line)
            print('prim_file=',prim_file)
            print('prim_file_id=',prim_file_id)
            print('prim_mod_id=',prim_mod_id)
            
            print('real_pri=',real_pri)
            print('pri_str=',pri_str)
            print('real_id=',real_id)
            
            xml_id = (int(prim_file_id)<<16) |  (real_id<<4) |(real_pri)
            print('xml_id=',hex(xml_id))
            new_element = ET.Element('')
            new_element.tag = 'MSG'
            new_element.attrib['STRUCTURE'] = 'DIAG_LOG_MSG_S'
            new_element.attrib['NAME'] = prim_sz+' @'+prim_file+'('+prim_line+'),'+pri_str
            new_element.attrib['ID'] = hex(xml_id)
            #自己无法对齐
            new_element.tail = '\n\t\t\t'
            msg_root_element.append(new_element)
        else:
            pass
        
        
#ET.dump(msg_root_element)        
    src_fp.close()        
        
def add_content_files_to_xml(base_file_dir,msg_root_element):
    if os.path.isdir(base_file_dir):
        file_name_list=os.listdir(base_file_dir)
        for file_name in file_name_list:
            add_content_to_xml(base_file_dir+'/'+file_name,msg_root_element)
        
def mk_prim_xml_main():
    mk_param_dic()
    ret,log_file_fp,last_print_target = set_print_target(g_params['LOG_FILE_NAME'].strip())
    tree = ET.parse(g_params['PRIM_XML_SRC_XML_FILE'].strip())
    msg_root_element = int_prim_xml_tree(tree)
    #ET.dump(msg_root_element)
    add_content_files_to_xml(g_params['PRIM_XML_TEMP_BASE_ROOT_DIR'].strip(),msg_root_element)
    write_prim_xml_tree(tree)
    restore_print_target(log_file_fp,last_print_target)
    exit(0)  

mk_prim_xml_main()