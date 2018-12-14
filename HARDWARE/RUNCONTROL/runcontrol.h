#ifndef __RUNCONTROL_H_
#define __RUNCONTROL_H_
#include "sys.h"
	
	
void RunControl(void);
void WorkRunControl(void);
void CylinderAllConnect(u8 key);//不支持连按
void RunStatus(void);
extern u32 Systime_cnt;
#endif


