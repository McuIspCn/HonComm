/**********************
CopyRight www.mcuisp.com,2008~2020
App运行于EP968中，实现串口、SWD、JTAG、SWIM、ICSP等各种接口的ISP下载。
串口下载功能也可运行于HonComm串口下载平台中
--运行于EP968中的App被分成多个6KB大小的页
--AppMain--AppPage0--...--AppPage9
--页之间函数调用及返回，将产生时间为几百毫秒的页切换
--页之间可共享sram中的变量，但不允许共享flash数据(页切换后其他页数据不可访问)
**********************/
#include "HonCommTypeDef_FuncDef.h"
#include "HonSubs.h"

#ifdef __ICCARM__
#define prama_location_HonCommUartIspAppInfo _Pragma("location=\"HonCommUartIspAppInfo\"") __root
#endif

#ifdef __CC_ARM
#define prama_location_HonCommUartIspAppInfo _Pragma("arm section rodata=\"HonCommUartIspAppInfo\"") __attribute__((used))
#endif

#ifndef prama_location_HonCommUartIspAppInfo
#define prama_location_HonCommUartIspAppInfo
#endif

prama_location_HonCommUartIspAppInfo
const char IspAppSign[24]="HonCommIspAppSig V0";//前面16个字符不能修改
prama_location_HonCommUartIspAppInfo
const unsigned long HonCommDwFlag=0x55aa6699;
prama_location_HonCommUartIspAppInfo
const unsigned long HonCommReserverSpace=2;//1KB~16MB

prama_location_HonCommUartIspAppInfo
const char McuChipType[32]="STM32F103XXXX";

prama_location_HonCommUartIspAppInfo
const char McuVendor[32]="STMicroelectronics";

prama_location_HonCommUartIspAppInfo
const char AppAuthor[32]="Sample Project";

//--

#include "HonCommHalSample.c"
void ResetChip_DtrHighRst_RtsHighIsp(u32 ms)
{
  HonComm_SetDtrRtsBreak((1<<2)|(1<<9));//SetDtr ClrRts
  HonComm_DelayAtLeastUs(ms*1000);
  HonComm_SetDtrRtsBreak((1<<10));//ClrDtr
  return;
}
int stm32ftestACmd(void)
{//测试基本的串口收发 Use Uart Func to Get Version
#define ISP_ACK (0x79)
#define ISP_NAK (0x1f)
  signed long result;
  unsigned char bootloadversion,optionbyte1,optionbyte2;
  HonComm_SendAByte(0x01);
  HonComm_SendAByte(0xfe);
  result=HonComm_ReadUartWithTimeOut(100);
  if(result<0)return -1;
  if(result==ISP_NAK)return 1;
  if(result!=ISP_ACK)return -2;
  result=HonComm_ReadUartWithTimeOut(100);
  if(result<0)return -1;
  bootloadversion=result;
  result=HonComm_ReadUartWithTimeOut(100);
  if(result<0)return -1;
  optionbyte1=result;
  result=HonComm_ReadUartWithTimeOut(100);
  if(result<0)return -1;
  optionbyte2=result;
  result=HonComm_ReadUartWithTimeOut(100);
  if(result<0)return -1;
  if(result==ISP_NAK)return 1;
  if(result!=ISP_ACK)return -2;
  //Send 3 bytes to Scr;
  char str[16];
  str[0]=toHex(bootloadversion>>4);
  str[1]=toHex(bootloadversion>>0);
  str[2]=toHex(optionbyte1>>4);
  str[3]=toHex(optionbyte1>>0);
  str[4]=toHex(optionbyte2>>4);
  str[5]=toHex(optionbyte2>>0);
  str[6]=0;
  HonUi_DisplayStr("Use Uart Func to Get Version:\r\n",0,64,0);
  HonUi_DisplayStr(str,0,64+32,0);
  return 0;
}
#include <string.h>
int stm32f103xxxxIsp(u32 progcount)
{
  union{
    unsigned char byte_buf256[256];
    unsigned short u16_buf128[128];
    unsigned long u32_buf64[64];
  }tb;
  u64 SN=0;
  if(LoadSerialNo(&SN)<0)return -1001;
  u64 snbak;
  TDataMaskInfo snmask8;
  snmask8.address=0x08000100;
  snmask8.datalen=8;
  snmask8.data=(u8*)(&SN);
  snmask8.databak=(u8*)(&snbak);
  
  char str[32];
  HonUi_ClrRect(0,0,128);
  Hon_SetLeds(0x0000,80);
  s32 bps=HonComm_GetSetBPS(0);
  bps=230400;
  HonComm_GetSetComSettings(SetChFMT_DataBit8+SetChFMT_ONESTOPBIT+SetChFMT_EVENPARITY);
  HonComm_GetSetBPS(bps);
  ResetChip_DtrHighRst_RtsHighIsp(100);
  HonUi_DisplayStr("开始连接芯片... bps:",0,0,0);
  DisplayInt32A(bps,48,16,0);
  HonUi_DisplayStr("\r\n",0,32,0);
  if(STM32AN3155_ConnectChip(5000)<0)return -1;
  HonUi_Beep(100,6);
  HonUi_ClrRect(0,0,128);
  
  HonUi_DisplayStr("STM32AN3155_ConnectChip Ok\r\n",0,0,0);
  u32 StartTime;
  StartTime=HonComm_GetMsTicks();
  if(0)
  {
    HonUi_DisplayStr("stm32ftestACmd\r\n",0,32,0);
    if(stm32ftestACmd()<0)return -5;
  }
  if(STM32AN3155_Get(tb.byte_buf256)<0)return -2;
  unsigned char EraseCmd=tb.byte_buf256[8];//43 or 44
  if(STM32AN3155_GetVersion_RdpStatus(tb.byte_buf256)<0)return -3;
  if(STM32AN3155_GetChipProductID(tb.byte_buf256)<0)return -4;
  unsigned long address=0x08000000,size=8;
  signed long result;
  
  if(0)
  {//擦除芯片,缺省是60秒超时，如果需要在10~200秒之间调节
    u32 Second=100;
    u32 pagecount=0xffff;
    STM32AN3155_ReadOutUnProtect(Second<<20);
    STM32AN3155_Erase43( (Second<<20)+pagecount,tb.byte_buf256);
    STM32AN3155_Erase44( (Second<<20)+pagecount,tb.u16_buf128);
  }
  if(0)
  {
    HonUi_DisplayStr("Always UnLock and Erase Chip\r\n",0,64+32,0);
    if(STM32AN3155_ReadOutProtect()<0)return -33;
    if(STM32AN3155_ReadOutUnProtect(0)<0)return -34;
    if(STM32AN3155_ConnectChip(1000)<0)return -35;
  }
  else
  {
    result=STM32AN3155_ReadMemory(address,tb.byte_buf256,size);
    if(result<0)
    {
      if(result!=-1)return -5;
      //STM32AN3155_ReadMemory==-1,NACKed,Erase Chip
      HonUi_DisplayStr("UnLock and Erase Chip\r\n",0,64+32,0);
      if(STM32AN3155_ReadOutUnProtect(0)<0)return -31;
      if(STM32AN3155_ConnectChip(1000)<0)return -32;
    }
    else
    {//Erase Chip
      if(EraseCmd==0x43)
      {
        HonUi_DisplayStr("Full Erase Chip43\r\n",0,64+32,0);
        if(STM32AN3155_Erase43(0xffff,tb.byte_buf256)<0)return -11;
      }
      else if(EraseCmd==0x44)
      {
        HonUi_DisplayStr("Full Erase Chip44\r\n",0,64+32,0);
        if(STM32AN3155_Erase44(0xffff,tb.u16_buf128)<0)return -12;
      }
      else
      {
        HonUi_DisplayStr1S("Full Erase Chip Cmd Eror\r\n",0,64+32,0);
        return -1;
      }
      if(STM32AN3155_WriteUnProtect()<0)return -13;
      if(STM32AN3155_ConnectChip(1000)<0)return -14;
    }
  }
  HonUi_ClrRect(0,0,128);
  HonUi_DisplayStr("Erase Chip Ok\r\n",0,0,0);
  result=STM32AN3155_ReadMemory(0x1ffff7e8,tb.byte_buf256,12);
  if(result<0)
  {
    HonUi_DisplayStr1S("读芯片唯一序列号失败 Read ChipUID Fail...\r\n",0,32,0);
    return -15;
  }
  if(1)
  {
    HonUi_DisplayStr("芯片唯一序列号 ChipUID...\r\n",0,32,0);
    DisplayMemoryA(tb.byte_buf256,0,64,12);
    HonUi_DisplayStr("\r\n",0,64+32,2);
  }
  HonUi_ClrRect(0,0,128);
  result=HonFile_SwitchSegment(0,tb.byte_buf256,sizeof(TSegmentInfomation));
  //buf里返回16字节segment name,unsigned long startaddress,unsigned long datasize,
  if(1)
  {//显示自动增量信息
    HonUi_DisplayStr("自动增量:",0,64+32,0);
    DisplayHex32A(SN>>32,0,64+48,8);
    DisplayHex32A(SN>>0,64,64+48,8);
    HonUi_DisplayStr("\r\n",0,128,0);
  }
  HonUi_DisplayStr("Write Flash..\r\n",0,0,0);
  if(result>=0)
  {
    TSegmentInfomation *seginfo=(TSegmentInfomation*)tb.byte_buf256;
    unsigned long startaddress=(*seginfo).startaddress;
    unsigned long datasize=(*seginfo).loadedsize;
    unsigned long steplen;
    HonUi_DisplayProgress(0,datasize,48);
    for(unsigned long i=0;i<datasize;i+=steplen)
    {
      steplen=datasize-i;
      if(steplen>256)steplen=256;
      if(HonFile_ReadSegmentData(startaddress+i,tb.byte_buf256,steplen)<0)
      {
        HonUi_DisplayStr1S("ReadSegmentData Error\r\n",0,64,0);
        return -61;
      }
      WriteDataMask(&snmask8,startaddress+i,tb.byte_buf256,steplen);
      if(STM32AN3155_WriteMemory(startaddress+i,tb.byte_buf256,steplen)<0)
      {
        HonUi_DisplayStr1S("Write Flash Error\r\n",0,0,0);
        return -62;
      }
      if((i%1024)==0)
      {
        HonUi_DelStr();
        IntToStr(i/1024,(u8*)str);
        strcat(str,"KB Writed!!!\r\n");
        HonUi_DisplayStr(str,0,16,0);
        HonUi_DisplayProgress(i,datasize,48);
      }
    }
    HonUi_DisplayProgress(datasize,datasize,48);
    HonUi_DisplayStr("Write Flash Ok\r\n",0,0,0);
    HonUi_ClrRect(0,0,64);
    HonUi_DisplayStr("Start Verify\r\n",0,0,0);
    HonUi_DisplayProgress(0,datasize,48);
    for(unsigned long i=0;i<datasize;i+=steplen)
    {
      steplen=datasize-i;
      if(steplen>256)steplen=256;
      if(STM32AN3155_ReadMemory(startaddress+i,tb.byte_buf256,steplen)<0)return -63;
      if(VerifyDataMask(&snmask8,startaddress+i,tb.byte_buf256,steplen)<0)return -65;
      if(HonFile_CmpSegmentData(startaddress+i,tb.byte_buf256,steplen)<0)return -64;
      if((i%1024)==0)
      {
        HonUi_DelStr();
        IntToStr(i/1024,(u8*)str);
        strcat(str,"KB Verifyed!!!\r\n");
        HonUi_DisplayStr(str,0,16,0);
        HonUi_DisplayProgress(i,datasize,48);
      }
    }
    HonUi_DisplayProgress(datasize,datasize,48);
    HonUi_ClrRect(0,0,48);
    HonUi_DisplayStr("Verify Flash Ok\r\n",0,0,0);
  }
  else
  {
    HonUi_DisplayStr1S("Open Segment 0 Error...\r\n",0,0,0);
    return -101;
  }
  if(1)
  {
    //专门校验一次自动增量
    memset(tb.byte_buf256,0,32);
    if(STM32AN3155_ReadMemory(snmask8.address,tb.byte_buf256,snmask8.datalen)<0)return -8300;
    HonUi_ClrRect(0,0,128);
    DisplayMemoryA(snmask8.data,0,0,8);
    DisplayMemoryA(tb.byte_buf256,0,32,8);
    if(memcmp(snmask8.data,tb.byte_buf256,snmask8.datalen)==0)
    {
      HonUi_DisplayStr("\r\n自动增量正确\r\n",0,64,0);
      HonUi_GetKey(1000);
    }
    else
    {
      HonUi_DisplayStr("\r\n自动增量错误\r\n",0,64,0);
      HonUi_GetKey(0);
    }
  }
  result=HonFile_SwitchSegment(1,tb.byte_buf256,sizeof(TSegmentInfomation));
  //buf里返回16字节segment name,unsigned long startaddress,unsigned long datasize,
  if(result>=0)
  {
    TSegmentInfomation *seginfo=(TSegmentInfomation*)tb.byte_buf256;
    unsigned long startaddress=(*seginfo).startaddress;
    unsigned long datasize=(*seginfo).loadedsize;
    unsigned long steplen;
    for(unsigned long i=0;i<datasize;i+=steplen)
    {
      steplen=datasize-i;
      if(steplen>16)steplen=16;
      if(HonFile_ReadSegmentData(startaddress+i,tb.byte_buf256,steplen)<0)return -67;
      if(STM32AN3155_WriteMemory(startaddress+i,tb.byte_buf256,steplen)<0)return -68;
    }
    HonUi_DisplayStr("Write OptionBytes Ok...\r\n",0,0,0);
    if(0)//optionbytes cannot readout after RDP
    {
      for(unsigned long i=0;i<datasize;i+=steplen)
      {
        steplen=datasize-i;
        if(steplen>16)steplen=16;
        if(STM32AN3155_ReadMemory(startaddress+i,tb.byte_buf256,steplen)<0)return -69;
        if(HonFile_CmpSegmentData(startaddress+i,tb.byte_buf256,steplen)<0)return -70;
      }
      HonUi_DisplayStr("Verify OptionBytes Ok\r\n",0,0,0);
    }
  }
  else
  {
    HonUi_DisplayStr1S("Open Segment 1 Error...\r\n",0,0,0);
    return -102;
  }
  SN++;
  if(WriteSerialNo(&SN)<0)return -1002;
  if(LoadSerialNo(&SN)<0)return -1003;
  HonUi_ClrRect(0,0,128);
  HonUi_DisplayStr("新自动增量:",0,64+32,0);
  DisplayHex32A(SN>>32,0,64+48,8);
  DisplayHex32A(SN>>0,64,64+48,8);
  HonUi_DisplayStr("\r\n",0,128,0);
  Hon_SetLeds(0xff00,80);
  HonUi_Beep(100,8);
  if(progcount==1)
  {
    HonUi_DisplayStr("单次烧录成功\r\n",0,64,0);
    return 0;//单次烧录
  }
  StartTime=HonComm_GetMsTicks()-StartTime;
  IntToStr(StartTime,(u8*)str);
  strcat(str,"ms,All Ok\r\n");
  HonUi_DisplayStr(str,0,64,0);
  HonUi_DisplayStr("循环烧录模式\r\n",0,64+16,0);
  HonUi_DisplayStr("开始检测芯片的移除...\r\n",0,0,0);
  if(STM32AN3155_PollChip(500)<0)return 0;
  HonUi_DisplayStr("检测到芯片已移除\r\n",0,0,0);
  return 1;//继续烧录
}
int basesub0(void)
{
  HonUi_DisplayStr1S("现在在base page\r\n",0,0,0);
  return 0;
}
int main()
{
  volatile signed long x;
  x=GetHonCommVersion();
  HonUi_ClrScr();
#ifdef __ICCARM__
  HonUi_DisplayStrLenXY("STM32串口下载演示-IAR EWARM\r\n",0);
#endif
#ifdef __CC_ARM
  HonUi_DisplayStrLenXY("STM32串口下载演示-KeilMDK\r\n",0);
#endif
  HonUi_DisplayStr("开始串口下载\r\n",0,32,0);
  HonUi_DisplayStr("请按1键开始一次烧录，按0开始循环烧录\r\n",0,64,0);
  s32 progcount=HonUi_GetKey(0);
  Hon_OpenUartPort();
  Hon_SetPin_Low(J_6P5_Boot0);
  Hon_SetPin_Low(J_6P4_Reset);
  Hon_SetTckPullUpDown(PullDisable);
  Hon_SetVpp(3300);
  Hon_SetVcctCurrentLimit(250);
  Hon_SetVcctOut(3000);
  Hon_PowerUpVcct(5000,2900);
  Hon_SetLeds(0x5555,50);
  Hon_ATE_IO(0x5);
  //返回即结束下载
  while(1)
  {
    x=stm32f103xxxxIsp(progcount);
    if(x<0)
    {
      Hon_SetLeds(0x00ff,80);//亮红灯
      HonUi_ClrRect(0,0,128);
      HonUi_DisplayStr("STM32串口下载出错\r\n",0,0,0);
      u8 buf[16];
      IntToStr(-x,buf);
      HonUi_DisplayStr((char*)buf,0,64,10);
      HonUi_GetKey(0);
      break;
    }
    else if(x==0)
    {
      return 0;
    }
  }
  inlineasm("bkpt 0");
  return -1;
}
