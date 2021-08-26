/*
 * @Author: sdegl
 * @LastEditTime: 2021-03-11 17:49:38
 * @FilePath: \RWC\code\driver\src\key.c
 */

#include "key.h"
#include "stm32wlxx.h"


void Key_Init(void)
{
    GPIO_InitTypeDef  gpio_init_structure = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_init_structure.Pin   = GPIO_PIN_1;
    gpio_init_structure.Mode  = GPIO_MODE_INPUT;
    gpio_init_structure.Pull  = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_init_structure);
}

uint8_t Key_GetInput1(void)
{
    return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
}

//uint8_t Key_GetInput2(void)
//{

//}

//uint8_t Key_GetInput3(void)
//{
//    
//}

