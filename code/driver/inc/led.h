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
 * @brief ��LEDֱ��ms����
 * 
 * @param led led����
 * @param ms ��ʱ�䣬0����
 */
void LED_OpenUntil(enum led_type led, uint32_t ms);

/**
 * @brief �ر�LED
 * 
 * @param type LED����
 */
void LED_Close(enum led_type type);
#endif
