/*
 * @Author: sdegl
 * @LastEditTime: 2021-04-09 09:56:57
 * @FilePath: \softtimer\SoftTimer.c
 * @LastEditors: sdegl
 */
#include "SoftTimer.h"

typedef struct __soft_timer
{
    uint32_t timerout;
    struct __soft_timer *next;
}SoftTimer_t;

SoftTimer a;

void SoftTimer_Creat()
{
	
}

void SoftTimer_Stop()
{

}



void SoftTimer_Handler(void)
{

}

//soft_timer_t SoftTimer_GetTimeStamp(void)
//{
//    
//}



