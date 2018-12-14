#include "sys.h"
#include "include.h"
#include "runcontrol.h"
#include "RS485.h"

///正常运行
static void WorkRun(void)
{
	
}

///执行复位操作
//return 0:复位成功 1-8：复位失败气缸编号
static u8 WorkReset(void)
{
	static u8 WorkReset_Step=0;
	switch(WorkReset_Step)
	{
		case 0:   //执行复位操作 所有气缸复位，转盘转一圈
		{
			break;
		}
		case 1:		//复位完成或者故障
		{
			break;
		}
		default: WorkReset_Step=0;break;
	}
	return 0;
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

void RunStatus(void)
{
	Status_Data[0]=CylinderShrink01;
	Status_Data[1]=CylinderRelaxation01;
	Status_Data[2]=CylinderShrink02;
	Status_Data[3]=CylinderRelaxation02;
	if(CylinderShrink01==0&&CylinderRelaxation01)  			//气缸收缩且没舒张
	{
		Cylinder_Data[3]=0;
	}
	else if(CylinderShrink01&&CylinderRelaxation01==0)  //气缸舒张且没收缩
	{
		Cylinder_Data[3]=1;
	}
	else
	{
		Cylinder_Data[3]=2;
	}
	
	if(CylinderShrink02==0&&CylinderRelaxation02) 		  //气缸收缩且没舒张为正常状态
	{
		Cylinder_Data[2]=0;
	}
	else if(CylinderShrink02&&CylinderRelaxation02==0)  //气缸舒张且没收缩
	{
		Cylinder_Data[2]=1;
	}
	else
	{
		Cylinder_Data[2]=2;//故障
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
	return 0;
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
		return 2;//故障
	}
	return 0;
}


//内构件下压气缸控制
u32 Systime_cnt=0;
u8 CylinderStep_Flag=0,AllTime_Cnt=0;
u8 CylinderStepConnect(void)
{
	static u8 Step=0,time_cnt=0,cishujishu=0,ErrorTime_cnt=0,Light_cnt=0;
	static u32 systime=0;

	time_cnt=0;
	switch(Step)
	{
		case 0:
		{
			if(Materials02==0)//光电滤波
			{
				Light_cnt++;
			}
			else
			{
				Light_cnt=0;
			}
			if(Light_cnt>10)//光电传感器100ms内
			{			
				if(CylinderShrink02==0&&CylinderRelaxation02) 		  //气缸2收缩且没舒张为正常状态
				{
					systime=Systime_cnt;//取时间
					Cylinder02=1;//气缸2舒张
					AllTime_Cnt=10;//下一步延时10*10ms
					ErrorTime_cnt=0;
					Step=1;
				}
				else
				{
					ErrorTime_cnt++;//动作超时计数
				}
				
				if(ErrorTime_cnt>100)//动作10*100ms则报故障
				{
					ErrorTime_cnt=0;
					CylinderStep_Flag=0;
					return 2;//气缸2故障
				}
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
					ErrorTime_cnt=0;
					Step=2;
				}
			}
			else
			{
				ErrorTime_cnt++;//动作超时计数
			}
			
			if(ErrorTime_cnt>100)//动作10*100ms则报故障
			{
				ErrorTime_cnt=0;
				CylinderStep_Flag=0;
				return 2;//气缸2故障
			}
			break;
		}
		case 2:
		{
			if(CylinderShrink01&&CylinderRelaxation01==0)  //气缸1舒张且没收缩
			{
				Cylinder01=0;//气缸1收缩
				ErrorTime_cnt=0;
				Step=3;
			}
			else
			{
				ErrorTime_cnt++;//动作超时计数
			}
			
			if(ErrorTime_cnt>100)//动作10*100ms则报故障
			{
				ErrorTime_cnt=0;
				CylinderStep_Flag=0;
				return 1;//气缸1故障
			}
			break;
		}
		case 3:
		{
			if(CylinderShrink01==0&&CylinderRelaxation01)  			//气缸1收缩且没舒张
			{
				Cylinder02=0;//气缸2收缩
				ErrorTime_cnt=0;
				Step=4;
			}
			else
			{
				ErrorTime_cnt++;//动作超时计数
			}
			
			if(ErrorTime_cnt>100)//动作10*100ms则报故障
			{
				ErrorTime_cnt=0;
				CylinderStep_Flag=0;
				return 1;//气缸1故障
			}
			break;
		}
		case 4:
		{
			
			if(CylinderShrink02==0&&CylinderRelaxation02) 		  //气缸2收缩且没舒张为正常状态
			{
				cishujishu++;
				if(cishujishu>200)
				{
					cishujishu=0;
				}
				
				printf("Time=%d 次数:%d\n",Systime_cnt-systime,cishujishu);
				ErrorTime_cnt=0;
				Step=0;
				CylinderStep_Flag=0;
			}
			else
			{
				ErrorTime_cnt++;//动作超时计数
			}
			
			if(ErrorTime_cnt>100)//动作10*100ms则报故障
			{
				ErrorTime_cnt=0;
				CylinderStep_Flag=0;
				return 2;//气缸2故障
			}
			break;
		}
	}	
	return 0;
}

//按键总控制
void CylinderAllConnect(u8 key)//不支持连按  //10ms进入一次
{
	u8 Error=0;
	if(AllTime_Cnt)
	{
		AllTime_Cnt--;
	}
	if(key==1)
	{
		Error=CylinderConnect01();
	}
	else if(key==2)
	{
		Error=CylinderConnect02();
	}
	else if(key==3)
	{
		CylinderStep_Flag=1;
	}
	//內构上料
	else if(CylinderStep_Flag)
	{
		Error=CylinderStepConnect();
	}
	
	if(Error)//错误信息不为0
	{
		printf("气缸%d--发生故障\n",Error);
		Error=0;
	}
	
}









