# HonComm

#### 介绍
鸿盟 单片机 串口下载软件

#### 软件架构
使用C语言，可用IAR EWARM或Keil MDK编译
可运行在HonComm.exe串口平台上
或运行在HP968手持编程器上

#### 安装教程

1.  HonCommIsp目录---源代码，可用IAR EWARM和Keil MDK打开
2.  HonComm.exe---运行软件，运行于Windows系统
3.  1GPIOTestForEpLink20160829.hex 1opt1FFFF800__a5_NoWrp.txt，烧录测试的hex代码和选项字节设置
4.  HonCommApp.hex HonCommAppKeil.hex,支持STM32F1串口下载的App。


#### 使用说明

1.  用IAR EWARM打开HonComm.eww，或用KeilMdk打开HonComm.uvmpw，选择BatchBuild，即可编译
2.  产生的HonCommApp.Hex在..\objout\目录下。
3.  运行HonComm.exe，点选App模式，按 装入并运行App，选择HonCommApp.hex或HonCommAppKeil.hex
4.  HonComm.exe软件首先按Loader项目中的设置装入烧录的目标代码(1GPIOTestForEpLink20160829.hex 1opt1FFFF800__a5_NoWrp.txt)。
5.  之后运行Loader项目对目标代码进行检查和处理
6.  最后运行IspApp项目开始串口下载(需要先选择串口并打开)。

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
