#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

// 
//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define KEY3 PAin(0)	//PA0  WK_UP
 
#define KEY0  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键3(WK_UP) 



/*触发为低*/
#define CylinderShrink01 		  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)//气缸收缩
#define CylinderRelaxation01  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)//气缸舒张
#define CylinderShrink02 		  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)//气缸收缩
#define CylinderRelaxation02  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)//气缸舒张
#define Materials02						GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)//物料2


#define KEY_UP 		4
#define KEY_LEFT	3
#define KEY_DOWN	2
#define KEY_RIGHT	1

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数					    
#endif
