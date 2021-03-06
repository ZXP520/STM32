#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


#define Cylinder01 PEout(2)   //气缸01
#define Cylinder02 PEout(4)   //气缸02
#define Cylinder03 PEout(6)	  //气缸03
#define Cylinder04 PCout(13) 	//气缸04
#define Cylinder05 PCout(15) 	//气缸05
#define Cylinder06 PCout(0) 	//气缸06  吹气
#define Screw_Cap01  PCout(15)// PCout(2) //拧瓶盖0111 拧瓶盖就是气缸05
#define AllPower 	   PEout(11)   //总电控制
#define Turntable01  PEout(13) //转盘PEout(11)为继电器2
#define BeefLedRed   PEout(9)
#define LedYellow 	 PEout(7)
#define LedGreen     PBout(1)
#define X4    			 PCout(5)//继电器4  
void LED_Init(void);//初始化
void TurnOnLedRed(void);
void TurnOnLedYellow(void);
void TurnOnLedGreen(void);
		 				    
#endif
