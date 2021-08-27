#include "relay.h"
#include "stm32wlxx.h"

struct relay
{
	uint16_t pin_close;
	GPIO_TypeDef * port_close;
    uint16_t pin_open;
	GPIO_TypeDef * port_open;
} led_config[RELAY_TYPE_NUM] = 
{
	{GPIO_PIN_2, GPIOA, GPIO_PIN_1, GPIOA},
	{GPIO_PIN_0, GPIOA, GPIO_PIN_8, GPIOB},
	{GPIO_PIN_7, GPIOB, GPIO_PIN_6, GPIOB},
};

/**
 * @brief �̵�����ʼ��
 */
void Relay_Init(void)
{
    GPIO_InitTypeDef  gpio_init_structure = {0};
	
    __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure.Pull  = GPIO_NOPULL;
	gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    for (uint8_t i = 0; i < RELAY_TYPE_NUM; i++) {
		gpio_init_structure.Pin = led_config[i].pin_close;
		HAL_GPIO_Init(led_config[i].port_close, &gpio_init_structure);
        gpio_init_structure.Pin = led_config[i].pin_open;
		HAL_GPIO_Init(led_config[i].port_open, &gpio_init_structure);
	}
}

/**
 * @brief �򿪼̵���
 * 
 * @param type 
 */
void Relay_Open(enum relay_type type)
{
    HAL_GPIO_WritePin(led_config[type].port_close, led_config[type].pin_close, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(led_config[type].port_open, led_config[type].pin_open, GPIO_PIN_SET);
}

/**
 * @brief �رռ̵���
 * 
 * @param type 
 */
void Relay_Close(enum relay_type type)
{
    HAL_GPIO_WritePin(led_config[type].port_close, led_config[type].pin_close, GPIO_PIN_SET);
    HAL_GPIO_WritePin(led_config[type].port_open, led_config[type].pin_open, GPIO_PIN_RESET);
}
