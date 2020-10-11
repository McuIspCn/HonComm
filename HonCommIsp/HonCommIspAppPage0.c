/**********************
CopyRight www.mcuisp.com,2008~2020


**********************/

#include "HonCommTypeDef_FuncDef.h"
#include "HonSubs.h"
#include "HonCommHalSample.c"



__root const unsigned char app0data[256]={
  0x55,0xaa,0x12,0x34,0x56,0x78,0x89,0xab,0x12,0x36,
};

const u8 str1[]="app0func 演示代码\r\n";
const u8 str2[]="app0func 演示代码2\r\n";
int app0func(void)
{
  HonUi_DisplayStrLenXY((char*)str1,0);
  DisplayMemoryA((u8*)app0data,0,0,32);
  for(;1;)
  {
    s32 key=HonUi_GetKey(0);
    if(key==0xe)
    {
      int app0sub(void);
      app0sub();
    }
    else if(key==0x1)
    {
      int basesub0(void);
      basesub0();
    }
    else if(key==8)
    {
      break;
    }
  }
  return 0;
}
int app0sub(void)
{
  HonUi_DisplayStrLenXY((char*)str2,0);
  return 0;
}
