/**********************
CopyRight www.mcuisp.com,2008~2020
一些常用的函数
函数必须定义为static型
**********************/
static s32 Hon_SetVcctOut(u32 mV)
{
  //mv==0 ShutDown,mv==1 SetOldVcct,mv==1000~5500 SetNewVcct And Output
  //return VcctValue
  return HonIO_GetSetPIN((mV<<8)+128);
}
static s32 Hon_SetVcctCurrentLimit(u32 mA)
{
  //mA
  //return VcctCurrentLimit
  return HonIO_GetSetPIN(((mA*2/5)<<8)+129);
}
static s32 Hon_SetVpp(u32 mV)
{
  //Vpp 3000~13000 mV
  //return VppValue
  return HonIO_GetSetPIN((mV<<8)+130);
}

static s32 Hon_PowerUpVcct(u32 timeoutms,u32 minvcct)
{
  timeoutms=timeoutms/100;minvcct=minvcct/50;
  timeoutms&=0x3ff;minvcct&=0x3ff;
  return HonIO_GetSetPIN( (1<<28)+(timeoutms<<18)+(minvcct<<8)+131);
}
static s32 Hon_PowerDownVcct(u32 timeoutms,u32 minvcct)
{
  timeoutms=timeoutms/100;minvcct=minvcct/50;
  timeoutms&=0x3ff;minvcct&=0x3ff;
  return HonIO_GetSetPIN( (0<<28)+(timeoutms<<18)+(minvcct<<8)+131);
}

static s32 Hon_ShutDownAllPin()
{
  return HonIO_GetSetPIN( 136);
}
static s32 Hon_OpenUartPort()
{
  return HonIO_GetSetPIN( (1<<8)+137);
}
static s32 Hon_CloseUartPort()
{
  return HonIO_GetSetPIN( (0<<8)+137);
}
typedef enum
{ 
  PullDisable=0,
  PullLow4k7=1,
  PullLow750=2,
  PullLow680=3,
  PullDisableA=4,
  PullHigh4k7=5,
  PullHigh750=6,
  PullHigh680=7,
}TTckPullMode;
static void Hon_SetTckPullUpDown(TTckPullMode pullmode)
{
  u32 pm=pullmode;
  pm&=0x7;
  pm+=8;
  HonIO_GetSetPIN( (pm<<8)+J_TCK);
  return;
}
static void Hon_SetPin_Input( TJtagPin pin)
{
  HonIO_GetSetPIN( (7<<8)+(u32)pin);
  return;  
}
static void Hon_SetPin_Low( TJtagPin pin)
{
  HonIO_GetSetPIN( (0<<8)+(u32)pin);
  return;  
}
static void Hon_SetPin_High( TJtagPin pin)
{
  HonIO_GetSetPIN( (1<<8)+(u32)pin);
  return;  
}
static void Hon_SetPin_Disconnect( TJtagPin pin)
{
  HonIO_GetSetPIN( (2<<8)+(u32)pin);
  return;  
}

static void Hon_SetPin_OutVpp( TJtagPin pin)
{
  HonIO_GetSetPIN( (3<<8)+(u32)pin);
  return;  
}

static void Hon_SetLeds(u32 ledonoff,u32 bright100percent)
{
  ledonoff&=0xffff;//high 8 bit is Green
  bright100percent%=100;
  HonIO_GetSetPIN( (bright100percent<<24)+(ledonoff<<8)+132);
  return;  
}
static s32 Hon_ATE_IO(u32 cmd)
{
  //bit0 GreenLedOff,bit1 GreenLedOn,USB_DM
  //bit2 RedLedOff,bit3 RedLedOn,USB_DP
  //bit4 ReadATE_Input,USB_ID
  return HonIO_GetSetPIN((cmd<<8)+133);
}
static unsigned char toHex(unsigned char a)
{
  a=a&0xf;
  if(a>9)return a-0xa+'A';
  return a+'0';
}
#define DisplayMemoryA(mem,x,y,len) __Display__MemoryA(mem,((x&0x3ff)<<10)+((y&0x3ff)<<0)+((len&0xff)<<20));
static s32 __Display__MemoryA(u8 *mem,u32 lenxy)
{//以DisplayAsc8x16显示
  u8 buf[16];
  u32 len=(lenxy>>20)&0xff;
  u32 x=(lenxy>>10)&0x3ff;
  u32 y=(lenxy>>0)&0x3ff;
  for(int i=0;i<len;)
  {
    u32 dlen=8;
    if((dlen+i)>len)dlen=len-i;
    for(int j=0;j<dlen*2;j+=2)
    {
      buf[j+0]=toHex(mem[i+j/2]>>4);
      buf[j+1]=toHex(mem[i+j/2]>>0);
    }
    HonUi_DisplayStr((char*)buf,x,y,dlen*2);
    x+=dlen*8*2;
    if(x>=128){y+=16;x-=128;};
    i+=dlen;
  }
  return len;
}
static int IntToHex8(unsigned long a,char *s)
{
  s[8]=0;
  for(int i=8;;)
  {
    i--;
    if(i<0)break;
    s[i]=toHex(a);
    a>>=4;
  }
  return 8;
}
static int IntToHex(unsigned long a,char *s)
{
  int i=0;
  int j=0;
  for(;j<7;j++)
  {
    if((a>>28)!=0)break;
    a<<=4;
  }
  for(;j<8;j++)
  {
    s[i]=toHex(a>>28);
    i++;
    a<<=4;
  }
  s[i]=0;
  return i;
}
static int IntToStr(unsigned long ui32,unsigned char *str)
{
  unsigned long ui8,n;
  unsigned long ui32a=1000000000;//4294967295
  ui8=0;
  while(ui32a>1)
  {
    n=0;
    while(ui32>=ui32a)
    {
      ui32-=ui32a;
      n++;
    }
    if((n==0)&&(ui8==0))
    {
    }
    else
    {
      str[0]=toHex(n);
      ui8++;
      str++;
    }
    ui32a/=10;
  }
  str[0]=toHex(ui32);
  ui8++;
  str++;str[0]=0;
  return ui8;
}
static void Put0ToMinlen(u8 *digits,u32 minlen)
{
  for(;1;)
  {
    u32 slen=0;
    for(slen=0;slen<minlen;slen++)
    {
      if(digits[slen]==0)break;
    }
    if(slen>=minlen)break;
    u8 temp='0';
    u8 tempa;
    for(int i=0;i<=(slen+1);i++)
    {
      tempa=digits[i];
      digits[i]=temp;
      temp=tempa;
    }
  }
  //DisplayMemoryA(digits,0,0,16);
}
#define DisplayHex32A(hex,x,y,len) __Display__Hex32A(hex,((x&0x3ff)<<10)+((y&0x3ff)<<0)+((len&0xf)<<20));
static s32 __Display__Hex32A(u32 hex,u32 lenxy)
{
  char buf[16];
  u32 Count=IntToHex(hex,buf);
  u32 dwidth=(lenxy>>20)&0xf;
  u32 x=(lenxy>>10)&0x3ff;
  u32 y=(lenxy>>0)&0x3ff;
  if(dwidth>0)
  {
    if(Count<dwidth)
    {
      Put0ToMinlen((u8*)buf,dwidth);
      Count=dwidth;
    }
  }
  HonUi_DisplayStr(buf,x,y,Count);
  return Count;
}
#define DisplayInt32A(hex,x,y,len) __Display__Int32A(hex,((x&0x3ff)<<10)+((y&0x3ff)<<0)+((len&0xf)<<20));
static s32 __Display__Int32A(u32 ui32,u32 lenxy)
{//以DisplayAsc8x16显示
  u8 buf[16];
  u32 Count=IntToStr(ui32,buf);
  u32 dwidth=(lenxy>>20)&0xf;
  u32 x=(lenxy>>10)&0x3ff;
  u32 y=(lenxy>>0)&0x3ff;
  if(dwidth>0)
  {
    if(Count<dwidth)
    {
      Put0ToMinlen(buf,dwidth);
      Count=dwidth;
    }
  }
  HonUi_DisplayStr((char*)buf,x,y,Count);
  return Count;
}
//序列号(自动增量的例子),数据保存在flash segment的ExtraData区域
//使用 crc32 做校验,如不符，提示用户复位
static s32 WriteSerialNo(u64 *serialno)
{
  union{
  u8 buf[128];
  u16 buf16[128/2];
  u32 buf32[128/4];
  u64 buf64[128/8];
  }tb;
  int result;
  TSegmentInfomation *seginfo=(TSegmentInfomation*)tb.buf;
  result=HonFile_SwitchSegment(0,seginfo->bin,sizeof(TSegmentInfomation));
  if(result<0)return -1;
  u32 extradatasize=seginfo->ExtraDataSize1024*1024;
  if(extradatasize<512)return -2;
  result=HonFile_ReadSegmentExtraData(0,tb.buf,64);
  if(result<0)return -3;
  tb.buf64[0]=*serialno;
  //计算校验和
  u32 crc32=HonFile_GetCrc32_4c11db7(tb.buf32+0,2,0xffffffff);
  tb.buf32[2]=crc32;
  result=HonFile_WriteSegmentExtraData(0,tb.buf,64);
  if(result<0)return -4;
  return 0;
}
static s32 LoadSerialNo(u64 *serialno)
{
  union{
  u8 buf[128];
  u16 buf16[128/2];
  u32 buf32[128/4];
  u64 buf64[128/8];
  }tb;
  int result;
  TSegmentInfomation *seginfo=(TSegmentInfomation*)tb.buf;
  result=HonFile_SwitchSegment(0,seginfo->bin,sizeof(TSegmentInfomation));
  if(result<0)return -1;
  u32 extradatasize=seginfo->ExtraDataSize1024*1024;
  if(extradatasize<512)return -2;
  result=HonFile_ReadSegmentExtraData(0,tb.buf,64);
  if(result<0)return -3;
  //计算校验和
  u32 crc32=HonFile_GetCrc32_4c11db7(tb.buf32+0,2,0xffffffff);
  if(crc32!=tb.buf32[2])
  {//校验和不对，提示用户确认后，初始化
    HonUi_ClrRect(0,0,128);
    HonUi_DisplayStr("自动增量校验错误,按1键初始化\r\n",0,0,0);
    if(HonUi_GetKey(0)==1)
    {
      u64 serialno=0;
      return WriteSerialNo(&serialno);
    }
    return -4;
  }
  *serialno=tb.buf64[0];
  return 0;
}
static s32 WriteDataMask(TDataMaskInfo *mask,u32 address,u8 *buf,u32 len)
{//用mask的数据取代原数据，并备份原数据
  if((mask->address>=(address+len))||((mask->address+mask->datalen)<address))return 0;
  u32 i=0;
  for(i=0;i<mask->datalen;i++)
  {
    if(((mask->address+i)>=address)&&((mask->address+i)<(address+len)))
    {
      mask->databak[i]=buf[mask->address+i-address];
      buf[mask->address+i-address]=mask->data[i];
    }
  }
  return i;
}
static s32 VerifyDataMask(TDataMaskInfo *mask,u32 address,u8 *buf,u32 len)
{//校验mask的数据，并恢复原数据
  if((mask->address>=(address+len))||((mask->address+mask->datalen)<address))return 0;
  u32 i=0;
  for(i=0;i<mask->datalen;i++)
  {
    if(((mask->address+i)>=address)&&((mask->address+i)<(address+len)))
    {
      if(mask->data[i]!=buf[mask->address+i-address])return -1;
      buf[mask->address+i-address]=mask->databak[i];
    }
  }
  return i;
}


