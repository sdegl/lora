/*
 * @Author: sdegl
 * @LastEditTime: 2021-03-22 15:37:47
 * @FilePath: \RWC\code\driver\inc\led.h
 */
#ifndef __LED_H__
#define __LED_H__
 
enum led_type{
    LED_POWER_ON,
    LED_LORA_MISS,
    LED_LOST_POWER,
    LED_GET,

    LED_TYPE_NUM,
};

void LED_Init(void);

/**
 * @brief 打开LED直到ms结束
 * 
 * @param led led类型
 * @param ms 打开时间，0常开
 */
void LED_OpenUntil(enum led_type led, uint32_t ms);

/**
 * @brief 关闭LED
 * 
 * @param type LED类型
 */
void LED_Close(enum led_type type);
#endif
