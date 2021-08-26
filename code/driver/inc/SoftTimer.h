/*
 * @Author: sdegl
 * @LastEditTime: 2021-04-09 10:01:28
 * @FilePath: \softtimer\SoftTimer.h
 * @LastEditors: sdegl
 */
#ifndef __SOFTTIMER_H__
#define __SOFTTIMER_H__

#include <stdint.h>

//#define SOFT_TIMER_STRUCT
#ifdef SOFT_TIMER_STRUCT
typedef struct __soft_timer
{
    uint32_t timerout;
    struct __soft_timer *next;
}SoftTimer_t;
#endif

typedef struct __soft_timer SoftTimer;
#undef SOFT_TIMER_STRUCT
#endif