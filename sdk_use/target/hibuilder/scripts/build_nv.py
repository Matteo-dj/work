import os,sys,re
import configparser
import shutil
import xml.etree.ElementTree as ET
import copy
from ctypes import *
import time,hashlib,binascii
import datetime
#-----------------------------------基本错误定义-----------------------------------
HI_ERR_SUCCESS=0
#-----------------------------------基本类型定义-----------------------------------
HI_CHAR=c_char
HI_S8=c_byte
HI_U8=c_ubyte
HI_S16=c_short
HI_U16=c_ushort
HI_S32=c_int
HI_U32=c_uint

#-----------------------------------NV结构体定义-----------------------------------
class HI_UE_SOFT_VERSION_S(Structure):
    _fields_ = [
        ("ulMajorMinorVersion", HI_U32),
        ("ulRevisionVersion", HI_U32),
        ("ulBuildVersion", HI_U32),
        ]

class NV_FILE_SECTION_ID_STRU(Structure):
    _fields_ = [
        ("ulId", HI_U32),
        ("ulSectionSize", HI_U32),
        ]
        
class NV_FILE_STRU(Structure):
    _fields_ = [
        ("ulSigCode", HI_U32),
        ("ulStartAddr", HI_U32),
        ("szDateTime", HI_CHAR*16),
        ("ulPid", HI_U32),
        ("ulIid", HI_U32),
        
        ("ulNvVer", HI_U32),
        ("szFileName", HI_CHAR*64),
        ("usSectionNum", HI_U16),
        ("usType", HI_U16),
        ("stUeSoftPrivateVer", HI_UE_SOFT_VERSION_S),
        
        ("szManufacturerId", HI_CHAR*32),
        ("szModelId", HI_CHAR*32),
        ("szProductName", HI_CHAR*32),
        ("szProductNamePrivate", HI_CHAR*32),
        ("szHwName", HI_CHAR*32),
        
        ("szHwNamePrivate", HI_CHAR*32),
        ("ulTotalFileSize", HI_U32),
        ("stSectinId", NV_FILE_SECTION_ID_STRU*8),
        ("ulChecksum", HI_U32),
        ("ulReserved", HI_CHAR*32),#HI_U8 实际内容是字符串，因此换成char类型
        
        ("ulSectionDataSize", HI_U32),
        ("ucSectoinData", HI_U8*0),
        ]        
###############################NV 分区头格式 ###############################################        
class NV_FILE_SECTION_STRU(Structure):
    _fields_ = [
        ("ulId", HI_U32),
        ("szName", HI_CHAR*64),
        ("ulNvIdRange", HI_U32*2),
        ("usTotalNviNumber", HI_U16),
        ("usNviMaxLength", HI_U16),
        
        ("ulOwner", HI_U32),
        ("ulFeature", HI_U32),
        ("ucReserved", HI_U8*32),
        ("ulTotalNviSize", HI_U32),
        ]

###############################NV项头信息###############################################  
class NV_ITEM_STRU(Structure):
    _fields_ = [
        ("ulNvId", HI_U16),
        ("usNvIdLength", HI_U16),
        ("ucNvData", HI_U8*0),
        ]

        
#-----------------------------------生成HNV文件流程-----------------------------------

'''
功能:生成nv_section信息
入参
grp_element:ET group节点
src_bin_dic:nv bin字典  {'0xf01':bytearray(...),....}
返回值
ret错误吗
src_str:拼接后字符串
section_bin: 类型bytearray   分区bin文件
''' 
def nv_process_mk_section(grp_element,src_bin_dic):
    nv_id_list=[]
    nv_item_bin_list=[]
    nv_total_size=0
    max_id=0
    min_id=0xffffffff
    nv_total_num=0
    nv_max_len=0
    
    #1.将所有NV ID排序
    for nv_element in grp_element:
        nv_id_list.append(int(nv_element.attrib['ID'],16))   
    nv_id_list.sort()
    #print(nv_id_list)

    #2.按照ID顺序生成nv_item_bin(含nv_item头和nv_item内容),添加到nv_item_bin_list中
    for nv_id in nv_id_list:
        #申请nv_item_bin空间并转换成nv_item_st
        src_bin=src_bin_dic[hex(nv_id)]   
        nv_item_bin=bytearray(sizeof(NV_ITEM_STRU)+len(src_bin))#申请空间
        nv_item_st=NV_ITEM_STRU.from_buffer(nv_item_bin)
        
        #填充nv_item_bin,并将nv_item_bin添加到nv_item_bin_list中
        nv_item_st.ulNvId=nv_id#赋值ID
        nv_item_st.usNvIdLength=len(src_bin)#赋值NV长度
        nv_item_bin[sizeof(NV_ITEM_STRU):]=src_bin#拷贝bin
        nv_item_bin_list.append(nv_item_bin)
        
        #确定section中最大nv id和最小nv id
        if nv_id>max_id:
            max_id=nv_id
        if nv_id <min_id:
            min_id=nv_id
            
        #确定分组中最长nv长度
        if nv_item_st.usNvIdLength>nv_max_len:
            nv_max_len=nv_item_st.usNvIdLength
        
        #统计分组nv_item_bin总大小,统计nv总个数
        nv_total_size=nv_total_size+len(nv_item_bin)#计算所有nv长度
        nv_total_num=nv_total_num+1
    
    
    #3.生成section_bin
    #申请section_bin空间并转换成section_st
    section_bin=bytearray(sizeof(NV_FILE_SECTION_STRU)+nv_total_size)
    section_st=NV_FILE_SECTION_STRU.from_buffer(section_bin)
    
    #对section_st赋值
    section_st.ulId=hi_str_to_int(grp_element.attrib['ID'],None)
    section_st.szName=grp_element.attrib['NAME'].encode('utf-8')
    section_st.ulNvIdRange[0]=min_id
    section_st.ulNvIdRange[1]=max_id
    section_st.usTotalNviNumber=nv_total_num
    section_st.usNviMaxLength=nv_max_len
    section_st.ulOwner=0#不知道是啥
    feature_list=grp_element.attrib['FEATURE'].split(',')#这步最终删除
    for feature in feature_list:
        section_st.ulFeature=section_st.ulFeature+eval(feature)
    #section_st.ucReserved=
    section_st.ulTotalNviSize=nv_total_size
    
    #将nv_item_bin依次拷贝到section_bin的后面
    offset=sizeof(NV_FILE_SECTION_STRU)
    for nv_item_bin in nv_item_bin_list:
        section_bin[offset:]=nv_item_bin
        offset=offset+len(nv_item_bin)  
        
    #print(hex(id(section_st)),hex(id(section_st.szName)),hex(id(section_bin)),hex(id(section_bin[0])),hex(id(section_bin[1])),hex(id(section_bin[2])))
    return (HI_ERR_SUCCESS,section_bin)
    
    
   
'''
功能:生成hnv文件
入参
element_root:nv根节点
src_bin_dic:nv bin字典  {'0xf01':bytearray(...),....}
nv_file_name:填充到nv_file_st.szFileName
reserver_path:填充到nv_file_st.ulReserved,与V100保持一致存储编译路径
返回值
ret错误吗
nv_file_Bin: 类型bytearray   nv文件内容
'''     
def hi_nv_process_mk_hnv(element_root,src_bin_dic,nv_file_name,reserver_path=None):
    #print(src_bin_dic)
    
    section_bin_list=[]
    
    


    
    '''
    i=0
    j=0
    while i < len(element_grp_list):
        j=i
        while(j<len(element_grp_list)):
            if grp_list[i].ulId > grp_list[j].ulId:
                temp_grp=grp_list[i]
                temp_grp_bin=section_bin_list[i]
                grp_list[i]=grp_list[j]
                section_bin_list[i]=section_bin_list[j]
                grp_list[j]=temp_grp
                section_bin_list[j]=temp_grp_bin
            j=j+1
        i=i+1
    '''
    
    #2.生成section_bin,并将section_bin 添加到 section_bin_list
    section_num=0
    nv_total_size=0
    for element_grp in element_root:
        (ret,section_bin)=nv_process_mk_section(element_grp,src_bin_dic)
        if ret!=HI_ERR_SUCCESS:
            return(ret,None)   
        section_bin_list.append(section_bin)
        section_num=section_num+1
        nv_total_size=nv_total_size+len(section_bin)
    
    

    
    #3.申请nv_file_Bin空间,转成nv_file_st结构并填充nv_file_st
    nv_file_Bin=bytearray(sizeof(NV_FILE_STRU)+nv_total_size)
    nv_file_st=NV_FILE_STRU.from_buffer(nv_file_Bin)
    
    #填充nv_file_st
    nv_file_st.ulSigCode=0x24564e48#HNV$
    nv_file_st.ulStartAddr=0
    time_str=time.strftime('%Y%m%d %H:%M',time.localtime(time.time()))
    #print(time_str)
    nv_file_st.szDateTime=time_str.encode('utf-8')
    nv_file_st.ulPid=0x121#插件id,hso上插件id 目前好像没用
    nv_file_st.ulIid=0x1#实例ID？目前好像没用
    
    nv_file_st.ulNvVer=0#目前好像没用
    nv_file_st.szFileName=nv_file_name.encode('utf-8')#文件名,没什么实际用
    nv_file_st.usSectionNum=section_num
    nv_file_st.usType=0#目前好像没用
    
    nv_file_st.stUeSoftPrivateVer.ulMajorMinorVersion=0x10000#目前好像没用
    nv_file_st.stUeSoftPrivateVer.ulRevisionVersion=0#目前好像没用
    nv_file_st.stUeSoftPrivateVer.ulBuildVersion=0#目前好像没用
    
    nv_file_st.szManufacturerId='HUAWEI'.encode('utf-8')
    nv_file_st.szModelId='LTE'.encode('utf-8')
    nv_file_st.szProductName='Modem'.encode('utf-8')
    nv_file_st.szProductNamePrivate='Modem'.encode('utf-8')
    nv_file_st.szHwName='03020QXR'.encode('utf-8')
    
    nv_file_st.szHwNamePrivate='MT1B390A'.encode('utf-8')
    nv_file_st.ulTotalFileSize=len(nv_file_Bin)
    i=0
    while i < section_num: 
        section_st=NV_FILE_SECTION_STRU.from_buffer(section_bin_list[i])
        nv_file_st.stSectinId[i].ulId=section_st.ulId
        nv_file_st.stSectinId[i].ulSectionSize=section_st.ulTotalNviSize
        i=i+1
        
    nv_file_st.ulChecksum=0#当前没用？
    if reserver_path==None:
        #print(type(nv_file_st.ulReserved),type(""))
        nv_file_st.ulReserved=b""#编译路径，没啥用
    else:
        nv_file_st.ulReserved=reserver_path.encode('utf-8')#编译路径，没啥用
        
    nv_file_st.ulSectionDataSize=nv_total_size
    #nv_file_st.ucSectoinData=
    
    
    #将section_bin依次添加到nv_file_Bin后面
    offset=sizeof(NV_FILE_STRU)
    for section_bin in section_bin_list:
        nv_file_Bin[offset:]=section_bin
        offset=offset+len(section_bin)
   
    return (HI_ERR_SUCCESS,nv_file_Bin)

         
HI_CFG_CUR_DEEP=4


    
class NvEnv:
    def __init__(self):
        self.app_name=[]#app名称
        self.root_dir=[]#编译根目录
        self.mdm_xml=[]#mdm_xml全路径
        self.app_xml=[]#app_xml全路径
        self.app_sdk_level=[]#app sdk等级
        self.app_cfg_dir=[]#客制化文件夹路径,带产品形态路径
        self.src_bin_dir=[]#原bin路径,带产品形态路径
        
        self.product_type=[]#产品形态名称
        self.temp_dir=[]#临时文件夹路径,带产品形态
        self.combin_xml=[]#合并后xml全路径,带or不带产品形态
        self.nv_tool_dir=[]#
        self.nv_h_dir=[]#
        self.nv_bin_max_size=0
        
g_nv_env = NvEnv()

def hi_str_to_int(line,num):
    if num!=None:
        return int(line,num)
    match1=re.match(r'\s*0x',line)
    match2=re.match(r'\s*0X',line)
    if match1 or match2:
        return int(line,16)
    else:
        return int(line,10)

def hi_nv_set_cfg(root_dir,product_lib_out,xml_hfile_dir,product_name,app_name,hnv_dir,str_pwd):
    global g_nv_env
    
    #cur_dir =sys.path[0]
    #root_dir=cur_dir
    #i=0
    #while i<HI_CFG_CUR_DEEP:
        #root_dir=os.path.dirname(root_dir)
        #i=i+1
        
    g_nv_env.pwd=str_pwd
    g_nv_env.hnv_dir=hnv_dir#hnv存放位置
    g_nv_env.nv_h_dir=xml_hfile_dir#存放结构体txt文件存放位置
    g_nv_env.app_name=app_name#app名称
    g_nv_env.product_type=product_name#产品形态名称
    g_nv_env.root_dir=root_dir#根目录
    g_nv_env.temp_dir=product_lib_out#临时文件目录
    
    g_nv_env.mdm_xml=os.path.join(root_dir,'nv','src','mss_nvi_db.xml')#mss_nvi_db.xml路径
    g_nv_env.app_xml=os.path.join(root_dir,'app',g_nv_env.app_name,'nv','app_nvi_db.xml')#app_nvi_db.xml路径
    g_nv_env.combin_xml=os.path.join(g_nv_env.temp_dir,'mss_nvi_db.xml')#合并后combin.xml路径
    g_nv_env.combin_product_xml=os.path.join(g_nv_env.temp_dir,'mss_nvi_db_'+g_nv_env.product_type+'.xml')#合并分割后combin_coo.xml路径
    g_nv_env.combin_product_security_xml=os.path.join(g_nv_env.temp_dir,'combin_'+g_nv_env.product_type+'_security.xml')#合并分割后combin_coo.xml路径
    g_nv_env.app_cfg_dir=os.path.join(root_dir,'app',g_nv_env.app_name,'nv','cfg',g_nv_env.product_type)#存放cfg文件路径
    g_nv_env.src_bin_dir=os.path.join(root_dir,'app',g_nv_env.app_name,'nv','src_bin',g_nv_env.product_type)#存放必源src bin文件路径
    #g_nv_env.app_sdk_level='A-SDK'
    g_nv_env.nv_tool=os.path.join(root_dir,'tools','nv','NVCodecTool.exe')#nv工具路径
    g_nv_env.nv_bin_max_size=0x2000#TODO 需要与代码同源获取
    if g_nv_env.hnv_dir==None:
        g_nv_env.hnv_dir=g_nv_env.temp_dir
    
    #print("g_nv_env.hnv_dir=%s"%(g_nv_env.hnv_dir))
    #print("g_nv_env.nv_h_dir=%s"%(g_nv_env.nv_h_dir))
    #print("g_nv_env.app_name=%s"%(g_nv_env.app_name))
    #print("g_nv_env.product_type=%s"%(g_nv_env.product_type))
    #print("g_nv_env.root_dir=%s"%(g_nv_env.root_dir))
    #print("g_nv_env.temp_dir=%s"%(g_nv_env.temp_dir))
    #print("g_nv_env.mdm_xml=%s"%(g_nv_env.mdm_xml))
    #print("g_nv_env.app_xml=%s"%(g_nv_env.app_xml))
    #print("g_nv_env.combin_xml=%s"%(g_nv_env.combin_xml))
    #print("g_nv_env.app_cfg_dir=%s"%(g_nv_env.app_cfg_dir))
    #print("g_nv_env.src_bin_dir=%s"%(g_nv_env.src_bin_dir))
    #print("g_nv_env.nv_tool=%s"%(g_nv_env.nv_tool))


    
def hi_nv_fomat_nv_id(root):
    ret = 0
    for grp in root:
        for nv in grp:
            if 'ID' in nv.attrib.keys():
                str_id=nv.attrib['ID']
                int_id=hi_str_to_int(str_id,None)
                str_id=hex(int_id)
                nv.attrib['ID']=str_id
    return 0

def pwd_to_nv_val(pwd):
    pwd=pwd.strip()
    password=pwd.encode('utf-8')
    #print('password111111111111111111111111111111111',type(password),password)    
    salt = b'hct default salt'
    sp = hashlib.pbkdf2_hmac('sha256',password,b'hct default salt', 5000)
    ak = binascii.hexlify(sp)
    #print (sp)
    
    sh = hashlib.sha256()
    sh.update(sp)
    sha256 = sh.hexdigest()
    sha_string = '['
    for i in range(32):
        sha_string += '0x'
        sha_string += sha256[2*i]
        sha_string += sha256[2*i + 1]
        if(i != 31):
            sha_string += ','
        else:
            sha_string += ']'
    #transter hex to asiic
    crc_assic = binascii.a2b_hex(sha256.encode('utf-8'))
    crc  = (binascii.crc32(crc_assic))
            
    magic_ver = r'{0x505744,0x1}'
    salt_string = r'[0x68,0x63,0x74,0x20,0x64,0x65,0x66,0x61,0x75,0x6c,0x74,0x20,0x73,0x61,0x6c,0x74]'
    result = '{'+magic_ver+ ',' +sha_string + ',' + hex(crc) + ',' + salt_string + r',5000,[0,0,0]'+'}'
    #print ('result======',result)
    return result    
    
    
def make_0x8009_nv():
    global g_nv_env
    #"{V100R003C00SPC001B011,0x1301,01,[16,6,3],[0,]}"
    str_0x8009="{asiic_ver,bcd_ver,asic_chip,[year,month,day],[0,]}"

    

    
    #str_date=datetime.datetime.now().strftime('%y,%m,%d')#直接转出的值已0开头时生成nv工具无法解析。如:[2017,01,01]

    
    #替换版本号asiic_ver
    ver_h_file=os.path.join(g_nv_env.root_dir,'target','hibuilder','ver_cfg','hb_ver_cfg.h')
    fp=open(ver_h_file,'r')
    lines=fp.readlines()
    for line in lines:
        if line.find('PRODUCT_CFG_VERSION_STR')>=0:
            list=line.split("\"")
            sdk_ver =list[1].strip()
            main_ver = sdk_ver.split("B")
            str_ver = main_ver[0].strip()
    fp.close()
    
    str_0x8009=str_0x8009.replace('asiic_ver',str_ver)
    
    #替换BCD版本号
    #V100R003C00SPC001B011
    #V200R001C00B052
    
    pos=str_ver.lower().find('spc')
    if pos>=0:
        str_spc=str_ver[pos+4:pos+6]
    else:
        str_spc='00'
        
    pos=str_ver.lower().find('v')
    if pos>=0:
        str_v=str_ver[pos+1:pos+2]
    else:
        print('err ver',str_ver)
        exit(-1) 
        
    pos=str_ver.lower().find('r')
    if pos>=0:
        str_r=str_ver[pos+3:pos+4]
    else:
        print('err ver',str_ver)
        exit(-1)  
        

    str_0x8009=str_0x8009.replace('bcd_ver',"0x"+str_v+str_r+str_spc)  
    
    
    #替换编译时间年月日
    now=datetime.datetime.now()
    year=str(now.year%100)
    month=str(now.month)
    day=str(now.day)
    str_0x8009=str_0x8009.replace('year',year)
    str_0x8009=str_0x8009.replace('month',month)
    str_0x8009=str_0x8009.replace('day',day)
    
    #替换chip类型asic_chip
    if g_nv_env.product_type=="STA":
        str_0x8009=str_0x8009.replace('asic_chip',"03")
    else:
        str_0x8009=str_0x8009.replace('asic_chip',"03")
        
    print('str_0x8009',str_0x8009)  
    return str_0x8009
"""
合并xml;nv id标准化;nv 按组号进行排序
"""    
def HI_NV_XmlCombineXml(src_xml_list,combin_xml):
    root_list=[]
    group_dics={}
    exist_item_id_list=[]
    global g_nv_env
    
    for src_xml in src_xml_list:
        tree = ET.parse(src_xml)
        root = tree.getroot()
        hi_nv_fomat_nv_id(root)#标准化NVID
        root_list.append(root)
    
    new_root=ET.Element('HISTUDIO')
    
    #判断是否有重复的ID
    for root in root_list:
        grp_list=root.findall('GROUP')#找到所有的grp
        for grp in grp_list:
            item_list=grp.findall('NV')
            for item in item_list:
                str_id=item.attrib['ID']
                int_id=hi_str_to_int(str_id,None)
                if int_id in exist_item_id_list:
                    print(exist_item_id_list)
                    return -1
                exist_item_id_list.append(int_id)
                    
    #合并xml                
    for root in root_list:
        grp_list=root.findall('GROUP')#找到所有的grp
        for grp in grp_list:
            grp_name=grp.attrib['NAME']
            if not grp_name in group_dics.keys():
                new_root.append(grp)
                group_dics[grp_name]=grp
            else:
                old_grp=group_dics[grp_name]
                sub_elements=grp.findall('NV')
                old_grp.extend(sub_elements)
               



    #对新xml进行排序
    element_grp_list=[]
    for element_grp in new_root:
        element_grp_list.append(element_grp)
   
    element_grp_list.sort(key=lambda x:int(x.attrib['ID'],16))
    i=0
    while i< len(element_grp_list):
        new_root[i]=element_grp_list[i]
        i=i+1

    #临时放在这
    nv_val=pwd_to_nv_val(g_nv_env.pwd)
    nv_replace_val(new_root,'0xfa7',nv_val,True)
    
    #临时放在这
    str_0x8009=make_0x8009_nv()
    nv_replace_val(new_root,'0x8009',str_0x8009,True)
    
    
    #回写xml 
    new_tree=ET.ElementTree(new_root)
    new_tree.write(combin_xml,encoding="utf-8")
    

def hi_nv_mk_temp_dir():
    global g_nv_env
    #print(g_nv_env.temp_dir)
    if os.path.exists(g_nv_env.temp_dir):
        shutil.rmtree(g_nv_env.temp_dir)
        os.makedirs(g_nv_env.temp_dir)
    else:
        os.makedirs(g_nv_env.temp_dir)
        
    if not os.path.exists(g_nv_env.hnv_dir):    
        os.makedirs(g_nv_env.hnv_dir)
        
"""
合并mss和app xml
"""        
def hi_nv_combine_mss_and_app_xml():
    global g_nv_env
    src_xml_list=[]
    src_xml_list.append(g_nv_env.mdm_xml)
    if os.path.exists(g_nv_env.app_xml):
        src_xml_list.append(g_nv_env.app_xml)    
    HI_NV_XmlCombineXml(src_xml_list,g_nv_env.combin_xml)
    
      
      
def HI_NV_XmlSplitByProductType(src_xml,product_list,dst_xml_list): 
    new_root_list=[]
    tree = ET.parse(src_xml)
    root = tree.getroot()
    i=0
    #print('src_xml',src_xml)
    #print('product_list',product_list)
    #print('dst_xml_list',dst_xml_list)
    while i<len(product_list):
        new_root=copy.deepcopy(root)
        for grp in new_root:
            del_nv_list=[]
            for nv in grp:
                list=nv.attrib['DEV'].split('-')
                #print('list',list)
                if product_list[i] not in list:
                    del_nv_list.append(nv)
                else:
                    pass
                    #del nv.attrib['DEV']#删除dev属性
            for nv in del_nv_list:
                grp.remove(nv)
        new_tree=ET.ElementTree(new_root)
        new_tree.write(dst_xml_list[i],encoding="utf-8")
        i=i+1


        
def hi_nv_split_by_product_type():
    global g_nv_env
    product_list=[]
    product_list.append(g_nv_env.product_type)
    dst_xml_list=[]
    dst_xml_list.append(g_nv_env.combin_product_xml)
    HI_NV_XmlSplitByProductType(g_nv_env.combin_xml,product_list,dst_xml_list)

    
    

    
def nv_replace_val(root,str_id,str_val,bForce):
    #print(str_id,str_val)

    for grp in root:
        for nv in grp:
            if nv.attrib['ID']==str_id:
                if 0:#nv.attrib['SECURITY']=='PRIVATE':
                    return -1#私有NV不允许客制化
                else:
                    nv.attrib['PARAM_VALUE']=str_val
                
    return 0    


    
def HI_NV_XmlModifyByCfg(cfg_file,src_xml,dst_xml):
    cfg_fp=open(cfg_file)
    lines=cfg_fp.readlines()
    tree = ET.parse(src_xml)
    root = tree.getroot()
    
    str_id=''
    str_val=''
    for line in lines:
        L=line.split('=')
        if len(L)==2:
            key=L[0].strip()
            val=L[1].strip()
            if key=='ID':
                int_id=hi_str_to_int(val,None)
                str_id=hex(int_id)
            elif key=='PARAM_VALUE':
                str_val=val
                nv_replace_val(root,str_id,str_val,False)
            else:
                pass  

    new_tree=ET.ElementTree(root)
    new_tree.write(dst_xml,encoding="utf-8")
        
    cfg_fp.close()


    
def HI_NV_Xml2Bin(nv_tool,src_xml,dst_bin_dir,include_dir):
    cmd_line=nv_tool
    cmd_line=cmd_line+' '+src_xml
    cmd_line=cmd_line+' '+include_dir
    cmd_line=cmd_line+' '+dst_bin_dir
    #cmd_line=cmd_line+' >> '+r'F:\code\v20_0819\target\hibuilder\scripts\nv\log.txt'
    #print("cmd_line=",cmd_line)
    ret=os.system(cmd_line)
    if ret!=0:
        print('error'+cmd_line)
        sys.exit(ret)


        
def HI_NV_HNV_Build(src_xml,dst_hnv,bin_dir):
    global g_nv_env
    #print("src_xml=",src_xml)
    #print("dst_hnv=",dst_hnv)
    #print("bin_dir=",bin_dir)
    #input("继续")
    
    src_bin_dic={}
    tree = ET.parse(src_xml)
    root = tree.getroot()
    
    
    bin_file_list=os.listdir(bin_dir)
    
    #生成bin dictionary
    for bin_file in bin_file_list:
        if bin_file.endswith(".bin"):
            bin_id=bin_file[:-len('.bin')]
            bin_file_full_name=os.path.join(bin_dir,bin_file)
            fp=open(bin_file_full_name,'rb')
            bin=fp.read()
            src_bin_dic[bin_id]=bin
            #print(src_bin_dic)
            #input("继续")
            fp.close()
    
    (ret,bytearray_hnv)=hi_nv_process_mk_hnv(root,src_bin_dic,os.path.basename(dst_hnv),reserver_path=None)
    if len(bytearray_hnv)>=g_nv_env.nv_bin_max_size:#检测NV文件大小,边界也有问题
        print(dst_hnv,"error nv size large than nv_bin_max_size(%u)"%(g_nv_env.nv_bin_max_size))
        exit(-1)
    if ret==HI_ERR_SUCCESS:
        hnvfp=open(dst_hnv,'wb+')
        hnvfp.write(bytearray_hnv)
        hnvfp.close()
    #input("continue")

    
def hi_nv_modify_by_cfg():
    global g_nv_env
    cfg_file_list=os.listdir(g_nv_env.app_cfg_dir)
    for cfg_file in cfg_file_list:
        if cfg_file.endswith(".cfg"):
            cfg_file=cfg_file[:-len('.cfg')]           
            cfg_file_full_name=os.path.join(g_nv_env.app_cfg_dir,cfg_file+'.cfg')
            cfg_xml_full_name=os.path.join(g_nv_env.temp_dir,cfg_file+'.xml')
            cfg_hnv_full_name=os.path.join(g_nv_env.hnv_dir,cfg_file+'.xml.hnv')
            cfg_bin_dir=os.path.join(g_nv_env.temp_dir,cfg_file)
            if os.path.exists(cfg_bin_dir):
                shutil.rmtree(cfg_bin_dir)
                os.makedirs(cfg_bin_dir)
            else:
                os.makedirs(cfg_bin_dir)
                
            HI_NV_XmlModifyByCfg(cfg_file_full_name,g_nv_env.combin_product_xml,cfg_xml_full_name)
            
            HI_NV_Xml2Bin(g_nv_env.nv_tool,cfg_xml_full_name,cfg_bin_dir,g_nv_env.nv_h_dir)
            #将xml和bin文件合成hnv文件
            HI_NV_HNV_Build(cfg_xml_full_name,cfg_hnv_full_name,cfg_bin_dir)

#print('zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz')
#argvi=0
#while argvi<len(sys.argv):
    #print('argv[%u]'%(argvi),sys.argv[argvi])
    #argvi=argvi+1

    
#argv[0] F:/code/v200_zuixin_gai/target/hibuilder/scripts/build_nv.py
#argv[1] F:/code/v200_zuixin_gai
#argv[2] F:/code/v200_zuixin_gai/target/delivery/HiMDL_release/CCO/nv
#argv[3] F:/code/v200_zuixin_gai/target/delivery/HiMDL_release/CCO/hdb/nv/
#argv[4] CCO
#argv[5] ,MRS,

    
g_root_dir=sys.argv[1]
g_product_lib_out=sys.argv[2]
g_xml_hfile_dir=sys.argv[3]
g_product_name=sys.argv[4]
g_app_name_list_str=sys.argv[5]
g_str_pwd=sys.argv[6]
g_hnv_dir=sys.argv[7]

#g_root_dir=r'F:/code/v200_zuixin_gai'
#g_product_lib_out=r'F:/code/v200_zuixin_gai/target/delivery/HiMDL_release/CCO/nv'
#g_xml_hfile_dir=r'F:/code/v200_zuixin_gai/target/delivery/HiMDL_release/CCO/hdb/nv/'
#g_product_name='CCO'
#g_app_name_list_str=',MRS,'
#g_str_pwd="mima"


def HI_NV_MakeMain(root_dir,product_lib_out,xml_hfile_dir,product_name,app_name_list_str,str_pwd):
    global g_hnv_dir
    str_pwd=str_pwd.strip()
    app_name=app_name_list_str.strip(',').lower()
    hi_nv_set_cfg(root_dir,product_lib_out,xml_hfile_dir,product_name,app_name,g_hnv_dir,str_pwd)
    hi_nv_mk_temp_dir()
    hi_nv_combine_mss_and_app_xml()
    hi_nv_split_by_product_type()
    hi_nv_modify_by_cfg()
    
HI_NV_MakeMain(g_root_dir,g_product_lib_out,g_xml_hfile_dir,g_product_name,g_app_name_list_str,g_str_pwd)

#hi_nv_set_cfg(root_dir,include_file_dir,hnv_dir,'mrs',product_type)
#hi_nv_mk_temp_dir()
#hi_nv_combine_mss_and_app_xml()
#hi_nv_split_by_product_type()
#hi_nv_modify_by_cfg()

