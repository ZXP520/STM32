#include "led.h"

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

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化



void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PA,PB,PC,PE,PE端口时钟
 //PA0 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
 //PB1
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
 //PC0 C2 C5 C13 C15
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_13|GPIO_Pin_15;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 
 //PE2 E4 E6 E7 E9 E11 E13
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
 //输出低
 GPIO_ResetBits(GPIOA,GPIO_Pin_0);						
 GPIO_ResetBits(GPIOB,GPIO_Pin_1);		
 GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_13|GPIO_Pin_15);						 
 GPIO_ResetBits(GPIOE,GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13);
	
}
void TurnOnLedRed(void)
{
	static u8 LedRedTime_Cnt=0;
	LedRedTime_Cnt++;
	if(LedRedTime_Cnt<11)//1s
	{
		BeefLedRed=1;
		LedYellow=0;
		LedGreen=0;
	}
	else if(LedRedTime_Cnt>20)
	{
		LedRedTime_Cnt=0;
	}
	else
	{
		BeefLedRed=0;
		LedYellow=0;
		LedGreen=0;
	}
	
}
void TurnOnLedYellow(void)
{
	BeefLedRed=0;
	LedYellow=1;
	LedGreen=0;
}
void TurnOnLedGreen(void)
{
	BeefLedRed=0;
	LedYellow=0;
	LedGreen=1;
}
 
