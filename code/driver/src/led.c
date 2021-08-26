#include "led.h"
#include "stm32wlxx.h"
#include "timer.h"

static TimerEvent_t RedTimer;
static TimerEvent_t BlueTimer;
static TimerEvent_t GreenTimer;

static void LED_RedClose(void* parm)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
}

static void LED_GreenClose(void* parm)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
}

static void LED_BlueClose(void* parm)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
}

void LED_Init(void)
{
    GPIO_InitTypeDef  gpio_init_structure = {0};
	
    __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_init_structure.Pin   = GPIO_PIN_4 | GPIO_PIN_3;
	gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure.Pull  = GPIO_NOPULL;
	gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio_init_structure);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	
	TimerInit(&RedTimer, LED_RedClose);
	TimerInit(&BlueTimer, LED_BlueClose);
	TimerInit(&GreenTimer, LED_GreenClose);

    TimerSetValue(&RedTimer, 500);
    TimerSetValue(&BlueTimer, 500);
    TimerSetValue(&GreenTimer, 500);
}

 

void LED_RedToggle(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    TimerStart(&RedTimer);
}

void LED_BlueToggle(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
    TimerStart(&BlueTimer);
}

void LED_GreenToggle(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
    TimerStart(&GreenTimer);
}
