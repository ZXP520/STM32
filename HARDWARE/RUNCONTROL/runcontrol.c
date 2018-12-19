#include "sys.h"
#include "include.h"
#include "runcontrol.h"
#include "RS485.h"
#include "stdlib.h"


//创建队列
sequeue_t * create_empty_sequeue(void)
{
	sequeue_t * sq;
	//sq = (sequeue_t *)malloc(sizeof(sequeue_t));
	sq->front = sq->rear = 0;
  return sq;
}
sequeue_t *SQ;//定义队列

//队列初始化
void InitSequeuet(void)
{
	 SQ=create_empty_sequeue();
}

//判断队列是否为空
u8 check_seqeue_empty(sequeue_t * sq)
{
  return !(sq->front == sq->rear);
}

//入队
u8 enqueue(sequeue_t *sq ,u8 val)
{
    sq->rear = (sq->rear+1) % (N-1);     //循环存储
    sq->data[sq->rear] = val;
 
    return 0;
}

//出队
u8 dequeue(sequeue_t * sq)
{
  u8 val = 0;
  sq->front = (sq->front+1) % (N-1);
  val = sq->data[sq->front] ;
	//printf("p| %p ",&(sq->data[sq->front]));
  return val;
} 


/*
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
	
	if(check_seqeue_empty(SQ))//如果队列不为空
	{
		
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

*/


//气缸运行状态监测100ms
void RunStatus(void)
{
	
	if((*Modbus_InputIO[CylSh01]==0)&&(*Modbus_InputIO[CylRe01]))  		//气缸收缩且没舒张
	{
		Cylinder_Data[3]=0;
	}
	else if(*Modbus_InputIO[CylSh01]&&(*Modbus_InputIO[CylRe01]==0))  //气缸舒张且没收缩
	{
		Cylinder_Data[3]=1;
	}
	else
	{
		Cylinder_Data[3]=2;
	}
	
	if(*Modbus_InputIO[CylSh02]==0&&(*Modbus_InputIO[CylRe02])) 		  //气缸收缩且没舒张为正常状态
	{
		Cylinder_Data[2]=0;
	}
	else if(*Modbus_InputIO[CylSh02]&&(*Modbus_InputIO[CylRe02]==0))  //气缸舒张且没收缩
	{
		Cylinder_Data[2]=1;
	}
	else
	{
		Cylinder_Data[2]=2;
	}
}

/*------------------------------------------瓶  身--------------------------------------------------------------------*/
u8 TurntableConnect(void)
{
	if(Modbus_InputIO[TurTa01]==0)//转盘有信号
	{
		
	}
	else
	{
		return 10;//错误
	}
	return 0;
}

/*------------------------------------------內构件--------------------------------------------------------------------*/
///气缸1控制  內构

u8 CylinderConnect01(void)
{
	if(*Modbus_InputIO[CylSh01]==0&&(*Modbus_InputIO[CylRe01]))  			//气缸收缩且没舒张
	{
		Cylinder01=1;
	}
	else if(*Modbus_InputIO[CylSh01]&&(*Modbus_InputIO[CylRe01]==0))  //气缸舒张且没收缩
	{
		Cylinder01=0;
	}
	else
	{
		return 1;//故障
	}
	return 0;
}

///气缸2控制  內构

u8 CylinderConnect02(void)
{
	if(*Modbus_InputIO[CylSh02]==0&&(*Modbus_InputIO[CylRe02])) 		  //气缸收缩且没舒张为正常状态
	{
		Cylinder02=1;
	}
	else if(*Modbus_InputIO[CylSh02]&&(*Modbus_InputIO[CylRe02]==0))  //气缸舒张且没收缩
	{
		Cylinder02=0;
	}
	else
	{
		return 2;//故障
	}
	return 0;
}


//内构件下压控制
u32 Systime_cnt=0;
u8 CylinderStep_Flag=0,AllTime_Cnt=0;
u8 CylinderStepConnect(void)
{
	static u8 Step=0,cishujishu=0,ErrorTime_cnt=0,Light_cnt=0;
	static u32 systime=0;

	switch(Step)
	{
		case 0:
		{
			if(*Modbus_InputIO[Light03]==0)//光电3滤波
			{
				Light_cnt++;
			}
			else
			{
				Light_cnt=0;
			}
			if(Light_cnt>10)//光电传感器100ms内
			{			
				if(*Modbus_InputIO[CylSh02]==0&&(*Modbus_InputIO[CylRe02])) 		  //气缸2收缩且没舒张为正常状态
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
			if(*Modbus_InputIO[CylSh02]&&(*Modbus_InputIO[CylRe02]==0))  //气缸2舒张且没收缩
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
			if(*Modbus_InputIO[CylSh01]&&(*Modbus_InputIO[CylRe01]==0))  //气缸1舒张且没收缩
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
			if(*Modbus_InputIO[CylSh01]==0&&(*Modbus_InputIO[CylRe01]))  			//气缸1收缩且没舒张
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
			
			if(*Modbus_InputIO[CylSh02]==0&&(*Modbus_InputIO[CylRe02])) 		  //气缸2收缩且没舒张为正常状态
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
/*---------------------------------------------------故障报警-----------------------------------------------------------*/
void Wainmming(void)
{
	
}


/*---------------------------------------------------按钮扫描-----------------------------------------------------------*/
//按键扫描10ms进入一次
//返回按键值
u8 Key_Scan(void)
{
	u8 i;
	static u8 StarStop=0;
	for(i=1;i<CONNECT_LEN;i++)
	{
		if(Connect_Data[i])
		{
			return i;
		}
	}
	//启动按键状态不同了
	if(Connect_Data[StopStart]!=StarStop)
	{
		StarStop=Connect_Data[StopStart];
		return 0;
	}
	return 20;
}

/*---------------------------------------------------按钮总控制-----------------------------------------------------------*/
//按键总控制
void CylinderAllConnect(u8 key)//不支持连按  //10ms进入一次
{
	u8 Error=0;
	
	switch(key)
	{
		case StopStart: 	 //启动停机按钮
		{
			
			break;
		}
		case TimeOut:		 	 //暂停
		{
			Connect_Data[TimeOut]=0;
			break;
		}
		case CountClear: 	 //计数清零
		{
			
			Cylinder_Data[0]=0;//清除计数寄存器
			
			Connect_Data[CountClear]=0;
			break;
		}
		case ReSet: 		 	 //整机复位
		{
			Connect_Data[ReSet]=0;
			break;
		}
		case BottleM:		 	 //瓶身上料
		{
			Connect_Data[BottleM]=0;
			break;
		}
		case InternalM:  	 //內构上料
		{
			CylinderStep_Flag=1;
			
			Connect_Data[InternalM]=0;
			break;
		}
		case CapM:				 //瓶盖上料
		{
			Connect_Data[CapM]=0;
			break;
		}
		case InternaR_C: 	 //內构旋转气缸
		{
			Connect_Data[InternaR_C]=0;
			break;
		}
		case CapR_C:			 //瓶盖旋转气缸
		{
			Connect_Data[CapR_C]=0;
			break;
		}
		case ScrewCap:		 //拧瓶盖
		{
			Connect_Data[ScrewCap]=0;
			break;
		}
		case InternaPush_C://推料气缸控制-內构
		{
			
			Error=CylinderConnect02();
			
			Connect_Data[InternaPush_C]=0;
			break;
		}
		case ScrewPush_C:	 //推料气缸控制-瓶盖
		{
			Connect_Data[ScrewPush_C]=0;
			break;
		}
		case Turntable:		 //转盘控制
		{
			Connect_Data[Turntable]=0;
			break;
		}
		case InternalPM_C: //压料气缸控制-內构
		{
			
			Error=CylinderConnect01();
			
			Connect_Data[InternalPM_C]=0;
			break;
		}
		case CapPM_C:			 //压料气缸控制-瓶盖
		{
			Connect_Data[CapPM_C]=0;
			break;
		}
		default :break;
		
	}
	
	
	if(AllTime_Cnt)
	{
		AllTime_Cnt--;
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









