****************************************************************************
* 文件说明
* 沈汉坤/00130424/2011-09-06
****************************************************************************

****************************************************************************
* 产品配置文件
* 包括 编译目录/编译公用宏定义(如产品形态/特性模块等)
****************************************************************************

产品版本配置文件简介如下:
****************************************************************************
version.txt : 版本配置
****************************************************************************

****************************************************************************
ASIC单板环境，Timer时钟25M
****************************************************************************


****************************************************************************
产品系列 定义说明（在代码中引用的宏定义）
****************************************************************************
CCO系列: PRODUCT_CFG_PRODUCT_TYPE_CCO
STA系列: PRODUCT_CFG_PRODUCT_TYPE_STA
NDM系列: PRODUCT_CFG_PRODUCT_TYPE_NDM
****************************************************************************


****************************************************************************
产品版本配置说明
****************************************************************************
1. HiMDL.product.config : 发布的产品版本配置，包括所有源代码，支持HiMDL和HiARMTools项目
    该产品配置构建后生成的目标镜像文件和NV配置文件如下: 
    a) <产品名称>_<版本号>_firmware_cco.hbin      - 【CCO系列】烧写镜像文件 包括boot/OS/sal/drv/mac/app等二进制代码. 网口ip默认为192.168.1.1
                                                     用途: 用于i)boot下烧写或 ii)烧片
    b) <产品名称>_<版本号>_firmware_sta.hbin      - 【STA系列】烧写镜像文件 包括boot/OS/sal/drv/mac/app等二进制代码. 网口ip默认为192.168.1.101
                                                     用途: 用于i)boot下烧写或 ii)烧片
    c) <产品名称>_<版本号>_firmware_diag.hbin     - 【NDM系列】烧写镜像文件 包括boot/OS/sal/drv/mac/app等二进制代码. 网口ip默认为192.168.1.41
                                                     用途: 用于i)boot下烧写或 ii)烧片
    d) <产品名称>_<版本号>_firmware_cco.hupg      - 【CCO系列】升级镜像文件 包括OS/sal/drv/mac/app等二进制代码. 网口ip默认为192.168.1.1
                                                     用途: 用于i)boot下烧写代码（不包含NV文件）或 ii)通过HSO升级
    e) <产品名称>_<版本号>_firmware_sta.hupg      - 【STA系列】升级镜像文件 包括OS/sal/drv/mac/app等二进制代码. 网口ip默认为192.168.1.101
                                                     用途: 用于boot下烧写代码（不包含NV文件）或 通过HSO升级
    f) <产品名称>_<版本号>_firmware_diag.hupg     - 【NDM系列】升级镜像文件 包括OS/sal/drv/mac/app等二进制代码. 网口ip默认为192.168.1.41
                                                     用途: 用于boot下烧写代码（不包含NV文件）或 通过HSO升级
    g) <产品名称>_<版本号>_firmware_cco_sta.hupg  - 【CCO+STA系列】升级镜像文件
                                                     用途: 全网升级
    h) product_nv_001.hnv  - 【CCO系列】默认NV配置文件
                              用途: 用于i)boot下烧写NV文件或 ii)通过HSO加载NV文件  
    i) product_nv_101.hnv  - 【STA系列】默认NV配置文件
                              用途: 用于i)boot下烧写NV文件或 ii)通过HSO加载NV文件   
    j) product_nv_041.hnv  - 【NDM系列】默认NV配置文件
                              用途: 用于i)boot下烧写NV文件或 ii)通过HSO加载NV文件   
    k) product_nv_500.hnv  - 工厂出厂默认的NV配置文件
                              用途: 用于产线
    注意:
    1) 以上产品名称是去除v/r/0-9字符后的产品名称
    2) 产品名称 即为产品配置文件名前缀；版本号在OneBuilder的config中配置

2. PHY_ASIC.product.config    : PHY的产品版本配置，包括os/sal/drv/phy/phy st的源代码，支持HiMDL项目
    该产品配置构建后生成的目标镜像文件和NV配置文件如下: 
    发布包文件
    image\package\PHY_ASIC_HiMDL_产品版本号_构建类型.7z 例如 image\package\PHY_ASIC_HiMDL_v100r001c00b014_debug.7z 
    压缩包包含文件说明参见1.

3. PHY_MAC_ASIC.product.config    : PHY的产品版本配置，包括os/sal/drv/phy/phy st的源代码，支持HiMDL项目
    该产品配置构建后生成的目标镜像文件和NV配置文件如下: 
    发布包文件
    image\package\PHY_ASIC_HiMDL_产品版本号_构建类型.7z 例如 image\package\PHY_MAC_ASIC_HiMDL_v100r001c00b015_debug.7z
    压缩包包含文件说明参见1.
****************************************************************************    

****************************************************************************
* 镜像文件(烧写、升级）生成配置
****************************************************************************
目录: image_cfg
使用规则参见 image_cfg\readme.txt
****************************************************************************

****************************************************************************
注意:
****************************************************************************
1) 产品版本配置文件名和路径不能带有中文和空格;
2) 后缀固定为.product.config, 文件名其它部分表示产品名称，产品名称不能带有.号，规则如下
    最终生成的镜像文件前缀带开始部分为 去除v/r/0-9字符的产品名称 
3) 添加.product.config文件后，需要关闭OneBuilder重启生效。
4）修改.product.config文件，需要点击OneBuilder的Config->Reload Product Config菜单后生效。
****************************************************************************

****************************************************************************
* 新增APP步骤
****************************************************************************
1. 在app目录下新建目录比如demo
   拷贝 ipmrs或是eqt目录下的inc/app_res_cfg.h inc/app_res_cfg_inc.def到demo/inc目录下，并修改 app_res_cfg.h 配置
   拷贝 ipmrs\init\app_init.c 到目录 demo/init 目录下
   拷贝 ipmrs下的所有的makefile和配置文件到demo目录下, 修改 cfg_makefile_src_dir.def 和 lm_xxx配置文件(可选)
2. 拷贝 IPMRS.product.config 为 DEMO.product.config, 修改DEMO.product.config文件
3. 修改 image_cfg 目录下的 镜像文件配置
4. 新建 target\delivery\DEMO_Release目录
   拷贝 target\delivery\IPMRS_Release\sdk_lib下所有文件到target\delivery\DEMO_Release\lib下
   拷贝 target\delivery\IPMRS_Release\lib下所有NV文件到target\delivery\DEMO_Release\lib下
   拷贝 target\delivery\IPMRS_Release\acore目录到target\delivery\DEMO_Release\lib下
   拷贝 target\delivery\IPMRS_Release\bootrom目录到target\delivery\DEMO_Release\lib下
   拷贝 target\delivery\IPMRS_Release\mcore目录到target\delivery\DEMO_Release\lib下
   拷贝 target\delivery\IPMRS_Release\bootloader目录到target\delivery\DEMO_Release\lib下
5. 重启OneBuilder,在Product菜单下选择DEMO菜单项
****************************************************************************
