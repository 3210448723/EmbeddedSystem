/*********************普通按键以及导航按键头文件********************
* 作者：宁晓兰
******************************************************************/

#ifndef _KEY_H
#define _KEY_H

#include "sys.h"
#include "delay.h"

//按键端口定义
#define key3 PCin(0)
#define key2 PCin(1)
#define key1 PCin(2)

#define keyCountMax 10

/****************变量设置*******************/
// extern u8 keyCountTotal; //设置按键检测次数
// extern u8 keyCount3; //按键3按下过程中抖动的次数
// extern u8 keyCount2; //按键2按下过程中抖动的次数
// extern u8 keyCount1; //按键1按下过程中抖动的次数
// extern u8 keyC3; //key3当前状态
// extern u8 keyP3; //key3前一个状态
// extern u8 keyC2; //key2当前状态
// extern u8 keyP2; //key2前一个状态
// extern u8 keyC1; //key1当前状态
// extern u8 keyP1; //key1前一个状态

/****************操作函数******************/
void KeyInit(void); //按键IO初始化
u8 KeyScan(void);  //按键扫描函数

void KeyNavInit(void); //导航按键初始化
u16 GetNavAdc(u8 ch); //获取ADC
u8 ScanNavKey(u8 ch); //扫描判断导航方向


#endif
