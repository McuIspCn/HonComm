/**********************
CopyRight www.mcuisp.com,2008~2020
Loaderֻ��������λ����
��������װ��,�������ݽ���Ԥ�ȴ���
**********************/

#include "HonCommTypeDef_FuncDef.h"
#include "HonSubs.h"

#ifdef __ICCARM__
#define prama_location_LoaderInfoTable _Pragma("location=\"LoaderInfoTable\"") __root
#define prama_location_SegmentsInfoTable _Pragma("location=\"SegmentsInfoTable\"") __root
#endif

#ifdef __CC_ARM
#define prama_location_LoaderInfoTable _Pragma("arm section rodata=\"LoaderInfoTable\"") __attribute__((used))
#define prama_location_SegmentsInfoTable _Pragma("arm section rodata=\"SegmentsInfoTable\"") __attribute__((used))
#endif

#ifndef prama_location_LoaderInfoTable
#define prama_location_LoaderInfoTable
#endif
#ifndef prama_location_SegmentsInfoTable
#define prama_location_SegmentsInfoTable
#endif

prama_location_LoaderInfoTable
const char LoaderSign[32]="HonCommLoaderSig V0";//ǰ��16���ַ������޸�

prama_location_LoaderInfoTable
const char McuChipType[32]="STM32F103XXXX";

prama_location_LoaderInfoTable
const char McuVendor[32]="STMicroelectronics";

prama_location_LoaderInfoTable
const char AppAuthor[32]="Sample Project";

prama_location_LoaderInfoTable
const char HpfCipherKey[32]={0x00,0x00,};
prama_location_LoaderInfoTable
const char HpfBindSN[16]={0x00,0x00,};
prama_location_LoaderInfoTable
const unsigned long HpfProgramCountLimit=0;
prama_location_LoaderInfoTable
const unsigned long HpfCipherType=0;
//
unsigned long FlashSegmentProcess(unsigned long para);
unsigned long OptionSegmentProcess(unsigned long para);
prama_location_SegmentsInfoTable
const TSegmentInfomation flashSegment=
{
  .name="flash",
  .defaultValue=0xff,
  .startaddress=0x08000000,
  .size=2048ul*1024ul,
  .SegmentProcess=FlashSegmentProcess,
  .ExtraDataSize1024=4,//��KBΪ��λ
};
prama_location_SegmentsInfoTable
const TSegmentInfomation OptionSegment=
{
  .name="optionbytes",
  .defaultValue=0xff,
  .startaddress=0x1ffff800,
  .size=16,
  .SegmentProcess=OptionSegmentProcess,
  .ExtraDataSize1024=2,//��KBΪ��λ
};
prama_location_SegmentsInfoTable
const TSegmentInfomation EndSeg=
{
  .name="",
  .defaultValue=0x00,
  .startaddress=0x00,
  .size=0,
  .SegmentProcess=(unsigned long (*)(unsigned long ))0,
};
unsigned long FlashSegmentProcess(unsigned long para)
{
  return 0;
}
unsigned long OptionSegmentProcess(unsigned long para)
{
  return 0;
}
#include "string.h"
#include "HonSubs.h"
//--
#include "HonCommHalSample.c"


int main()
{
  //�����ֽ���optionbytes
  union{
    unsigned char byte_buf256[256];
    unsigned short u16_buf128[128];
    unsigned long u32_buf64[64];
  }tb;
  signed long result;
#ifdef __ICCARM__
  HonUi_DisplayStrLenXY("  STM32��������-����װ����ʾ-IAR EWARM\r\n",0);
#endif
#ifdef __CC_ARM
  HonUi_DisplayStrLenXY("  STM32��������-����װ����ʾ-KeilMDK\r\n",0);
#endif
	
  result=HonFile_SwitchSegment(1,tb.byte_buf256,sizeof(TSegmentInfomation));
  if(result<0)
  {
    HonUi_DisplayStrLenXY("  װ��ѡ���ֽڴ���,Check OptionBytes Error\r\n",0);
  }
  TSegmentInfomation *seginfo=(TSegmentInfomation*)tb.byte_buf256;
  unsigned long startaddress=(*seginfo).startaddress;
  unsigned long loadedsize=(*seginfo).loadedsize;
  if(startaddress!=0x1ffff800)
  {
    HonUi_DisplayStrLenXY("  װ��ѡ���ֽڴ���,OptionBytes Error StartAddress Error\r\n",0);
  }
  if(loadedsize<16)
  {
    HonUi_DisplayStrLenXY("  װ��ѡ���ֽڴ���,OptionBytes Error loadedsize Error\r\n",0);
  }
  if(HonFile_ReadSegmentData(startaddress+0,tb.byte_buf256,16)<0)
  {
    HonUi_DisplayStrLenXY("  װ��ѡ���ֽڴ���,Read OptionBytes Error\r\n",0);
    return -6;
  }
  if(tb.byte_buf256[0]==0xa5)
  {
    HonUi_DisplayStrLenXY("  ������δ����,RDP is No Active\r\n",0);
  }
  else
  {
    HonUi_DisplayStrLenXY("  �������ѿ���,RDP is Set Active\r\n",0);
  }
  HonUi_DisplayStrLenXY("  ѡ���ֽ�OptionBytes:\r\n",0);
  if(1)
  {
    u8 displaybuf[128];
    memset(displaybuf,' ',8);
    for(int i=0;i<16;i++)
    {
      displaybuf[i*2+0+2]=toHex(tb.byte_buf256[i]>>4);
      displaybuf[i*2+1+2]=toHex(tb.byte_buf256[i]>>0);
    }
    HonUi_DisplayStrLenXY((char*)displaybuf,(32<<20)+2);
  }
  u64 SN=0x000055aa;
  WriteSerialNo(&SN);
  HonUi_DisplayStr("\r\n  �ѳ�ʼ���Զ�����:",0,0,0);
  DisplayHex32A(SN,0,0,0);
  HonUi_DisplayStr("\r\n",0,0,0);
  inlineasm("bkpt 0");
  return 0;
}

