#ifndef __INCLUDE_H
#define __INCLUDE_H
#include "sys.h"
#include "stm32f10x_tim.h"
#include "adc.h"
#include "stdio.h"
#include "stmflash.h"
#include <stdbool.h>
#include "key.h"
#include "led.h"

#define FLASH_SAVE_ADDR  0X08070000 				//flash存储地址

//读信号
#define	CylSh01 0		//PE1		//气缸01收缩  内构压  对应屏幕0X1寄存器
#define	CylRe01 1		//PB9		//气缸01舒张

#define	CylSh02 2		//PB6		//气缸02收缩	内构推
#define	CylRe02 3		//PB4		//气缸02舒张

#define	CylSh03 4		//PD7		//气缸03收缩	瓶盖压
#define	CylRe03 5		//PD5		//气缸03舒张

#define	CylSh04 6		//PD3		//气缸04收缩  瓶盖推
#define	CylRe04 7		//PD1		//气缸04舒张

#define	CylSh05 8		//PC12		//气缸05收缩	
#define	CylRe05 9		//PC10		//气缸05舒张

#define	CylSh06 10	//PA14		//气缸06收缩	
#define	CylRe06 11	//PA12		//气缸06舒张


#define Light01 12	//PA8		//光电01  瓶身进料
#define Light02 13	//PC8		//光电02	瓶身上料
#define Light03 14	//PC6		//光电03  內构进料
#define Light04 15	//PD14		//光电04	瓶盖上料

#define CylOp01 16	//Status_Data[0]		//气缸01号运行情况
#define CylOp02 17	//Status_Data[1]		//气缸02号运行情况
#define CylOp03 18	//Status_Data[2]		//气缸03号运行情况

#define ChuSt01 19  //Status_Data[3]    //料槽01状态
#define ChuSt02 20  //Status_Data[4]    //料槽02状态
#define ChuSt03 21  //Status_Data[5]    //料槽03状态

#define AllSt01 22 	//Status_Data[6]		//整机运行状态

#define TurTa01 23  //PB12    //转盘信号



//控制
#define StopStart 	     0     //启动停机按钮
#define CountClear 		   1	   //计数清零
#define ReSet     		   2     //整机复位
#define BottleM     	   3     //瓶身上料
#define InternalM   	   4     //內构上料
#define CapM         	   5     //瓶盖上料
#define ScrewCap		     6	   //拧瓶盖
#define InternaPush_C    7	   //推料气缸控制-內构
#define ScrewPush_C	  	 8     //推料气缸控制-瓶盖
#define Turntable		     9     //转盘控制
#define InternalPM_C     10	   //压料气缸控制-內构
#define CapPM_C          11    //压料气缸控制-瓶盖
typedef struct 
{
	u16 adc;
	u8 	key1;
	u8  key2;
	u8  ResetOK;
	enum 
	{
		Run=0,				//运行
	  Reset 				//复位
	
	}WorkStatus;//机器工作状态运行、复位
	enum 
	{
		Endend=0,		  //结束
		Ending,    		//准备结束
		Starting,			//准备开始
		Startend,  		//开始
	 	Stop			 		//暂停
	  	
	}Status;//机器状态开始、暂停、结束
}Machine;
	
extern Machine Mach;

#endif



