/*
 * @Author: sdegl
 * @Date: 2021-02-24 14:52:49
 * @LastEditTime: 2021-04-13 09:23:45
 * @Description  : main
 * @FilePath: \RWClora\code\user\src\main.c
 */
#include "main.h"
#include "SoftTimer.h"
#include "key.h"
#include "led.h"
#include "stm32_timer.h"
#include "stm32wlxx.h"
#include "subghz_app.h"
#include "timer_if.h"

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSIDiv = RCC_LSI_DIV1;
    RCC_OscInitStruct.HSEDiv = RCC_HSE_DIV1;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3|RCC_CLOCKTYPE_HCLK
                                |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                                |RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }
}

int main(void)
{
    HAL_Init();

    SystemClock_Config();
    Key_Init();
    UTIL_TIMER_Init();
    LED_Init();

    ButtonServer_Init();
    SubghzApp_Init();

    LED_OpenUntil(LED_POWER_ON, 0);

    while (1) {
        SubghzApp_Process();
    }
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    /*Don't enable SysTick if TIMER_IF is based on other counters (e.g. RTC) */
    /* USER CODE BEGIN HAL_InitTick_1 */

    /* USER CODE END HAL_InitTick_1 */
    return HAL_OK;
    /* USER CODE BEGIN HAL_InitTick_2 */

    /* USER CODE END HAL_InitTick_2 */
}

/**
  * @brief Provide a tick value in millisecond measured using RTC
  * @note This function overwrites the __weak one from HAL
  * @retval tick value
  */
uint32_t HAL_GetTick(void)
{
    /* TIMER_IF can be based onother counter the SysTick e.g. RTC */
    /* USER CODE BEGIN HAL_GetTick_1 */

    /* USER CODE END HAL_GetTick_1 */
    return TIMER_IF_GetTimerValue();
    /* USER CODE BEGIN HAL_GetTick_2 */

    /* USER CODE END HAL_GetTick_2 */
}

/**
  * @brief This function provides delay (in ms)
  * @param Delay: specifies the delay time length, in milliseconds.
  * @retval None
  */
void HAL_Delay(__IO uint32_t Delay)
{
    /* TIMER_IF can be based onother counter the SysTick e.g. RTC */
    /* USER CODE BEGIN HAL_Delay_1 */

    /* USER CODE END HAL_Delay_1 */
    TIMER_IF_DelayMs(Delay);
    /* USER CODE BEGIN HAL_Delay_2 */

    /* USER CODE END HAL_Delay_2 */
}
