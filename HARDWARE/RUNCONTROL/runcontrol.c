#include "sys.h"
#include "include.h"
#include "runcontrol.h"
#include "RS485.h"
#include "stdlib.h"
#include <string.h>

//创建队列
sequeue_t * create_empty_sequeue(void)
{
	sequeue_t * sq;
	//sq = (sequeue_t *)malloc(sizeof(sequeue_t));
	sq->front = sq->rear = 0;
  return sq;
}
sequeue_t *ERROR_SQ;//定义队列

//队列初始化
void InitSequeuet(void)
{
	 ERROR_SQ=create_empty_sequeue();
}

//判断队列是否为空
u8 check_seqeue_empty(sequeue_t * sq)
{
  return (sq->front == sq->rear);
}

//入队
u8 enqueue(sequeue_t *sq ,u8 val)
{
	if(val)//如果数据不为0则入列
		{
			sq->rear = (sq->rear+1) % (N-1);     //循环存储
			sq->data[sq->rear] = val;
		}
 
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

/*------------------------------------------运行状态周期监测--------------------------------------------------------------------*/
void ClearAllError(void)
{
	memset(Status_Data,0,sizeof(Status_Data));//清除状态寄存器
}

//气缸运行状态监测100ms
void RunStatus(void)
{
	//气缸状态
	if((*Modbus_InputIO[CylSh01]==0)&&(*Modbus_InputIO[CylRe01]))  		//气缸收缩且没舒张
	{
		Cylinder_Data[1]=0;
	}
	else if(*Modbus_InputIO[CylSh01]&&(*Modbus_InputIO[CylRe01]==0))  //气缸舒张且没收缩
	{
		Cylinder_Data[1]=1;
	}
	else
	{
		Cylinder_Data[1]=2;
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
	
	if((*Modbus_InputIO[CylSh03]==0)&&(*Modbus_InputIO[CylRe03]))  		//气缸收缩且没舒张
	{
		Cylinder_Data[3]=0;
	}
	else if(*Modbus_InputIO[CylSh03]&&(*Modbus_InputIO[CylRe03]==0))  //气缸舒张且没收缩
	{
		Cylinder_Data[3]=1;
	}
	else
	{
		Cylinder_Data[3]=2;
	}
	
	if(*Modbus_InputIO[CylSh04]==0&&(*Modbus_InputIO[CylRe04])) 		  //气缸收缩且没舒张为正常状态
	{
		Cylinder_Data[4]=0;
	}
	else if(*Modbus_InputIO[CylSh04]&&(*Modbus_InputIO[CylRe04]==0))  //气缸舒张且没收缩
	{
		Cylinder_Data[4]=1;
	}
	else
	{
		Cylinder_Data[4]=2;
	}
	
	
	if(*Modbus_InputIO[Light01] | *Modbus_InputIO[Light02])//料槽状态1
	{
		Status_Data[3]=1;
	}
	
	if(*Modbus_InputIO[Light03])
	{
		Status_Data[4]=1;
	}
	
	if(*Modbus_InputIO[Light04])
	{
		Status_Data[5]=1;
	}
	
	
}

/*------------------------------------------瓶  身--------------------------------------------------------------------*/
u8 Turntable_step=0;//转盘步骤
static u8 TurntableConnect(void)//转盘控制
{
	static u8 ErrorTime_cnt=0;
	
	switch(Turntable_step)
	{
		case 0:
		{
			
			break;
		}
		case 1:
		{
			if(Modbus_InputIO[TurTa01]==0)//转盘有信号
			{
				Turntable01=1;//启动转盘
				Turntable_step=2;
			}
			else
			{
				return 10;//error
			}
			break;
		}
		case 2:
		{
			if(Modbus_InputIO[TurTa01])//转盘没信号no signal说明转盘转动了
			{
				Turntable_step=3;
				ErrorTime_cnt=0;
			}
			else 
			{
				ErrorTime_cnt++;
			}
			
			break;
		}
		case 3:
		{
			if(Modbus_InputIO[TurTa01]==0)//hanve siganl
			{
				Turntable01=0;//close
				Turntable_step=0;
				ErrorTime_cnt=0;
			}
			else
			{
				ErrorTime_cnt++;
			}
			break;
		}
	}
	if(ErrorTime_cnt>100)//timeout
	{
		return 10;//
	}
	return 0;//have no error
	
}

/*------------------------------------------內构件--------------------------------------------------------------------*/
///气缸1控制  內构
u8 CylinderStep=0;//内构件下压步骤


static u8 CylinderConnect01(void)
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

static u8 CylinderConnect02(void)
{
	if(*Modbus_InputIO[CylSh02]==0&&(*Modbus_InputIO[CylRe02])) 		  //气缸收缩且没舒张为正常状态
	{
		Cylinder02=0;
	}
	else if(*Modbus_InputIO[CylSh02]&&(*Modbus_InputIO[CylRe02]==0))  //气缸舒张且没收缩
	{
		Cylinder02=1;
	}
	else
	{
		return 2;//故障
	}
	return 0;
}


//内构件下压控制
u32 Systime_cnt=0;
u8 AllTime_Cnt=0;
static u8 CylinderStepControl(void)
{
	static u8  ErrorTime_cnt=0,Light_cnt=0,Error=0,flag=0,cishucnt=0;
	static u32 systime=0;

	switch(CylinderStep)
	{
		case 0: break;
		case 1://气缸2收缩
		{
			if(*Modbus_InputIO[CylRe02]==0&&(*Modbus_InputIO[CylSh02])) 		  //气缸2舒张且没收缩为正常状态
			{
				systime=Systime_cnt;//取时间
				Cylinder02=1;//气缸2收缩
				ErrorTime_cnt=0;
				CylinderStep=2;
				flag=1;
			}
			else
			{
				Error=2;
				ErrorTime_cnt++;//动作超时计数
			}
			break;
		}
	  case 2://光电检测有料则气缸2推料
		{
			
			if(*Modbus_InputIO[CylSh02]==0&&(*Modbus_InputIO[CylRe02])&&flag) 		  //气缸1收缩且没舒张为正常状态
			{
				flag=0;
			}
			
			if(*Modbus_InputIO[Light03]==0)//光电3滤波
			{
				Light_cnt++;
			}
			else
			{
				Light_cnt=0;
			}
			//if(Light_cnt>3)//光电传感器100ms内
			if(*Modbus_InputIO[Light03]==0)//光电3滤波
			{	
				if(*Modbus_InputIO[CylSh02]==0&&(*Modbus_InputIO[CylRe02])) 		  //气缸1收缩且没舒张为正常状态
				{
					Light_cnt =0;
					Cylinder02=0;//气缸2舒张
					ErrorTime_cnt=0;
					CylinderStep=3;
				}
				else
				{
					Error=2;
					ErrorTime_cnt++;//动作超时计数
				}
			}
			break;
		}
		
		case 3://气缸1压料
		{
			if(*Modbus_InputIO[CylRe02]==0&&(*Modbus_InputIO[CylSh02])) 		  //气缸2舒张且没收缩为正常状态
			{
				if(*Modbus_InputIO[CylSh01]==0&&(*Modbus_InputIO[CylRe01])) 		  //气缸收缩且没舒张为正常状态
				{
					Cylinder01=1;
					ErrorTime_cnt=0;
					CylinderStep=4;
				}
				else
				{
					Error=1;
					ErrorTime_cnt++;//动作超时计数
				}
			}
			else
			{
				Error=2;
				ErrorTime_cnt++;//动作超时计数
			}
			break;
		}
		case 4://气缸1收回
		{
			if(*Modbus_InputIO[CylRe01]==0&&(*Modbus_InputIO[CylSh01]))  //气缸舒张且没收缩
			{
				Cylinder01=0;
				ErrorTime_cnt=0;
				CylinderStep=5;
			}
			else
			{
				Error=1;
				ErrorTime_cnt++;//动作超时计数
			}
			break;
		}
		case 5://气缸1收回完成
		{
			if(*Modbus_InputIO[CylSh01]==0&&(*Modbus_InputIO[CylRe01])) 		  //气缸收缩且没舒张为正常状态
			{
				ErrorTime_cnt=0;
				CylinderStep=0;//步骤清零
				Cylinder_Data[0]=Systime_cnt-systime;//线用次数来显示时间
				cishucnt++;
				printf("次数: %d      时间:%d\n",cishucnt,Cylinder_Data[0]);
			}
			else
			{
				Error=1;
				ErrorTime_cnt++;//动作超时计数
			}
			break;
		}
	}	
	if(ErrorTime_cnt>100)//out 1s
	{
		return Error;//返回错误气缸号
	}
	return 0;
}
/*---------------------------------------------------瓶盖控制-----------------------------------------------------------*/
u8 CapCylinderStep=0;//瓶盖下压步骤
///气缸4控制  瓶盖
static u8 CylinderConnect04(void)
{
	if(*Modbus_InputIO[CylSh04]==0&&(*Modbus_InputIO[CylRe04]))  			//气缸收缩且没舒张
	{
		Cylinder04=1;
	}
	else if(*Modbus_InputIO[CylSh04]&&(*Modbus_InputIO[CylRe04]==0))  //气缸舒张且没收缩
	{
		Cylinder04=0;
	}
	else
	{
		return 4;//故障
	}
	return 0;
}

///气缸5控制  瓶盖
static u8 CylinderConnect05(void)
{
	if(*Modbus_InputIO[CylSh05]==0&&(*Modbus_InputIO[CylRe05]))  			//气缸收缩且没舒张
	{
		Cylinder05=1;
	}
	else if(*Modbus_InputIO[CylSh05]&&(*Modbus_InputIO[CylRe05]==0))  //气缸舒张且没收缩
	{
		Cylinder05=0;
	}
	else
	{
		return 5;//故障
	}
	return 0;
}

///气缸6控制  瓶盖
static u8 CylinderConnect06(void)
{
	if(*Modbus_InputIO[CylSh06]==0&&(*Modbus_InputIO[CylRe06]))  			//气缸收缩且没舒张
	{
		Cylinder06=1;
	}
	else if(*Modbus_InputIO[CylSh06]&&(*Modbus_InputIO[CylRe06]==0))  //气缸舒张且没收缩
	{
		Cylinder06=0;
	}
	else
	{
		return 6;//故障
	}
	return 0;
}

///瓶盖下压控制
static u8 CapCylinderControl(void)
{
	static u8 ErrorTime_cnt=0,Error=0;
	static u32 systime=0;
	
	switch(CapCylinderStep)
	{
		case 0: break;
		case 1:  //下压
		{
			if(*Modbus_InputIO[CylSh05]==0&&(*Modbus_InputIO[CylRe05]))  			//气缸收缩且没舒张
			{
				Cylinder05=1;
				CapCylinderStep=2;
				ErrorTime_cnt=0;
				systime=Systime_cnt;//取时间
			}
			else
			{
				Error=5;
				ErrorTime_cnt++;
			}
			break;
		}
		case 2://压到底夹紧
		{
			if(*Modbus_InputIO[CylRe05]==0&&*Modbus_InputIO[CylSh05])
			{
				if(*Modbus_InputIO[CylSh06]==0&&(*Modbus_InputIO[CylRe06]))  			//气缸收缩且没舒张
				{
					Cylinder06=1;
					CapCylinderStep=3;
					ErrorTime_cnt=0;
					//Cylinder_Data[0]=Systime_cnt-systime;//线用次数来显示时间
				}
				else
				{
					Error=6;
					ErrorTime_cnt++;
				}
			}
			else
			{
				Error=5;
				ErrorTime_cnt++;
			}
			break;
		}
		case 3://夹紧了上升
		{
			if(*Modbus_InputIO[CylRe06]==0&&*Modbus_InputIO[CylSh06])
			{
				if(*Modbus_InputIO[CylSh05]&&(*Modbus_InputIO[CylRe05]==0))  //气缸舒张且没收缩
				{
					Cylinder05=0;
					CapCylinderStep=4;
					ErrorTime_cnt=0;
				}
				else
				{
					Error=5;
					ErrorTime_cnt++;
				}
			}
			else
			{
				Error=6;
				ErrorTime_cnt++;
			}
			break;
		}
		case 4://上到顶旋转
		{
			if(*Modbus_InputIO[CylSh05]==0&&*Modbus_InputIO[CylRe05])
			{
				if(*Modbus_InputIO[CylSh04]==0&&(*Modbus_InputIO[CylRe04]))  			//气缸收缩且没舒张
				{
					Cylinder04=1;
					CapCylinderStep=5;
					ErrorTime_cnt=0;
				}
				else
				{
					Error=4;
					ErrorTime_cnt++;
				}
			}
			else
			{
				Error=5;
				ErrorTime_cnt++;
			}
			break;
		}
		case 5://旋转完了下降
		{
			if(*Modbus_InputIO[CylRe04]==0&&*Modbus_InputIO[CylSh04])
			{
				if(*Modbus_InputIO[CylSh05]==0&&(*Modbus_InputIO[CylRe05]))  			//气缸收缩且没舒张
				{
					Cylinder05=1;
					CapCylinderStep=6;
					ErrorTime_cnt=0;
				}
				else
				{
					Error=5;
					ErrorTime_cnt++;
				}
			}
			else
			{
				Error=4;
				ErrorTime_cnt++;
			}
			break;
		}
		case 6://下到底松开
		{
			if(*Modbus_InputIO[CylRe05]==0&&*Modbus_InputIO[CylSh05])
			{
				if(*Modbus_InputIO[CylSh06]&&(*Modbus_InputIO[CylRe06]==0))  //气缸舒张且没收缩
				{
					Cylinder06=0;
					CapCylinderStep=7;
					ErrorTime_cnt=0;
				}
				else
				{
					Error=6;
					ErrorTime_cnt++;
				}
			}
			else
			{
				Error=5;
				ErrorTime_cnt++;
			}
			break;
		}
		case 7://松开完了上升
		{
			if(*Modbus_InputIO[CylSh06]==0&&*Modbus_InputIO[CylRe06])
			{
				if(*Modbus_InputIO[CylSh05]&&(*Modbus_InputIO[CylRe05]==0))  //气缸舒张且没收缩
				{
					Cylinder05=0;
					CapCylinderStep=8;
					ErrorTime_cnt=0;
				}
				else
				{
					Error=5;
					ErrorTime_cnt++;
				}
			}
			else
			{
				Error=6;
				ErrorTime_cnt++;
			}
			break;
		}
		case 8://上升完了旋转
		{
			if(*Modbus_InputIO[CylSh05]==0&&*Modbus_InputIO[CylRe05])
			{
				if(*Modbus_InputIO[CylSh04]&&(*Modbus_InputIO[CylRe04]==0))  //气缸舒张且没收缩
				{
					Cylinder04=0;
					CapCylinderStep=9;
					ErrorTime_cnt=0;
				}
				else
				{
					Error=4;
					ErrorTime_cnt++;
				}
			}
			else
			{
				Error=5;
				ErrorTime_cnt++;
			}
			break;
		}
		case 9://旋转完了
		{
			if(*Modbus_InputIO[CylSh04]==0&&*Modbus_InputIO[CylRe04])
			{
				CapCylinderStep=0;
				ErrorTime_cnt=0;
				Cylinder_Data[0]=Systime_cnt-systime;//线用次数来显示时间
			}
			else
			{
				Error=4;
				ErrorTime_cnt++;
			}
			break;
		}
		default:break;
	}
	if(ErrorTime_cnt>100)//out 1s
	{
		return Error;//返回错误气缸号
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
static u8 Key_Scan(void)
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

/*---------------------------------------------------按钮控制-----------------------------------------------------------*/
//按键总控制
static u8 CylinderAllConnect(u8 key)//不支持连按  //10ms进入一次
{
	u8 Error=0;
	switch(key)
	{
		case StopStart: 	 //启动停机按钮
		{
			ClearAllError();//清除所有错误
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
			
			ClearAllError();//清除所有错误
			
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
			if(CylinderStep==0)//空闲步骤才有效
			{
				CylinderStep=1;
			}
			
			Connect_Data[InternalM]=0;
			break;
		}
		case CapM:				 //瓶盖上料
		{
			if(CapCylinderStep==0)
			{
				CapCylinderStep=1;//空闲步骤才有效
			}
				
			Connect_Data[CapM]=0;
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
			
			Error=CylinderConnect05();
			
			Connect_Data[ScrewPush_C]=0;
			break;
		}
		case Turntable:		 //转盘控制
		{
			if(Turntable_step==0)//在空闲步骤下才起效
			{
				Turntable_step=1;
			}
			
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
			
			Error=CylinderConnect06();
			
			Connect_Data[CapPM_C]=0;
			break;
		}
		default :break;
		
	}
	
	return Error;
	
}

/*---------------------------------------------------总控制-----------------------------------------------------------*/
void ALLControl_10ms(void)
{
	u8 ERROR=0;
	
	if(AllTime_Cnt)
	{
		AllTime_Cnt--;
	}
	
	ERROR=CylinderAllConnect(Key_Scan());//key control
	enqueue(ERROR_SQ,ERROR);//入列
	ERROR=CylinderStepControl();//内构件下压控制
	enqueue(ERROR_SQ,ERROR);//入列
	ERROR=TurntableConnect();//转盘控制
	enqueue(ERROR_SQ,ERROR);//入列
//	ERROR=CapCylinderControl();//瓶盖下压控制
//	enqueue(ERROR_SQ,ERROR);//入列
	
	if(!check_seqeue_empty(ERROR_SQ))//错误队列不为空则报错
	{
		ERROR=dequeue(ERROR_SQ);//出列
		printf("气缸%d--发生故障\n",ERROR);
	}
}









