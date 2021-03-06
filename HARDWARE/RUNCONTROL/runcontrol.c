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
u32  Products_Cnt=0;//产品生产数量计数
u8 StopStart_Flag=0;//整机启动标志
u8 FiestStart_Flag=0;//初次启动标志
u8 All_ERROR=0;//总错误位
static void Wainmming(void);
/*------------------------------------------运行状态周期监测--------------------------------------------------------------------*/

//气缸运行状态监测100ms
void RunStatus(void)
{
	Wainmming();
	//气缸状态
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
		Cylinder_Data[4]=0;
	}
	else if(*Modbus_InputIO[CylSh02]&&(*Modbus_InputIO[CylRe02]==0))  //气缸舒张且没收缩
	{
		Cylinder_Data[4]=1;
	}
	else
	{
		Cylinder_Data[4]=2;
	}
	
	if((*Modbus_InputIO[CylSh03]==0)&&(*Modbus_InputIO[CylRe03]))  		//气缸收缩且没舒张
	{
		Cylinder_Data[5]=0;
	}
	else if(*Modbus_InputIO[CylSh03]&&(*Modbus_InputIO[CylRe03]==0))  //气缸舒张且没收缩
	{
		Cylinder_Data[5]=1;
	}
	else
	{
		Cylinder_Data[5]=2;
	}
	
	if(*Modbus_InputIO[CylSh04]==0&&(*Modbus_InputIO[CylRe04])) 		  //气缸收缩且没舒张为正常状态
	{
		Cylinder_Data[6]=0;
	}
	else if(*Modbus_InputIO[CylSh04]&&(*Modbus_InputIO[CylRe04]==0))  //气缸舒张且没收缩
	{
		Cylinder_Data[6]=1;
	}
	else
	{
		Cylinder_Data[6]=2;
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
	
	//吹气控制
	if(StopStart_Flag)//启动则开吹起
	{
		AllPower=1;//开总电
		Cylinder06=0;
		TurnOnLedGreen();
	}
	else							//暂停关吹气
	{
		if(All_ERROR==0)//暂停
		{
			TurnOnLedYellow();
		}
		AllPower=0;//关总电
		Cylinder06=1;
	}
	
}

/*------------------------------------------瓶  身--------------------------------------------------------------------*/
u8 Turntable_step=0;//转盘步骤
u8 BottleM_Flag=0;  //瓶身上料
u8 TurntableConnect_Flag=0;
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
			if(StopStart_Flag||BottleM_Flag)//运行状态 //光电一有信号
			{
				if(*Modbus_InputIO[Light01]==0)//test
				{
					BottleM_Flag=0;//瓶身上料
					Turntable01=1;//启动转盘
					Turntable_step=2;
					ErrorTime_cnt++;
				}
	  	}
			else if(TurntableConnect_Flag) //控制状态
			{
				TurntableConnect_Flag=0;
				Turntable01=1;//启动转盘
				Turntable_step=2;
				ErrorTime_cnt++;
			}
			break;
		}
		case 2:
		{
			if(*Modbus_InputIO[TurTa01]==0)//转盘信号no signal说明转盘转动了
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
			if(*Modbus_InputIO[TurTa01])//hanve no siganl
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
	if(ErrorTime_cnt>200)//timeout
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
static u8 CylinderStepControl(void)
{
	static u8  ErrorTime_cnt=0,Error=0;

	switch(CylinderStep)
	{
		case 0: break;
		case 1://气缸2收缩
		{
			if(*Modbus_InputIO[CylRe02]==0&&(*Modbus_InputIO[CylSh02])) 		  //气缸2舒张且没收缩为正常状态
			{
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
						
			if(*Modbus_InputIO[Light03]==0)//光电3有信号//test
			{	
				if(*Modbus_InputIO[CylSh02]==0&&(*Modbus_InputIO[CylRe02])) 		  //气缸1收缩且没舒张为正常状态
				{
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
				else//等待转盘转动
				{
//					Error=2;
//					ErrorTime_cnt++;//动作超时计数
				}
			}
			else if(Turntable_step==0)//测试模式  Turntable_step==0解决暂停自动压料问题，转盘没转不能压料，不然会压爆；
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
		ErrorTime_cnt=0;
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
u8 Light04_Flag=0;
static u8 CapCylinderControl(void)
{
	static u8  ErrorTime_cnt=0,Error=0;

	switch(CapCylinderStep)
	{
		case 0: break;
		case 1://气缸4收缩
		{
			if(*Modbus_InputIO[CylRe04]==0&&(*Modbus_InputIO[CylSh04])) 		  //气缸4舒张且没收缩为正常状态
			{
				Cylinder04=1;//气缸4收缩
				ErrorTime_cnt=0;
				CapCylinderStep=2;
				Light04_Flag =0x01;
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
						
			if(Light04_Flag&0x02)//光电4滤波test
			{	
				if(*Modbus_InputIO[CylSh04]==0&&(*Modbus_InputIO[CylRe04])) 		  //气缸4收缩且没舒张为正常状态
				{
					Cylinder04=0;//气缸4舒张
					ErrorTime_cnt=0;
					CapCylinderStep=3;
					Light04_Flag =0;
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
//					Error=4;
//					ErrorTime_cnt++;//动作超时计数
				}
			}
			else if(Turntable_step==0)//测试模式  Turntable_step==0解决暂停自动压料问题，转盘没转不能压料，不然会压爆；//调试
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
			}
			else
			{
				Error=3;
				ErrorTime_cnt++;//动作超时计数
			}
			break;
		}
	}	
	if(ErrorTime_cnt>200)//out 1s
	{
		ErrorTime_cnt=0;
		CapCylinderStep=20;//错误
		Light04_Flag =0;
		return Error;//返回错误气缸号
	}
	return 0;
}

/*---------------------------------------------------拧瓶控制-----------------------------------------------------------*/
u8 ScrewCapStep=0;//拧瓶步骤

void ScrewCapControl01()
{
	Screw_Cap01=~Screw_Cap01;
}

static u8 ScrewCapControl()
{
	static u8 ErrorTime_cnt=0;
	static u8 worktime=0;
	switch(ScrewCapStep)
	{
		case 0: break;
		case 1:
		{
			if(*Modbus_InputIO[CylSh05]==0&&(*Modbus_InputIO[CylRe05]))//气缸5收缩且没舒张
			{
				Screw_Cap01=1;//拧瓶
				ScrewCapStep=2;
				ErrorTime_cnt=0;
			}
			else
			{
				ErrorTime_cnt++;
			}
			break;
		}
		case 2:
		{
			if(*Modbus_InputIO[CylRe05]==0&&*Modbus_InputIO[CylSh05])//气缸5舒张没收缩
			{
				worktime++;
				if(worktime>SCREWCAPTIME)//拧瓶时间达到
				{
					ErrorTime_cnt=0;
					worktime=0;
					Screw_Cap01=0;//松瓶
					ScrewCapStep=3;
				}
	  	}
			else
			{
				ErrorTime_cnt++;
			}
			break;
		}
		case 3:
		{
			if(*Modbus_InputIO[CylSh05]==0&&*Modbus_InputIO[CylRe05])//气缸5收缩且没舒张
			{
				ScrewCapStep=0;//拧瓶完成
				ErrorTime_cnt=0;
			}
			else
			{
				ErrorTime_cnt++;
			}
			break;
		}
		default :break;
	}
	if(ErrorTime_cnt>200)//out 1s
	{
		ErrorTime_cnt=0;
		ScrewCapStep=20;//错误
		return 5;//返回错误气缸号
	}
	return 0;
}



/*---------------------------------------------------故障报警-----------------------------------------------------------*/
u8 AllReSet_Flag=0;

//100ms
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
				Status_Data[2]=1;//三号气缸
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
		//赋值错误编号
		Cylinder_Data[7]=All_ERROR;
		//红灯闪烁
		TurnOnLedRed();
		//停转盘
	  TurntableConnect_Flag=0;//转盘控制标志
		AllReSet_Flag=0;
		Turntable01=0;
		
		Status_Data[6]=1;//整机运行状态
		StopStart_Flag=0;//启动失能
		Connect_Data[StopStart]=0;//启动按钮失能
		printf("气缸%d--发生故障\n",ERROR);
		
	}
	
}


/*---------------------------------------------------按钮扫描-----------------------------------------------------------*/
static void AllReSet(void);
//按键扫描10ms进入一次
//返回按键值
static u8 Key_Scan(void)
{
	u8 i;
	static u8 StarStop=0,All_ERROR_cnt=0,AllReSet_cnt=0,Start_cnt=0,Stop_cnt=0;
	
	if(PBin(10))		//急停是反的,滤波
	{
		All_ERROR_cnt++;
		if(All_ERROR_cnt>20)
		{
			All_ERROR_cnt=20;
			All_ERROR=13;//急停
		}
	}
	else
	{
		All_ERROR_cnt=0;
	}
	
	
	if(PEin(10)==0&&AllReSet_Flag==0) 
	{
		Start_cnt++;
		if(Start_cnt>20)//滤波
		{
			Start_cnt=20;
			Stop_cnt=0;
			Connect_Data[StopStart]=1; 	 //启动  按键按下为低电平
		}	
	}
	else if(PEin(12)==0)
	{
		Stop_cnt++;
		if(Stop_cnt>20)//滤波
		{
			Start_cnt=0;
			Stop_cnt=20;
			Connect_Data[StopStart]=0; 	 //停机
		}
	}
	else
	{
		Start_cnt=0;
		Stop_cnt=0;
	}
	
	if(PEin(14)==0)  //关机|复位
	{
//		Connect_Data[ReSet]=1;
		AllReSet_cnt++;
		if(AllReSet_cnt>20)//滤波
		{
			AllReSet_cnt=20;
			if(AllReSet_Flag==0)
			{
				AllReSet();
			}
			else
			{
				AllReSet_cnt=0;
			}
		}
	}
	else
	{
		AllReSet_cnt=0;
	}
	
	for(i=1;i<CONNECT_LEN;i++)
	{
		if(Connect_Data[i])
		{
			return i;
		}
	}
	
	//启动按键状态不同了
	if(Connect_Data[StopStart]!=StarStop&&AllReSet_Flag==0)//复位结束才能启动
	{
		StarStop=Connect_Data[StopStart];
		return 0;
	}
	return 20;
}


/*---------------------------------------------------整机复位-----------------------------------------------------------*/
//u8 AllReSet_Flag=0;
u8 TurnTableTime=0;//复位转盘转动次数
void AllReSet(void)
{
	memset(Status_Data,0,sizeof(Status_Data));//清除状态寄存器ClearAllError
	/*********复位****************/
	Cylinder01=0;			//气缸01
	Cylinder02=0;			//气缸02
	Cylinder03=0;			//气缸03
	Cylinder04=0;			//气缸04
	Cylinder05=0;			//气缸05
	Cylinder06=0;			//气缸06
	Screw_Cap01=0;		//拧瓶
	Turntable01=0;		//转盘
	StopStart_Flag=0; //启动标志
	
	TurnTableTime=0;
	Turntable_step=0; //转盘
	TurntableConnect_Flag=0;//转盘控制标志
	
	CylinderStep=0;   //内构
	CapCylinderStep=0;//瓶盖
	ScrewCapStep=0;   //拧瓶
	
	Cylinder_Data[7]=0;//清除错误
	All_ERROR=0;
	Connect_Data[StopStart]=0;//启动按钮失能
	FiestStart_Flag=0;//初次启动标志清除
	/********判断机械位置********************/
	
	AllReSet_Flag=1;
	
	
}	


static u8 AllReSetstep(void)
{
	static u8  ErrorTime_cnt=0;
	static u8 AllReSet_cnt=0;
	static u8 ClearStep=0;//清料步骤
	if(AllReSet_Flag==0x01)
	{
		AllReSet_cnt++;
		if(AllReSet_cnt>50)//500ms
		{
			AllReSet_Flag=0;
			AllReSet_cnt=0;
			//內构
			if((*Modbus_InputIO[CylRe02]==0&&*Modbus_InputIO[CylSh02])&&(*Modbus_InputIO[CylSh01]==0&&*Modbus_InputIO[CylRe01])) 		  //气缸2舒张且气缸1收缩
			{
				AllReSet_Flag |=0x02;
			}
			else
			{
				AllReSet_Flag=0;
				return 2;//error
			}
			//拧瓶
			if(*Modbus_InputIO[CylSh05]==0&&*Modbus_InputIO[CylRe05])   //气缸5收缩
			{
				AllReSet_Flag |=0x08;
			}
			else
			{
				AllReSet_Flag=0;
				return 5;//error
			}
		}
	}
	if(AllReSet_Flag&0x0A) //0x02+0x08  转盘转动8次清料
	{
		if(TurnTableTime<8)
		{
			if(Turntable_step==0&&StopStart_Flag==0)//在空闲步骤下才起效
			{
				if(TurnTableTime==1) //复位清料
				{
					switch(ClearStep)
					{
						case 0:
						{
							Cylinder01=1;//气缸01舒张
							Cylinder03=1;//气缸03舒张
							ClearStep=1;
							break;
						}
						case 1:
						{
							if(*Modbus_InputIO[CylSh01]&&(*Modbus_InputIO[CylRe01]==0)&&
								 *Modbus_InputIO[CylSh03]&&(*Modbus_InputIO[CylRe03]==0))   //气缸舒张且没收缩
							{
								ErrorTime_cnt=0;
								Cylinder01=0;//气缸01收缩
								Cylinder03=0;//气缸03收缩
								ClearStep=2;
							}
							else
							{
								ErrorTime_cnt++;
							}
							
							break;
						}
						case 2:
						{
							if(*Modbus_InputIO[CylSh01]==0&&(*Modbus_InputIO[CylRe01])&&
								 *Modbus_InputIO[CylSh03]==0&&(*Modbus_InputIO[CylRe03]))  			//气缸收缩且没舒张
							{
								ErrorTime_cnt=0;
								ClearStep=0;//清料完成
								TurntableConnect_Flag=1;//转盘控制标志
								Turntable_step=1;
								TurnTableTime++;
							}
							else
							{
								ErrorTime_cnt++;
							}
							break;
						}
						default :break;
					}
					if(ErrorTime_cnt>100)
					{
						ClearStep=0;
						AllReSet_Flag=0;
						TurnTableTime=0;
						ErrorTime_cnt=0;
						return 13;
					}
				}
				else
				{
					ClearStep=0;
					TurntableConnect_Flag=1;//转盘控制标志
					Turntable_step=1;
					TurnTableTime++;
				}
			}
		}
		else  //8次转完
		{
			AllReSet_Flag=0;
			TurnTableTime=0;
		}
		
	}
	return 0;
}


/*---------------------------------------------------按钮控制-----------------------------------------------------------*/
//按键总控制

static u8 CylinderAllConnect(u8 key)//不支持连按  //10ms进入一次
{
	u8 Error=0;
	u16 ClearProducts[2]={0,0};
	switch(key)
	{
		case StopStart: 	 //启动停机按钮
		{
			StopStart_Flag=Connect_Data[StopStart];
			break;
		}
		case CountClear: 	 //计数清零
		{
			Products_Cnt=0;
			Cylinder_Data[0]=0;//清除计数寄存器
			Cylinder_Data[1]=0;
			STMFLASH_Write(FLASH_SAVE_ADDR,ClearProducts,2);//写Flash数据
			
			Connect_Data[CountClear]=0;
			break;
		}
		case ReSet: 		 	 //整机复位
		{
			if(AllReSet_Flag==0)
			{
				AllReSet();
			}
			
			Connect_Data[ReSet]=0;
			break;
		}
		case BottleM:		 	 //瓶身上料
		{
			BottleM_Flag=1;
			Turntable_step=1;
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
				ScrewCapStep=1;
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
				TurntableConnect_Flag=1;//转盘控制标志
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
		default : break;
		
	}
	
	return Error;
	
}
u16 Products_Temp[2]={0,0};
u16 Products_Temp1[2]={0,0};
/*---------------------------------------------------总控制-----------------------------------------------------------*/
void ALLControl_10ms(void)
{
	
	static u8 Step_Flag=0;
	static u32 RunTime_Cnt=0;
	u8 ERROR=0;
	
	
	
	ERROR=CylinderAllConnect(Key_Scan());//key control
	if(ERROR)
	{
		All_ERROR=ERROR;
	}
	
	if(All_ERROR)//如果故障，急停
	{
		return;
	}
	
	
	ERROR=CylinderStepControl();//内构件下压控制
	if(ERROR)
	{
		All_ERROR=ERROR;
	}
	
	ERROR=TurntableConnect();//转盘控制 瓶身上料
	if(ERROR)
	{
		All_ERROR=ERROR;
	}
	
	ERROR=CapCylinderControl();//瓶盖下压控制
	if(ERROR)
	{
		All_ERROR=ERROR;
	}
	
	ERROR=ScrewCapControl();//拧瓶控制
	if(ERROR)
	{
		All_ERROR=ERROR;
	}
	
	ERROR=AllReSetstep();//复位检测
	if(ERROR)
	{
		All_ERROR=ERROR;
	}
	
	
	//机器启动 总流程控制
	if(StopStart_Flag)//复位结束才能启动
	{
		if(Turntable_step==0&&ScrewCapStep==0&&Step_Flag==0)//转盘转动完成 拧瓶完成
		{
			Step_Flag=1;
			if(FiestStart_Flag>5)//拧瓶初次启动前五次次不动
			{
				ScrewCapStep=1;//拧瓶=1;
				Products_Cnt++;//拧瓶一次数量加1
				Cylinder_Data[0]=Products_Cnt&0X0000FFFF;
				Cylinder_Data[1]=(Products_Cnt>>16)&0X0000FFFF;
				Cylinder_Data[2]=Systime_cnt-RunTime_Cnt;//一个周期计时ms
				RunTime_Cnt=Systime_cnt;
				STMFLASH_Write(FLASH_SAVE_ADDR,Cylinder_Data,2);//写Flash数据
			}	
		}
				
		if(Turntable_step==0&&CylinderStep==0&&CapCylinderStep==0&&Step_Flag&&ScrewCapStep==0)//转盘转动完成，且內构下压完成  拧瓶了瓶且完成
		{
			Step_Flag=0;
			Turntable_step=1;//转盘转动
			
			if(FiestStart_Flag<6)
			{
				FiestStart_Flag++;//初次启动标志
			}
			
			if(FiestStart_Flag>1)//内构初次启动前两次不动
			{
				CylinderStep=1;  //內构下压
			}
			
			if(FiestStart_Flag>3)//瓶盖初次启动前三次不动
			{
				CapCylinderStep=1;//瓶盖下压
			}
				
		}	
	}
}









