################################################################################
# 文件说明
# 沈汉坤/00130424/2011-09-06
################################################################################

################################################################################
#使用说明
1. 配置测试用例命令
   ctscfg <路径>       #设置配置路径
   ctscfg              #显示当前配置路径
2. 测试用例执行命令
   cts <测试用例配置文件名称> <用例序号>   #执行用例配置文件
   测试用例配置文件名称不带文件路径和扩展名
3. 测试用例函数名称长度最大为64字节，包括\0

# 注意：
1. 测试用例函数名称.pcfg 放到 app\cts\config 目录下,编译cts目录后才能通过HSO使用下发参数配置，参见2.
2. 代码中 测试用例函数名称 到 文件 app_cts_entry.h 中, 例如 测试函数为 cts_HI_MDM_NM_GetAddr 则添加如下行 
   {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_NM_GetAddr)},
   测试用例参数配置文件为 app\cts\config\cts_HI_MDM_NM_GetAddr.pcfg 可支持多个测试用例的参数配置，以//@<序号>标记用例参数，
   参数配置文件 cts_HI_MDM_NM_GetAddr.pcfg例子如下
        //
	//@0 用例的序号为0
	// 测试用例函数命令: cts_HI_MDM_NM_GetAddr 0
	// 测试用例函数名称: cts_HI_MDM_NM_GetAddr
	// 测试用例参数配置文件为 app\cts\config\cts_HI_MDM_NM_GetAddr.pcfg
	//
	HI_MDM_NM_GetAddr_PARAM0_S
	{
	{0xEE,0xFF,0xCC,0xBB,0xAA},
	{0x01,0x02,0x03}
	};

	//
	//@1 表示用例的序号为1
	// 测试用例函数命令: cts_HI_MDM_NM_GetAddr 1
	// 测试用例函数名称: cts_HI_MDM_NM_GetAddr
	// 测试用例参数配置文件为 app\cts\config\cts_HI_MDM_NM_GetAddr.pcfg
	//
	HI_MDM_NM_GetAddr_PARAM2_S
	{
	{0xEE,0xFF,0xCC,0xBB,0xAA},
	{0x01,0x02,0x03}
	};

3. 用例执行的典型例子:
HSO的console中，敲入
ctscfg E:\s00130424_v2\v1r2_new\app\cts\config 
cts cts_HI_MDM_NM_GetAddr 0
################################################################################
