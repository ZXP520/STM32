#include "sys.h"
#include "include.h"
#include "runcontrol.h"


///正常运行
static void WorkRun(void)
{
	
}

///执行复位操作
static void WorkReset(void)
{
	
}

///机器运行状态控制
void WorkRunControl(void)
{
	switch(Mach.WorkStatus)
	{
		case Run:
		{
			WorkRun();
			break;
		}
		case Reset:
		{
			WorkReset();
			break;
		}
		default: break;
	}
}

///机器运行状态
void RunControl(void)
{
	switch(Mach.Status)
	{
		case Endend:
		{
			break;
		}
		case Ending:
		{
			
			Mach.WorkStatus=Reset;//结束键复位机器
			break;
		}
		case Starting:
		{
			if(Mach.ResetOK==0)
			{
				Mach.WorkStatus=Reset;//启动键复位机器
			}
			else if(Mach.ResetOK==1)
			{
				Mach.WorkStatus=Run;//启动键复位机器
			}
			break;
		}
		case Startend:
		{
			break;
		}
		case Stop:
		{
			Mach.ResetOK=0;
			break;
		}
		default: break;
	}
}


///气缸控制  內构

u8 CylinderConnect01(void)
{
	if(CylinderShrink01==0&&CylinderRelaxation01)  			//气缸收缩且没舒张
	{
		Cylinder01=1;
	}
	else if(CylinderShrink01&&CylinderRelaxation01==0)  //气缸舒张且没收缩
	{
		Cylinder01=0;
	}
	else
	{
		return 1;//故障
	}
}

u8 CylinderConnect02(void)
{
	if(CylinderShrink02==0&&CylinderRelaxation02) 		  //气缸收缩且没舒张为正常状态
	{
		Cylinder02=1;
	}
	else if(CylinderShrink02&&CylinderRelaxation02==0)  //气缸舒张且没收缩
	{
		Cylinder02=0;
	}
	else
	{
		return 1;//故障
	}
}

u32 Systime_cnt=0;
u8 CylinderStep_Flag=0,AllTime_Cnt=0;
void CylinderStepConnect(void)
{
	static u8 Step=0,time_cnt=0,cishujishu=0;
	static u32 systime=0;
	time_cnt++;
	if(time_cnt<=10)
	{
		return;
	}
	time_cnt=0;
	switch(Step)
	{
		case 0:
		{
			if(CylinderShrink02==0&&CylinderRelaxation02) 		  //气缸2收缩且没舒张为正常状态
			{
				systime=Systime_cnt;//取时间
				Cylinder02=1;//气缸2舒张
				AllTime_Cnt=10;//下一步延时40*5ms
				Step=1;
			}
			break;
		}
		case 1:
		{
			if(CylinderShrink02&&CylinderRelaxation02==0)  //气缸2舒张且没收缩
			{
				if(AllTime_Cnt==0)
				{
					Cylinder01=1;//气缸1舒张
					Step=2;
				}
			}
			break;
		}
		case 2:
		{
			if(CylinderShrink01&&CylinderRelaxation01==0)  //气缸1舒张且没收缩
			{
				Cylinder01=0;//气缸1收缩
				Step=3;
			}
			break;
		}
		case 3:
		{
			if(CylinderShrink01==0&&CylinderRelaxation01)  			//气缸1收缩且没舒张
			{
				Cylinder02=0;//气缸2收缩
				Step=4;
			}
			break;
		}
		case 4:
		{
			cishujishu++;
			if(cishujishu>200)
			{
				cishujishu=0;
			}
			printf("Time=%d 次数:%d\n",Systime_cnt-systime,cishujishu);
			Step=0;
			
			CylinderStep_Flag=0;
			break;
		}
	}
}


void CylinderAllConnect(u8 key)//不支持连按  //5ms进入一次
{
	if(AllTime_Cnt)
	{
		AllTime_Cnt--;
	}
	if(key==1)
	{
		CylinderConnect01();
	}
	else if(key==2)
	{
		CylinderConnect02();
	}
	else if(key==3)
	{
		CylinderStep_Flag=1;
	}
	else if(CylinderStep_Flag)
	{
		CylinderStepConnect();
	}
}









