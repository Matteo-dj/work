该文件夹下为编译特殊版本时的配置。目前主要包含的类型如下：

1.1 bbit_SimulateMeter(虚拟表程序配置）
要求app_nvi_db.xml中@1位置值为1，其他配置与app\mrs\nv\app_nvi_db.xml中的配置保持一致。

<NV ID="0x412" NAME="虚拟电表" PARAM_NAME="MV_APP_SIMU_METER_INFO" PARAM_VALUE="{@1,0,0,60}" CATEGORY="APP" DEV="STA" BROADCAST="1" DESCRIPTION="" />

1.2 snr_csg(南网SNR台区识别版本)
具体修改的参数请参考《Hi3911V200 版本客制化 配置说明.doc》中的说明。