#! python
# -*- coding: utf-8 -*- 
import os, time, string, re ,shutil, time, xml.etree.ElementTree as ET
from configparser import ConfigParser
#=================================================#
os.popen('python parse_cfg.py')
time.sleep(1)
cf = ConfigParser()
cf.read('inner_ver.ini')
product_ver     = cf.get('product','product_ver')

cfg = ConfigParser()
cfg.read('config.ini')
#cfg.sections()
root_path        = os.getcwd() + "/../../../"
pkt_path         = root_path + cfg.get('path','pkt_path')
chip_name       = cfg.get('product','chip')
product_name = cfg.get('path','product_name')
#product_ver     = cfg.get('product','product_ver')
product_type     = cfg.get('product','product_type')
fake_ver           = cfg.get('mode','fake_ver')
boot_ver           = cfg.get('mode','boot_ver')
target_type      = cfg.get('mode','target_type')
app_list           = cfg.get('app','app_list')
#配置文件地址#
bin_cfg_file    = os.getcwd() + "/packet_cfg/src_bin.cfg"
boot_bin_path = root_path + cfg.get('path','boot_bin_path')
cco_bin_path = root_path + cfg.get('path','cco_bin_path')
sta_bin_path= root_path + cfg.get('path','sta_bin_path')
ndm_bin_path    = root_path + cfg.get('path','ndm_bin_path')
cco_nv_path = root_path + cfg.get('path','cco_nv_path')
sta_nv_path = root_path + cfg.get('path','sta_nv_path')
ndm_nv_path = root_path + cfg.get('path','ndm_nv_path')
fake_ver_path   = root_path + cfg.get('path','fake_ver_path')
hupg_xml_file   = os.getcwd() + "/packet_cfg/3911_hupg.xml"
hupg_ini_path   = os.getcwd() + "/packet_cfg"

#源文件地址#
pkt_src_path  = pkt_path + "/src/"
bin_src_path  = pkt_src_path + "/Xcore_bin/"
#目标文件地址#
pkt_pub_path     = pkt_path + "/pub/"
zip_pub_path      = pkt_pub_path + "/_z/" 
zpk_pub_path     = pkt_pub_path + "/_zpk/" 
upg_pub_path    = pkt_path + "/image/" 
#编译工具及log文件地址#
tools_dir = root_path + "/tools/upg/"
tools_perl_dir = root_path + "/tools/perl/bin"
log_file = pkt_path + "/pub/Zpklog.txt"
upg_log_file = pkt_path + "/pub/upglog.txt"
image_log_file = pkt_path + "/pub/imagelog.txt"
cco_nv_file_name_list=[]
sta_nv_file_name_list=[]
ndm_nv_file_name_list=[]
#=================================================#


def cfg_file_2_cmd_param(cfg_file,bAddAttriName):
    param_str=''
    if cfg_file:
        cfg = ConfigParser()
        cfg.read(os.path.join(hupg_ini_path,cfg_file))
        #print('--------------------',cfg_file)
        
        cfg_val=cfg.get('main_cfg','magic')
        if(cfg_val):
            if(bAddAttriName):
                param_str=param_str+' -'+'magic'
            param_str=param_str+' '+cfg_val
        
        cfg_val=cfg.get('main_cfg','manucode')
        if(cfg_val):
            if(bAddAttriName):
                param_str=param_str+' -'+'manucode'
            param_str=param_str+' '+cfg_val    
            
        return param_str
    else:
        return param_str
        
def make_nv_file_name_list():
    global cco_nv_file_name_list
    global sta_nv_file_name_list
    global ndm_nv_file_name_list
    cfg = ConfigParser()
    cfg.read(os.path.join(hupg_ini_path,'nv_file_cfg.ini'))
    cco_files=cfg.get('file_name','cco_files')
    sta_files=cfg.get('file_name','sta_files')
    ndm_files=cfg.get('file_name','ndm_files')
    cco_nv_file_name_list=[c.strip() for c in cco_files.split(';')]  
    sta_nv_file_name_list=[c.strip() for c in sta_files.split(';')]  
    ndm_nv_file_name_list=[c.strip() for c in ndm_files.split(';')]  
    #print("cco_nv_file_name_list",cco_nv_file_name_list)
    #print("sta_nv_file_name_list",sta_nv_file_name_list)
    #print("ndm_nv_file_name_list",ndm_nv_file_name_list)
    
class ZipBuild:
    def __init__(self):
        if not os.path.exists(pkt_path):
            os.mkdir(pkt_path)
        if not os.path.exists(pkt_src_path):
            os.mkdir(pkt_src_path)
        if not os.path.exists(bin_src_path):
            os.mkdir(bin_src_path)
        if not os.path.exists(pkt_pub_path):
            os.mkdir(pkt_pub_path)
        if not os.path.exists(zip_pub_path):
            os.mkdir(zip_pub_path)

        self.__rootPath         = root_path
        self.__binCfgFile       = bin_cfg_file
        self.__binSrcDir        = bin_src_path
        self.__zipPubDir        = zip_pub_path
        self.__toolsDir            = tools_dir
        self.__bootBinPath    	= boot_bin_path
        self.__ccoBinPath    	= cco_bin_path
        self.__staBinPath     	= sta_bin_path
        self.__ndmBinPath   	= ndm_bin_path
        self.cco_nv_path        = cco_nv_path
        self.sta_nv_path        = sta_nv_path    
        self.ndm_nv_path        = ndm_nv_path
        self.__fakeVerPath    	= fake_ver_path
        self.__logFile              = open(log_file, "wb")

        self.__SetEnv()
        self.__GetSrcBin()
        
    def work(self):
        #压缩文件生成#  
        self.__compressCmdList = self.__CompressBinFile()
        for x in self.__compressCmdList:
            outPut = os.popen(x)
            self.__logFile.write(bytes(outPut.read(), 'UTF-8'))
            outPut.close
            #print (x) 

    def __SetEnv(self):
        os.environ["PATH"] = self.__toolsDir + ";" + os.environ["PATH"]
        
    # 复制bin文件 #
    def __GetSrcBin(self):
        for name in os.listdir(self.__binSrcDir):
            os.remove(os.path.join(self.__binSrcDir.encode('utf-8'), name.encode('utf-8')))

        if os.path.exists(self.__bootBinPath):
            bootBinFileNames = os.listdir(self.__bootBinPath)
            for a in bootBinFileNames:
                if '.bin' in a:
                    shutil.copy(os.path.join(self.__bootBinPath.encode('utf-8'), a.encode('utf-8')),self.__binSrcDir.encode('utf-8'))

        if os.path.exists(self.__ccoBinPath):
            ccoBinFileNames = os.listdir(self.__ccoBinPath)
            for b in ccoBinFileNames:
                if '.bin' in b:
                    shutil.copy(os.path.join(self.__ccoBinPath.encode('utf-8'), b.encode('utf-8')),self.__binSrcDir.encode('utf-8'))
 
        if os.path.exists(self.__staBinPath):
            staBinFileNames = os.listdir(self.__staBinPath)
            for c in staBinFileNames:
                if '.bin' in c:
                    shutil.copy(os.path.join(self.__staBinPath.encode('utf-8'), c.encode('utf-8')),self.__binSrcDir.encode('utf-8'))

        if os.path.exists(self.__ndmBinPath):
            ndmBinFileNames = os.listdir(self.__ndmBinPath)
            for d in ndmBinFileNames:
                if '.bin' in d:
                    shutil.copy(os.path.join(self.__ndmBinPath.encode('utf-8'), d.encode('utf-8')),self.__binSrcDir.encode('utf-8'))


        if os.path.exists(self.cco_nv_path):
            nvHnvFileNames = os.listdir(self.cco_nv_path)
            for e in nvHnvFileNames:
                if '.hnv' in e:
                    shutil.copy(os.path.join(self.cco_nv_path.encode('utf-8'), e.encode('utf-8')),self.__binSrcDir.encode('utf-8'))
                    
        if os.path.exists(self.sta_nv_path):
            nvHnvFileNames = os.listdir(self.sta_nv_path)
            for e in nvHnvFileNames:
                if '.hnv' in e:
                    shutil.copy(os.path.join(self.sta_nv_path.encode('utf-8'), e.encode('utf-8')),self.__binSrcDir.encode('utf-8'))
                    
        if os.path.exists(self.ndm_nv_path):
            nvHnvFileNames = os.listdir(self.ndm_nv_path)
            for e in nvHnvFileNames:
                if '.hnv' in e:
                    shutil.copy(os.path.join(self.ndm_nv_path.encode('utf-8'), e.encode('utf-8')),self.__binSrcDir.encode('utf-8'))   
                    
        if os.path.exists(self.__fakeVerPath):
            fakeVerFileNames = os.listdir(self.__fakeVerPath)
            for f in fakeVerFileNames:  
                if '.upgpk' in f and ('.cfg' not in f):
                    shutil.copy(os.path.join(self.__fakeVerPath.encode('utf-8'), f.encode('utf-8')),self.__binSrcDir.encode('utf-8'))

    def __CompressBinFile(self):
        compressCmdList           =  []
        compress_tool                =  "lzma.exe -a2 -d23 -lc2 -lp2 e   "
        binFileNames = os.listdir(self.__binSrcDir)
        for x in binFileNames:
            compressCmdList.append((compress_tool.encode('utf-8') + \
                                                    os.path.join(self.__binSrcDir.encode('utf-8') + \
                                                    x.encode('utf-8'))+ ' '.encode('utf-8')  +\
                                                    os.path.join(self.__zipPubDir.encode('utf-8'), x.encode('utf-8')) + \
                                                   '.z'.encode('utf-8')).decode())
        return compressCmdList

class ZpkBuild:
    #global cco_nv_file_name_list
    #global sta_nv_file_name_list
    #global ndm_nv_file_name_list
    make_nv_file_name_list()
    def __init__(self):
        if not os.path.exists(zpk_pub_path):
            os.mkdir(zpk_pub_path)

        self.__zipPubDir     = zip_pub_path
        self.__zpkPubDir     = zpk_pub_path
        self.__upgPubDir     = upg_pub_path
        self.__chipName      = chip_name
        self.__prodVer     = product_ver   
        self.__bootVer     = boot_ver   
        self.__fakeVer     =  fake_ver     
        self.__toolsDir       = tools_dir
        self.__logFile         = open(log_file, "wb")     

        self.__app_list     = app_list
        self.__SetEnv()

    def work(self):
        self.__zpkCmdList = self.   __ZpkFile()
        for x in self.__zpkCmdList:            
            outPut = os.popen(x)
            self.__logFile.write(bytes(outPut.read(), 'UTF-8')) 
            outPut.close
            #print (x)   

    def __SetEnv(self):
        os.environ["PATH"] = self.__toolsDir + ";" + os.environ["PATH"]

    def __ZpkFile(self):
        zpkCmdList           =  []
        zpk_tool                =  "packetfile.exe "
        zipFileNames        = os.listdir(self.__zipPubDir)
        app_list = self.__app_list.upper()
        for x in zipFileNames:
            if 'bootloader.bin' in x:
                zpk_type = ' code '
                print(app_list)
                if 'rmt' in app_list.lower():
                    zpkCmdList.append((zpk_tool.encode('utf-8')  + \
                                ' -e '.encode('utf-8')  +\
                                os.path.join(self.__zipPubDir.encode('utf-8') , x.encode('utf-8'))+ \
                                ' -o '.encode('utf-8')  +\
                                os.path.join(self.__zpkPubDir.encode('utf-8'), x.replace('.z', '.zpk').encode('utf-8'))  +\
                                ' -type '.encode('utf-8')  +\
                                zpk_type.encode('utf-8') +\
                                ' -chip '.encode('utf-8') +\
                                self.__chipName.encode('utf-8') +\
                                ' -ver '.encode('utf-8') +\
                                self.__bootVer.encode('utf-8') +\
                                ' -machine 1 -prot 4098 -plat 4 -prod 254 '.encode('utf-8')).decode())
                else:
                    zpkCmdList.append((zpk_tool.encode('utf-8')  + \
                                ' -e '.encode('utf-8')  +\
                                os.path.join(self.__zipPubDir.encode('utf-8') , x.encode('utf-8'))+ \
                                ' -o '.encode('utf-8')  +\
                                os.path.join(self.__zpkPubDir.encode('utf-8'), x.replace('.z', '.zpk').encode('utf-8'))  +\
                                ' -type '.encode('utf-8')  +\
                                zpk_type.encode('utf-8') +\
                                ' -chip '.encode('utf-8') +\
                                self.__chipName.encode('utf-8') +\
                                ' -ver '.encode('utf-8') +\
                                self.__bootVer.encode('utf-8') +\
                                ' -machine 1 -prot 4097 -plat 4 -prod 254 '.encode('utf-8')).decode())
            elif '.bin' in x:
                if 'hi3911_sta_mrs.bin' or 'hi3911_sta_csgmrs.bin' in x.lower():
                    plat_val='4'
                else:
                    plat_val='3'
                temp_str=' -machine 1 -prot 500 -plat '+plat_val+' -prod 254 '
                zpk_type = ' code '
                zpkCmdList.append((zpk_tool.encode('utf-8')  + \
                                ' -e '.encode('utf-8')  +\
                                os.path.join(self.__zipPubDir.encode('utf-8') , x.encode('utf-8'))+ \
                                ' -o '.encode('utf-8')  +\
                                os.path.join(self.__zpkPubDir.encode('utf-8'), x.replace('.z', '.zpk').encode('utf-8'))  +\
                                ' -type '.encode('utf-8')  +\
                                zpk_type.encode('utf-8') +\
                                ' -chip '.encode('utf-8') +\
                                self.__chipName.encode('utf-8') +\
                                ' -ver '.encode('utf-8') +\
                                self.__prodVer.encode('utf-8') +\
                                temp_str.encode('utf-8')).decode())
            if '.hnv' in x:
                #print("haiwen_list cco_nv_file_name_list",cco_nv_file_name_list)
                #print("haiwen_list sta_nv_file_name_list",sta_nv_file_name_list)
                #print("haiwen_list ndm_nv_file_name_list",ndm_nv_file_name_list)
                temp_x=x[0:len(x)-len('.xml.hnv.z')]
                if temp_x in sta_nv_file_name_list:
                    plat_val='4'
                elif temp_x in cco_nv_file_name_list or temp_x in ndm_nv_file_name_list:
                    plat_val='3'
                else:
                    print(temp_x,' not in nv_file_cfg.ini')
                    exit(-1)
                    
                #print("hw_nv_nameeeeeeeeeeeeeeeeeeeee x is=",temp_x,plat_val)
                zpk_type = ' nv '
                prod_val  = '0'
                str_tmp   = re.search(r'_\d+_\d+.+.xml.hnv', x)
                #print(str_tmp)
                res = re.findall('(\d+)', str_tmp.group())
                if len(res) > 2:
                    prot_val = res[1] + '.' + res[2]
                else:
                    prot_val = res[1] 
                zpkCmdList.append((zpk_tool.encode('utf-8')  + \
                                ' -e '.encode('utf-8')  +\
                                os.path.join(self.__zipPubDir.encode('utf-8') , x.encode('utf-8'))+ \
                                ' -o '.encode('utf-8')  +\
                                os.path.join(self.__zpkPubDir.encode('utf-8'), x.replace('.z', '.zpk').encode('utf-8'))  +\
                                ' -type '.encode('utf-8')  +\
                                zpk_type.encode('utf-8') +\
                                ' -chip '.encode('utf-8') +\
                                self.__chipName.encode('utf-8') +\
                                ' -ver '.encode('utf-8') +\
                                self.__prodVer.encode('utf-8') +\
                                ' -machine 1 -prot '.encode('utf-8') +\
                                prot_val.encode('utf-8') +\
                                (' -plat '+plat_val+' -prod ').encode('utf-8') +\
                                prod_val.encode('utf-8')).decode())     
            if '.upgpk' in x:
                zpk_type = ' code '
                str_tmp = re.search(r'_\d+.\d+.upgpk', x)
                res = re.findall('(\d+)', str_tmp.group())
                prot_val  = res[0]
                prod_val = res[1]
                zpkCmdList.append((zpk_tool.encode('utf-8')  + \
                                ' -e '.encode('utf-8')  +\
                                os.path.join(self.__zipPubDir.encode('utf-8') , x.encode('utf-8'))+ \
                                ' -o '.encode('utf-8')  +\
                                os.path.join(self.__zpkPubDir.encode('utf-8'), x.replace('.z', '.zpk').encode('utf-8'))  +\
                                ' -type '.encode('utf-8')  +\
                                zpk_type.encode('utf-8') +\
                                ' -chip '.encode('utf-8') +\
                                self.__chipName.encode('utf-8') +\
                                ' -ver '.encode('utf-8') +\
                                self.__prodVer.encode('utf-8') +\
                                ' -machine 1 -prot '.encode('utf-8') +\
                                prot_val.encode('utf-8') +\
                                ' -plat 3 -prod '.encode('utf-8') +\
                                prod_val.encode('utf-8')).decode())
        return zpkCmdList

class UpgBuild:
    def __init__(self):
        if not os.path.exists(upg_pub_path):
            os.mkdir(upg_pub_path)

        self.__binSrcDir     = bin_src_path
        self.__zpkPubDir     = zpk_pub_path
        self.__upgPubDir     = upg_pub_path
        self.__chipName      = chip_name
        self.__prodVer     = product_ver
        self.__app_list     = app_list
        self.__fakeVer     =  fake_ver
        self.__target_type = target_type   
        self.__product_type = product_type  
        self.__toolsDir       = tools_dir
        self.__toolsPerlDir = tools_perl_dir
        self.__upgLogFile = upg_log_file
        self.__imageLogFile =  open(image_log_file, "wb")   

        self.__product_name = product_name
        self.__logFile         = open(log_file, "wb")     

        self.__SetEnv()

    def work(self):
        self.__upgCmdList = self.__UpgFile()
        for x in self.__upgCmdList:       
            outPut = os.popen(x)
            self.__logFile.write(bytes(outPut.read(), 'UTF-8')) 
            outPut.close
            #print (x) 

    def __SetEnv(self):
        os.environ["PATH"] = self.__toolsDir + ";" + self.__toolsPerlDir+ ";" + os.environ["PATH"]

    def __UpgFile(self):
        upgCmdList           =  []
        product_name       =   self.__product_name.upper()
        product_ver        = self.__prodVer
        app_name             =   self.__app_list.upper()
        target_type            =   self.__target_type
        upg_tool                =  ' mkupg.exe '
        bupg_tool              =  ' perl.exe '
        hbin_tool              =  ' catbin.exe '
        ver_keyword            =  '@VERSION'

        # 读取xml配置 #
        print('read from xml ...')
        tree = ET.parse(hupg_xml_file) 
        list_product = tree.findall("PROJECT")
        for apps in list_product:
            # 选择工程名 #
            if (apps.attrib['name']  == product_name):
                print('PROJECT name : ' +apps.attrib['name'])
                for app in apps:
                    # 选择APP名 #
                    if (','+app.attrib['NAME'] + ',') in app_name :
                        print('APP name : ' + app.attrib['NAME'])
                        for prod_type in app:
                            if 'CFG_FILE' in prod_type.attrib.keys():
                                hupg_cfg_file_name=prod_type.attrib['CFG_FILE'].strip()
                            else:
                                hupg_cfg_file_name=None
                             
                            
                             
                            #print('build' + prod_type.attrib['NAME']+ 'image')
                            cfg_strs = prod_type.attrib['SRC_NAME'].replace(ver_keyword, product_ver.lower()).split(',')
                            prod_name = prod_type.attrib['ID']

                            # 构造secfile段，可能有1-3个段 #
                            upg_secfile_str          =  ' '
                            bupg_secfile_str          =  ' '
                            hbin_secfile_str          =  ' '
                            hnv_file             =  ' '

                            # 找出所有hupg配置 #
                            if prod_type.attrib['TYPE'] == 'hupg':
                                for cfg_str in cfg_strs:
                                    upg_secfile_str = upg_secfile_str + ' -secfile '+ self.__zpkPubDir  + cfg_str
                                    # 得到hnv文件 #
                                    if '.hnv.zpk' in cfg_str:
                                        hnv_file = cfg_str[:-4]
                                #print(upg_secfile_str)

                                # 构造输出文件名 #
                                outPutFile = product_name.lower() + '_' + product_ver.upper() + '_' + prod_name + '.' + prod_type.attrib['TYPE']
                
                                #print(prod_type.attrib['DEV'].lower().split(','))
                                #print(self.__product_type.split('_'))
                                # 判断产品类型 #
                                build_flay = 0
                                for X in prod_type.attrib['DEV'].lower().split('_'):
                                    for Y in self.__product_type.split(','):
                                        if (X == Y):
                                            build_flay =1
                                if build_flay:
                                     # 构造hupg命令 #
                                    print('build - ' + prod_type.attrib['NAME'] + ' ' + prod_type.attrib['TYPE'])
                                    upgCmdList.append((upg_tool.encode('utf-8')  + \
                                                                upg_secfile_str.encode('utf-8')+\
                                                                ' -o '.encode('utf-8')  +\
                                                                os.path.join(self.__upgPubDir.encode('utf-8'), outPutFile.encode('utf-8'))  +\
                                                                ' -hnv '.encode('utf-8')  +\
                                                                os.path.join(self.__binSrcDir.encode('utf-8'),  hnv_file.encode('utf-8'))+\
                                                                ' -log '.encode('utf-8') +\
                                                                self.__upgLogFile.encode('utf-8')  + \
                                                                ' -verfmt 0x01 -fcode -fsize 0x68000 -crc_fmt 1'.encode('utf-8')).decode()+\
                                                                cfg_file_2_cmd_param(hupg_cfg_file_name,True))
                                    self.__imageLogFile.write((outPutFile+'\r\n').encode('utf-8'))
                                    #print('haiwennnnnnnnnnnnnnnnnnnnn',upgCmdList)
                            # 找出所有bupg配置 #
                            elif prod_type.attrib['TYPE'] == 'bupg': 
                                # 构造secfile段 #
                                for cfg_str in cfg_strs:
                                    bupg_secfile_str = bupg_secfile_str + ' '+ self.__upgPubDir  + cfg_str
                                #print(bupg_secfile_str)

                                # 构造输出文件名，bupg转换为hupg #
                                bupg_file_name = 'hupg'
                                outPutFile = product_name.lower() + '_' + product_ver.upper() + '_' + prod_name + '.' + bupg_file_name
                                #print(outPutFile)

                                # 判断产品类型 # 
                                build_flay = 0
                                for X in prod_type.attrib['DEV'].lower().split('_'):
                                    if X not in self.__product_type.split(','):
                                        build_flay = 0
                                        break
                                    build_flay = 1

                                if build_flay:
                                    # 构造bupg命令 #
                                    print('build - ' + prod_type.attrib['NAME'] + ' ' + bupg_file_name)                      
                                    upgCmdList.append((bupg_tool.encode('utf-8')  + \
                                                             '   '.encode('utf-8')  +\
                                                            os.path.join(self.__toolsDir.encode('utf-8') , 'catfilebin.pl'.encode('utf-8'))+ \
                                                             '   '.encode('utf-8')  +\
                                                            bupg_secfile_str.encode('utf-8') +\
                                                            '   '.encode('utf-8')  +\
                                                            os.path.join(self.__upgPubDir.encode('utf-8') , outPutFile.encode('utf-8'))).decode()+\
                                                            cfg_file_2_cmd_param(hupg_cfg_file_name,False)) 
                                                            
                                    self.__imageLogFile.write((outPutFile+'\r\n').encode('utf-8'))
                                    #print('haiwennnnnnnnnnnnnnnnnnnnn bupg',upgCmdList)
                            # 找出所有hbin配置 #
                            elif prod_type.attrib['TYPE'] == 'hbin': 
                                hbin_secfile_str = ' -file ' + self.__binSrcDir + cfg_strs[0] + ',' + cfg_strs[1]  + ' -file '  +  self.__upgPubDir + cfg_strs[2] + ',' + cfg_strs[3] + ' -size ' + cfg_strs[4]  + ' -pading 0xFF '
                                # 构造输出文件名 #
                                outPutFile = product_name.lower() + '_' + product_ver.upper() + '_' + prod_name + '.' + prod_type.attrib['TYPE']
                                # 判断产品类型 #    
                                build_flay = 0
                                for X in prod_type.attrib['DEV'].lower().split('_'):
                                    for Y in self.__product_type.split(','):
                                        if (X == Y):
                                            build_flay =1
                                if build_flay:
                                    # 构造hbin命令 #
                                    print('build - ' + prod_type.attrib['NAME'] + ' ' + prod_type.attrib['TYPE'])
                                    upgCmdList.append((hbin_tool.encode('utf-8')  + \
                                                            hbin_secfile_str.encode('utf-8')  +\
                                                            ' -o '.encode('utf-8')  + \
                                                            os.path.join(self.__upgPubDir.encode('utf-8') , outPutFile.encode('utf-8'))).decode()) 
                                    self.__imageLogFile.write((outPutFile+'\r\n').encode('utf-8'))
                                    #print('hbin bupg',upgCmdList)
        return upgCmdList

# main function #
if __name__ == '__main__':
    t1 = time.time()
    fc = ZipBuild()
    fc.work()
    print ("files compress ok!! \r\n")
    fp = ZpkBuild()
    fp.work()
    fu = UpgBuild()
    fu.work()
    #fc0 = NvSdkBuild()
    #fc0.work()

    print('TOTAL TIME:%ss'%str(time.time() - t1))
