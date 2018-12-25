#include "key.h"
#include "sys.h" 
#include "delay.h"
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
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化KEY0-->GPIOA.13,KEY1-->GPIOA.15  上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	
	// PA 3
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PB 5
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// PC 4
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// PD 8
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	// PE 1
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PE1~1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成浮空输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE1
	
	
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//按键按松开标志
//	static u8 KeyTime_cnt=0;
//	if(mode)key_up=1;  //支持连按		  
//	if(key_up&&(KEY0==1||KEY1==0||KEY2==0||KEY3==0))
//	{
//		KeyTime_cnt++;
//		//delay_ms(10);//去抖动 
//		if(KeyTime_cnt<=10)//去抖动 
//		{
//			return 0;
//		}
//		KeyTime_cnt=0;
//		
//		key_up=0;
//		if(KEY1==0)return 1;
//		else if(KEY2==0)return 2;
//		else if(KEY3==0)return 3;
//		else if(KEY0==1)return 4;
//	}else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==0)key_up=1; 	    
// 	return 0;// 无按键按下
//}
