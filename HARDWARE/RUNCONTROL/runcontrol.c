#include "sys.h"
#include "include.h"
#include "runcontrol.h"
#include "RS485.h"
#include "stdlib.h"
#include <string.h>

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
u8 StopStart_Flag=0;//整机启动标志

static void Wainmming(void);
/*------------------------------------------运行状态周期监测--------------------------------------------------------------------*/

//气缸运行状态监测100ms
void RunStatus(void)
{
	Wainmming();
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
			if(*Modbus_InputIO[TurTa01]!=0)//转盘有信号
			{
				Turntable01=1;//启动转盘
				Turntable_step=2;
			}
			else
			{
				Turntable_step=2;
				Turntable01=1;//启动转盘
				ErrorTime_cnt++;
			}
			break;
		}
		case 2:
		{
			if(*Modbus_InputIO[TurTa01]==0)//转盘没信号no signal说明转盘转动了
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
			if(*Modbus_InputIO[TurTa01]!=0)//hanve siganl
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
		ErrorTime_cnt=0;
		Turntable01=0;//close
		Turntable_step=20;//错误
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
	static u8  ErrorTime_cnt=0,Light_cnt=0,Error=0,cishucnt=0;
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
			if(StopStart_Flag) //启动模式
			{
				if(*Modbus_InputIO[CylRe02]==0&&(*Modbus_InputIO[CylSh02])&&Turntable_step==0) //气缸2舒张且没收缩为正常状态且转盘转动完成
				{
					if(*Modbus_InputIO[CylSh01]==0&&(*Modbus_InputIO[CylRe01])) 		 						 //气缸收缩且没舒张为正常状态
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
			}
			else//测试模式
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
		CylinderStep=20;//错误
		return Error;//返回错误气缸号
	}
	return 0;
}
/*---------------------------------------------------瓶盖控制-----------------------------------------------------------*/
u8 CapCylinderStep=0;//瓶盖下压步骤
///气缸3控制  瓶盖
static u8 CylinderConnect03(void)
{
	if(*Modbus_InputIO[CylSh03]==0&&(*Modbus_InputIO[CylRe03]))  			//气缸收缩且没舒张
	{
		Cylinder03=1;
	}
	else if(*Modbus_InputIO[CylSh03]&&(*Modbus_InputIO[CylRe03]==0))  //气缸舒张且没收缩
	{
		Cylinder03=0;
	}
	else
	{
		return 3;//故障
	}
	return 0;
}

///气缸4控制  瓶盖

static u8 CylinderConnect04(void)
{
	if(*Modbus_InputIO[CylSh04]==0&&(*Modbus_InputIO[CylRe04])) 		  //气缸收缩且没舒张为正常状态
	{
		Cylinder04=0;
	}
	else if(*Modbus_InputIO[CylSh04]&&(*Modbus_InputIO[CylRe04]==0))  //气缸舒张且没收缩
	{
		Cylinder04=1;
	}
	else
	{
		return 3;//故障
	}
	return 0;
}


///瓶盖下压控制
static u8 CapCylinderControl(void)
{
	static u8  ErrorTime_cnt=0,Light_cnt=0,Error=0,cishucnt=0;
	static u32 systime=0;

	switch(CapCylinderStep)
	{
		case 0: break;
		case 1://气缸4收缩
		{
			if(*Modbus_InputIO[CylRe04]==0&&(*Modbus_InputIO[CylSh04])) 		  //气缸2舒张且没收缩为正常状态
			{
				systime=Systime_cnt;//取时间
				Cylinder04=1;//气缸2收缩
				ErrorTime_cnt=0;
				CapCylinderStep=2;
			}
			else
			{
				Error=4;
				ErrorTime_cnt++;//动作超时计数
			}
			break;
		}
	  case 2://光电检测有料则气缸4推料
		{
						
			if(*Modbus_InputIO[Light04]==0)//光电3滤波
			{
				Light_cnt++;
			}
			else
			{
				Light_cnt=0;
			}
			//if(Light_cnt>3)//光电传感器100ms内
			if(*Modbus_InputIO[Light04]==0)//光电3滤波
			{	
				if(*Modbus_InputIO[CylSh04]==0&&(*Modbus_InputIO[CylRe04])) 		  //气缸1收缩且没舒张为正常状态
				{
					Light_cnt =0;
					Cylinder04=0;//气缸2舒张
					ErrorTime_cnt=0;
					CapCylinderStep=3;
				}
				else
				{
					Error=4;
					ErrorTime_cnt++;//动作超时计数
				}
			}
			break;
		}
		
		case 3://气缸3压料
		{
			if(StopStart_Flag)//运行状态
			{
				if(*Modbus_InputIO[CylRe04]==0&&(*Modbus_InputIO[CylSh04])&&Turntable_step==0) 		  //气缸4舒张且没收缩为正常状态和转盘转动完成
				{
					if(*Modbus_InputIO[CylSh03]==0&&(*Modbus_InputIO[CylRe03])) 		  //气缸收缩且没舒张为正常状态
					{
						Cylinder03=1;
						ErrorTime_cnt=0;
						CapCylinderStep=4;
					}
					else
					{
						Error=3;
						ErrorTime_cnt++;//动作超时计数
					}
				}
				else
				{
					Error=4;
					ErrorTime_cnt++;//动作超时计数
				}
			}
			else  //调试
			{
				if(*Modbus_InputIO[CylRe04]==0&&(*Modbus_InputIO[CylSh04])) 		  //气缸4舒张且没收缩为正常状态
				{
					if(*Modbus_InputIO[CylSh03]==0&&(*Modbus_InputIO[CylRe03])) 		  //气缸收缩且没舒张为正常状态
					{
						Cylinder03=1;
						ErrorTime_cnt=0;
						CapCylinderStep=4;
					}
					else
					{
						Error=3;
						ErrorTime_cnt++;//动作超时计数
					}
				}
				else
				{
					Error=4;
					ErrorTime_cnt++;//动作超时计数
				}
			}
			break;
		}
		case 4://气缸3收回
		{
			if(*Modbus_InputIO[CylRe03]==0&&(*Modbus_InputIO[CylSh03]))  //气缸舒张且没收缩
			{
				Cylinder03=0;
				ErrorTime_cnt=0;
				CapCylinderStep=5;
			}
			else
			{
				Error=3;
				ErrorTime_cnt++;//动作超时计数
			}
			break;
		}
		case 5://气缸3收回完成
		{
			if(*Modbus_InputIO[CylSh03]==0&&(*Modbus_InputIO[CylRe03])) 		  //气缸收缩且没舒张为正常状态
			{
				ErrorTime_cnt=0;
				CapCylinderStep=0;//步骤清零
				Cylinder_Data[0]=Systime_cnt-systime;//线用次数来显示时间
				cishucnt++;
				printf("次数: %d      时间:%d\n",cishucnt,Cylinder_Data[0]);
			}
			else
			{
				Error=3;
				ErrorTime_cnt++;//动作超时计数
			}
			break;
		}
	}	
	if(ErrorTime_cnt>100)//out 1s
	{
		CapCylinderStep=20;//错误
		return Error;//返回错误气缸号
	}
	return 0;
}

/*---------------------------------------------------拧瓶控制-----------------------------------------------------------*/
void ScrewCapControl01(u8 mode)
{
	Screw_Cap01=mode;
}


/*---------------------------------------------------故障报警-----------------------------------------------------------*/
u8 All_ERROR=0;
void Wainmming(void)
{

	if(All_ERROR)
	{
		switch(All_ERROR)
		{
			case 1:
			{
				Status_Data[0]=1;//一号气缸
				break;
			}
			case 2:
			{
				Status_Data[0]=1;//一号气缸
				break;
			}
			case 3:
			{
				Status_Data[1]=1;//二号气缸
				break;
			}
			case 4:
			{
				Status_Data[1]=1;//二号气缸
				break;
			}
			case 5:
			{
				break;
			}
			case 6:
			{
				break;
			}
			case 7:
			{
				break;
			}
			default :break;
		}
		
		Status_Data[6]=1;//整机运行状态
		StopStart_Flag=0;//启动失能
		printf("气缸%d--发生故障\n",ERROR);
	}
	
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


/*---------------------------------------------------整机复位-----------------------------------------------------------*/
static void AllReSet(void)
{
	memset(Status_Data,0,sizeof(Status_Data));//清除状态寄存器ClearAllError
	StopStart_Flag=0;
	Turntable_step=0;//转盘
	CylinderStep=0;  //内构
	
	//Status_Data[6]=1;//整机运行状态
	All_ERROR=0;
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
			StopStart_Flag=Connect_Data[StopStart];
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
			AllReSet();
			
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
			if(CylinderStep==0&&StopStart_Flag==0)//空闲步骤且不在启动状态才有效
			{
				CylinderStep=1;
			}
			
			Connect_Data[InternalM]=0;
			break;
		}
		case CapM:				 //瓶盖上料
		{
			if(CapCylinderStep==0&&StopStart_Flag==0)
			{
				CapCylinderStep=1;//空闲步骤才有效
			}
				
			Connect_Data[CapM]=0;
			break;
		}
		case ScrewCap:		 //拧瓶盖
		{
			if(StopStart_Flag==0)
			{
				ScrewCapControl01(1);
			}
			Connect_Data[ScrewCap]=0;
			break;
		}
		case InternaPush_C://推料气缸控制-內构
		{
			if(StopStart_Flag==0)
			{
				Error=CylinderConnect02();
			}
			
			Connect_Data[InternaPush_C]=0;
			break;
		}
		case ScrewPush_C:	 //推料气缸控制-瓶盖
		{
			if(StopStart_Flag==0)
			{
				Error=CylinderConnect04();
			}
			
			Connect_Data[ScrewPush_C]=0;
			break;
		}
		case Turntable:		 //转盘控制
		{
			if(Turntable_step==0&&StopStart_Flag==0)//在空闲步骤下才起效
			{
				Turntable_step=1;
			}
			
			Connect_Data[Turntable]=0;
			break;
		}
		case InternalPM_C: //压料气缸控制-內构
		{
			if(StopStart_Flag==0)
			{
				Error=CylinderConnect01();
			}
			
			Connect_Data[InternalPM_C]=0;
			break;
		}
		case CapPM_C:			 //压料气缸控制-瓶盖
		{
			if(StopStart_Flag==0)
			{
				Error=CylinderConnect03();
			}
			
			Connect_Data[CapPM_C]=0;
			break;
		}
		default :ScrewCapControl01(0);break;
		
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
	if(ERROR)
	{
		All_ERROR=ERROR;
	}
	
	ERROR=CylinderStepControl();//内构件下压控制
	if(ERROR)
	{
		All_ERROR=ERROR;
	}
	
	ERROR=TurntableConnect();//转盘控制
	if(ERROR)
	{
		All_ERROR=ERROR;
	}
	
	ERROR=CapCylinderControl();//瓶盖下压控制
	if(ERROR)
	{
		All_ERROR=ERROR;
	}
	
	//机器启动
	if(StopStart_Flag)
	{
		if(Turntable_step==0&&CylinderStep==0)//转盘转动完成，且內构下压完成
		{
			Turntable_step=1;//转盘转动
		}
		
		if(CylinderStep==0)//內构下压完成
		{
			CylinderStep=1;  //內构下压
		}
	}
	
}









