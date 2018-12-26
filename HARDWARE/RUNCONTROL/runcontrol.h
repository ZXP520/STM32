#ifndef __RUNCONTROL_H_
#define __RUNCONTROL_H_
#include "sys.h"
	

#define CONNECT_LEN 20
#define N 20
void RunControl(void);
void WorkRunControl(void);
//u8 CylinderAllConnect(u8 key);//不支持连按
void RunStatus(void);
//u8 Key_Scan(void);							//键值扫描

void ALLControl_10ms(void);
extern u32 Systime_cnt;
#endif


