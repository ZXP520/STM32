#ifndef __RS485_H
#define __RS485_H
#include "sys.h"
#include <stdbool.h>



void Timer7_Init(void);
void RS485_Init(void);
void RS485_Service(void);
extern u32 Cylinder_Data[7];
extern u32 Connect_Data[20];
#endif



