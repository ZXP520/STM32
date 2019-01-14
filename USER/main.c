#include "sys.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
#include "pid.h"
#include "include.h"
#include "runcontrol.h"
#include "RS485.h"

//ALIENTEK战舰STM32开发板实验4
//串口实验  

extern u8 Flag_1ms,Flag_5ms,Flag_10ms,Flag_20ms,Flag_100ms,Flag_500ms,Flag_1000ms; //时间标志
Machine Mach;
static void Stm32_Init(void)
{
	
	delay_init();	    	 				//延时函数初始化	  
	NVIC_Configuration(); 	 		//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	TIM4_Int_Init(1000-1,72-1); //1M  1ms定时
	RS485_Init();								//MODBUS初始化
	uart_init(115200);				  //串口初始化为115200
 	LED_Init();			     				//LED端口初始化
	KEY_Init();          				//初始化与按键连接的硬件接口
	STMFLASH_Read(FLASH_SAVE_ADDR,Cylinder_Data,2);//读Flash数据  产量数据
	printf("System inint sucess!\n");
	
}
int main(void)
 {		
	Stm32_Init();
 	while(1)
	{
		
		if(Flag_1ms)  //1MS
		{
			Flag_1ms=0;
			RS485_Service();//modbus数据处理，1MS处理一次
			Systime_cnt++;
		}
		if(Flag_5ms)  //5MS
		{
			Flag_5ms=0;
		}
		if(Flag_10ms)	//10MS
		{
			Flag_10ms=0;
			ALLControl_10ms();
		}
		if(Flag_20ms)	//20MS
		{
			Flag_20ms=0;
		}
		if(Flag_100ms)	//100MS
		{
			Flag_100ms=0;
			RunStatus();
		}
		if(Flag_500ms)	//500MS
		{
			Flag_500ms=0;
		}
		if(Flag_1000ms)  //1S
		{
			Flag_1000ms=0;
		}
	}		
 }

