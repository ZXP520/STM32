#ifndef __RS485_H
#define __RS485_H
#include "sys.h"
#include <stdbool.h>


void Timer7_Init(void);
void RS485_Init(void);
void RS485_Service(void);
extern u16 Cylinder_Data[20];
extern u32 Connect_Data[20];
extern u32 Status_Data[20];
extern vu32 *Modbus_InputIO[100];
#endif



