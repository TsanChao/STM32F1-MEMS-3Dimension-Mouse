#ifndef _DATA_CONVERT
#define _DATA_CONVERT
#include "stm32f10x.h"
int quat2Eular(float *quat,float *euler);
void Send_AHRS_Data(void);

#endif
