#ifndef HONSUB_DEF_H
#define HONSUB_DEF_H

#define HonUi_ClrScr() HonUi_DisplayStrLenXY(0,0xffffffff)
#define HonUi_DelStr() HonUi_DisplayStrLenXY(0,0xfffffffe)
#define HonUi_ClrRect(x,y,height) HonUi_DisplayStrLenXY(0,((x&0x3ff)<<10)+((y&0x3ff)<<0)+(((height&0x3ff)+2048-512)<<20));
#define HonUi_DisplayStr(str,x,y,len) HonUi_DisplayStrLenXY(str,((x&0x3ff)<<10)+((y&0x3ff)<<0)+((len&0x7ff)<<20));
#define HonUi_DisplayStr1S(str,x,y,len) HonUi_DisplayStrLenXY(str,((x&0x3ff)<<10)+((y&0x3ff)<<0)+((len&0x7ff)<<20)+0x80000000);
#define HonUi_Beep(ms,div16k) HonUi_DisplayStrLenXY(NULL,0xfe000000+((ms&0x3ff)<<8)+((div16k&0x3f)<<0))

#endif//HONSUB_DEF_H
