/*
 * @Author: sdegl
 * @LastEditTime: 2021-03-11 17:49:38
 * @FilePath: \RWC\code\driver\src\key.c
 */

#include "key.h"
#include "stm32wlxx.h"

#define MESSAGE_INPUT_PORT GPIOA
#define MESSAGE_INPUT_PIN GPIO_PIN_4

// #define POWER_INPUT_PORT 
// #define POWER_INPUT_PIN 

void Key_Init(void)
{
    GPIO_InitTypeDef  gpio_init_structure = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_init_structure.Mode  = GPIO_MODE_INPUT;
    gpio_init_structure.Pull  = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    gpio_init_structure.Pin   = MESSAGE_INPUT_PIN;
    HAL_GPIO_Init(MESSAGE_INPUT_PORT, &gpio_init_structure);

    // gpio_init_structure.Pin   = GPIO_PIN_4;
    // HAL_GPIO_Init(GPIOA, &gpio_init_structure);
}

uint8_t Key_GetInput1(void)
{
    return HAL_GPIO_ReadPin(MESSAGE_INPUT_PORT, MESSAGE_INPUT_PIN);
}

// uint8_t Key_GetPower(void)
// {
//     return HAL_GPIO_ReadPin(MESSAGE_INPUT_PORT, MESSAGE_INPUT_PIN);
// }

