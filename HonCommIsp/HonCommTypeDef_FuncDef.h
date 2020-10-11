/**********************
CopyRight www.mcuisp.com,2008~2020

**********************/


#ifndef HONCOMMTYPEDEF_H
#define HONCOMMTYPEDEF_H

#ifdef __CC_ARM
#pragma anon_unions
#endif
#ifdef __CC_ARM
#define __root __attribute__((used))
#endif
#ifdef __ICCARM__
#define inlineasm asm
#endif
#ifdef __CC_ARM
#define inlineasm __asm
#endif

typedef enum{//定义EP968的IO
  J_TRST=0,
  J_TDI=1,
  J_TMS=2,
  J_TCK=3, //TCK可设置内部上拉、下拉电阻和阻值
  J_RTCK=4,
  J_TDO=5,
  J_RST=6,
  J_NC1=7, //NC1输入仅适合低速信号(10K电阻接内部INPUT)
  J_NC2=8, //NC2输入输出能力均很弱(经4.7K电阻到内部IO)
}TJtagPin;
#define J_6P5_Boot0 J_TDO
#define J_6P4_Reset J_TRST

#define KEY_UP  (0x10)
#define KEY_DOWN  (0x11)
#define KEY_LEFT  (0x12)
#define KEY_RIGHT  (0x13)
#define KEY_BACKSPACE  (0x14)
#define KEY_CR  (0x15)
#define KEY_OK  (KEY_CR)
#define KEY_ESC  (0x16)
#define KEY_CANCEL  (KEY_ESC)


typedef unsigned char  __uint8_t;
typedef signed   char  __int8_t;
typedef unsigned short __uint16_t;
typedef signed   short __int16_t;
typedef unsigned long   __uint32_t;
typedef signed   long   __int32_t;
typedef unsigned long long   __uint64_t;
typedef signed long long   __int64_t;
typedef __uint32_t u32;
typedef __int32_t s32;
typedef __uint64_t u64;
typedef __int32_t i32;
typedef __int64_t i64;
typedef __uint8_t u8;
typedef __uint16_t u16;

typedef union{//数据段定义，用于装载数据如flash optionbytes等等
  unsigned char bin[64];
  struct {
    char name[32];
    union{
      unsigned long defaultValue;
      unsigned long loadedsize;
    };
    unsigned long startaddress;
    unsigned long size;
    union{
      unsigned long (* SegmentProcess)(unsigned long para);
      unsigned long offsetinbuffer;
    };
    //48
    unsigned long ExtraDataSize1024;//在segment空间前预留不参与md/crc的数据空间
    unsigned long ExtraDataoffsetinbuffer;//无意义
  };
}TSegmentInfomation;
typedef union{
  unsigned char bin[16];
  struct{
    unsigned char *data;
    unsigned char *databak;
    unsigned long datalen;
    unsigned long address;
  };
}TDataMaskInfo;
#define DefHonLibBaseAddress (0x20010000+1)
#define DefHonCommUartBaseFunctions(Number) (DefHonLibBaseAddress+0+(Number)*8)
#define DefHonFileBasicFunctions(Number) (DefHonLibBaseAddress+0x200+(Number)*8)
#define DefHonStm32IspFunctions(Number) (DefHonLibBaseAddress+0x400+(Number)*8)


#define GetHonCommVersion ((s32 (*)())DefHonCommUartBaseFunctions(0))
#define HonComm_ReadUartWithTimeOut ((s32 (*)(u32 ms))DefHonCommUartBaseFunctions(1))
//return >=0表示接收到byte,<0未接收到
#define HonComm_SendAByte ((s32 (*)(u32 byte))DefHonCommUartBaseFunctions(2))
#define HonComm_WaitSendFinish ((s32 (*)(u32 usTimeout))DefHonCommUartBaseFunctions(3))
#define HonComm_SetDtrRtsBreak ((s32 (*)(u32 cmds))DefHonCommUartBaseFunctions(4))
//cmds.0 SetBreak,cmds.1 SetRts,cmds.2 SetDtr;cmds.8 ClrBreak,cmds.9 ClrRts, cmds.10 ClrDtr
#define SetCom_SETBREAK (1<<0)
#define SetCom_SETRTS (1<<1)
#define SetCom_SETDTR (1<<2)
#define SetCom_CLRBREAK (1<<8)
#define SetCom_CLRRTS (1<<9)
#define SetCom_CLRDTR (1<<10)
#define HonComm_ReadDsrCtsDtrRtsBreak ((s32 (*)(void))DefHonCommUartBaseFunctions(5))
//bit.0 break_flag,bit.1 rts,bit.2 dtr,bit.3 cts,bit.4 dsr
#define HonComm_GetSetBPS ((s32 (*)(u32 bps))DefHonCommUartBaseFunctions(10))
  //参数为0获取当前的bps,不为0设置bps
#define HonComm_GetSetComSettings ((s32 (*)(u32 settings))DefHonCommUartBaseFunctions(11))
  //参数为0获取当前的格式,不为0设置格式
//Settings.bit0~3 DataBits,bit4~5 StopBits,bit6 fParity, bit7 NoUse, bit8~11 ParityMode
#define SetChFMT_DataBit8 (8<<0)
#define SetChFMT_DataBit7 (7<<0)
#define SetChFMT_DataBit6 (6<<0)
#define SetChFMT_DataBit5 (5<<0)
#define SetChFMT_ONESTOPBIT (0<<4)
#define SetChFMT_ONE5STOPBITS (1<<4)
#define SetChFMT_TWOSTOPBITS (2<<4)
#define SetChFMT_DoParityCheck (1<<6)
#define SetChFMT_NOPARITY (0<<8)
#define SetChFMT_ODDPARITY (1<<8)
#define SetChFMT_EVENPARITY (2<<8)
#define SetChFMT_MARKPARITY (3<<8)
#define SetChFMT_SPACEPARITY (4<<8)

#define HonComm_DelayAtLeastUs ((s32 (*)(u32 us))DefHonCommUartBaseFunctions(16))
#define HonComm_GetMsTicks ((u32 (*)(void))DefHonCommUartBaseFunctions(17))
#define HonIO_GetSetPIN ((s32 (*)(u32 mode24b_PIN8b))DefHonCommUartBaseFunctions(24))

//界面交互支持函数
#define HonUi_DisplayStrLenXY ((s32 (*)(char *str,u32 lenxy))DefHonCommUartBaseFunctions(32))
#define HonUi_GetKey ((s32 (*)(u32 timeoutms))DefHonCommUartBaseFunctions(33))
#define HonUi_DisplayProgress ((s32 (*)(u32 AAA,u32 BBB,u32 Y))DefHonCommUartBaseFunctions(34))

//代码文件读取函数集
#define HonFile_SwitchSegment ((s32 (*)(u32 SegmentNo,u8 *buf,u32 buflen))DefHonFileBasicFunctions(0))
#define HonFile_ReadSegmentData ((s32 (*)(u32 address,u8 *buf,u32 len))DefHonFileBasicFunctions(1))
#define HonFile_WriteSegmentData ((s32 (*)(u32 address,u8 *buf,u32 len))DefHonFileBasicFunctions(2))
#define HonFile_CmpSegmentData ((s32 (*)(u32 address,u8 *buf,u32 len))DefHonFileBasicFunctions(3))
#define HonFile_ReadSegmentExtraData ((s32 (*)(u32 address,u8 *buf,u32 len))DefHonFileBasicFunctions(4))
#define HonFile_WriteSegmentExtraData ((s32 (*)(u32 address,u8 *buf,u32 len))DefHonFileBasicFunctions(5))

#define HonFile_GetCrc32_4c11db7 ((u32 (*)(u32 *dwbuf,u32 dwlen,u32 precrc))DefHonFileBasicFunctions(16))
#define HonFile_GetCrc16_1021 ((s32 (*)(u8 *buf,u32 len,u32 precrc))DefHonFileBasicFunctions(17))
#define HonFile_GetBackCrc32_4c11db7 ((u32 (*)(u32 *dwbuf,u32 dwlen,u32 endcrc))DefHonFileBasicFunctions(20))
#define HonFile_GetCrc32ImdData_4c11db7 ((u32 (*)(u32 precrc,u32 endcrc))DefHonFileBasicFunctions(21))
#define HonFile_GetCrc16ImdData_1021 ((s32 (*)(u16 precrc,u16 endcrc))DefHonFileBasicFunctions(22))

//stm32AN3155支持函数集
#define STM32AN3155_ConnectChip ((s32 (*)(u32 ConnectTimeOutMs))DefHonStm32IspFunctions(0))
#define STM32AN3155_PollChip ((s32 (*)(u32 ConnectTimeOutMs))DefHonStm32IspFunctions(1))
#define STM32AN3155_Get ((s32 (*)(u8 *receivedbuf))DefHonStm32IspFunctions(2))
#define STM32AN3155_GetVersion_RdpStatus ((s32 (*)(u8 *receivedbuf))DefHonStm32IspFunctions(3))
#define STM32AN3155_GetChipProductID ((s32 (*)(u8 *receivedbuf))DefHonStm32IspFunctions(4))
#define STM32AN3155_ReadMemory ((s32 (*)(u32 address,u8 *buf,u32 len))DefHonStm32IspFunctions(5))
#define STM32AN3155_GoRun ((s32 (*)(u32 address))DefHonStm32IspFunctions(6))
#define STM32AN3155_WriteMemory ((s32 (*)(u32 address,u8 *buf,u32 len))DefHonStm32IspFunctions(7))
#define STM32AN3155_Erase43 ((s32 (*)(u32 timeoutsecondhigh12bit_pageCount,u8 *pagenumbers))DefHonStm32IspFunctions(8))
#define STM32AN3155_Erase44 ((s32 (*)(u32 timeoutsecondhigh12bit_pageCount,u16 *pagenumbers))DefHonStm32IspFunctions(9))
#define STM32AN3155_WriteProtect ((s32 (*)(u32 SectorCount,u8 *sectornumbers))DefHonStm32IspFunctions(10))
#define STM32AN3155_WriteUnProtect ((s32 (*)(void))DefHonStm32IspFunctions(11))
#define STM32AN3155_ReadOutProtect ((s32 (*)(void))DefHonStm32IspFunctions(12))
#define STM32AN3155_ReadOutUnProtect ((s32 (*)(u32 timeoutsecond_high12bit))DefHonStm32IspFunctions(13))

/*
0x20000000~0x2000FFFF MainAppSram 64KB,use 12KB now
0x20010000~0x2001FFFF AppLib      64KB
0x20020000~0x2002FFFF MainAppCode 64KB,use 6KB now
0x20030000~0x200FFFFF SubAppCode  64KB每个,1MB,13个

0x20200000~0x2020FFFF LoaderCode  64KB,可扩展
0x20400000~0x2040FFFF LoaderSram  64KB
*/
#endif//HONCOMMTYPEDEF_H



