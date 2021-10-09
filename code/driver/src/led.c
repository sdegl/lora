#include "led.h"
#include "stm32_timer.h"
#include "stm32wlxx.h"

struct LED_GPIO_CONF {
	uint16_t pin;
	GPIO_TypeDef * port;
} led_config[LED_TYPE_NUM] = 
{
	{GPIO_PIN_11, GPIOA},
	{GPIO_PIN_12, GPIOB},
	{GPIO_PIN_2, GPIOB},
	{GPIO_PIN_10, GPIOA},
};

UTIL_TIMER_Object_t timer[LED_TYPE_NUM];

/**
 * @brief 关闭LED
 * 
 * @param type LED类型
 */
void LED_Close(enum led_type type)
{
	HAL_GPIO_WritePin(led_config[type].port, led_config[type].pin, GPIO_PIN_RESET);
}

/**
 * @brief LED初始化
 */
void LED_Init(void)
{
    GPIO_InitTypeDef  gpio_init_structure = {0};
	
    __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure.Pull  = GPIO_PULLUP;
	gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	for (uint8_t i = 0; i < LED_TYPE_NUM; i++) {
		gpio_init_structure.Pin = led_config[i].pin;
		HAL_GPIO_Init(led_config[i].port, &gpio_init_structure);
		UTIL_TIMER_Create(&timer[i], 0, UTIL_TIMER_ONESHOT, (void (*)(void *))LED_Close, i);
	}
}

/**
 * @brief 打开LED直到ms结束
 * 
 * @param led led类型
 * @param ms 打开时间，0常开
 */
void LED_OpenUntil(enum led_type led, unsigned int ms)
{
	HAL_GPIO_WritePin(led_config[led].port, led_config[led].pin, GPIO_PIN_SET);

	if (ms != 0) {
		UTIL_TIMER_StartWithPeriod(&timer[led], ms);
	}
}
 
