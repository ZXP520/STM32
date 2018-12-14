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
//读信号
#define	CylSh01 0				//气缸01收缩  瓶盖旋  对应屏幕0X1寄存器
#define	CylRe01 1				//气缸01舒张

#define	CylSh02 2				//气缸02收缩	瓶盖压
#define	CylRe02 3				//气缸02舒张

#define	CylSh03 4				//气缸03收缩	瓶盖推
#define	CylRe03 5				//气缸03舒张

#define	CylSh04 6				//气缸04收缩  內构旋
#define	CylRe04 7				//气缸04舒张

#define	CylSh05 8				//气缸05收缩	內构压
#define	CylRe05 9				//气缸05舒张

#define	CylSh06 10			//气缸06收缩	內构推
#define	CylRe06 11			//气缸06舒张

#define	CylSh07 12			//气缸07收缩  夹紧左
#define	CylRe07 13			//气缸07舒张

#define	CylSh08 14			//气缸08收缩	夹紧右
#define	CylRe08 15			//气缸08舒张

#define Light01 16			//光电01  瓶身进料
#define Light02 17			//光电02	瓶身上料
#define Light03 18			//光电03  內构进料
#define Light04 19			//光电04	瓶盖上料

#define CylOp01 20			//气缸01号运行情况
#define CylOp02 21			//气缸02号运行情况
#define CylOp03 22			//气缸03号运行情况

#define ChuSt01 23      //料槽01状态
#define ChuSt02 24      //料槽02状态
#define ChuSt03 25      //料槽03状态

#define AllSt01 26 			//整机运行状态

#define TurTa01 27      //转盘信号



//控制
#define StopStart 	0     //启动停机按钮
#define TimeOut  		1     //暂停
#define CountClear  2			//计数清零
#define ReSet     	3     //整机复位
#define BottleM     4     //瓶身上料
#define InternalM   5     //內构上料
#define CapM        6     //瓶盖上料
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



